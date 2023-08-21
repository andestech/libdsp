/******************************************************************************
 * Copyright (C) 2010-2023 Arm Limited or its affiliates. All rights reserved.*
 * Copyright (C) 2018-2023 Andes Technology Corporation. All rights reserved. *
 *                                                                            *
 * SPDX-License-Identifier: Apache-2.0                                        *
 *                                                                            *
 * Licensed under the Apache License, Version 2.0 (the License); you may      *
 * not use this file except in compliance with the License.                   *
 * You may obtain a copy of the License at                                    *
 *                                                                            *
 * www.apache.org/licenses/LICENSE-2.0                                        *
 *                                                                            *
 * Unless required by applicable law or agreed to in writing, software        *
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT    *
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.           *
 * See the License for the specific language governing permissions and        *
 * limitations under the License.                                             *
 ******************************************************************************/

#include <config.h>
#include "internal_filtering_math.h"

/**
 * @ingroup filtering
 */

/**
 * @defgroup nlms Normalized LMS Filter
 *
 * The NLMS algorithm It is related to the Least Mean Square (LMS) adaptive
 * filter and includes an additional normalization factor which increases
 * the the adaptation rate of the filter.With input signal array
 * <code>src[n]</code> of length <code>size</code>and filter coefficient
 * array <code>f[k]</code> of length <code>coeff_size</code>, for k = 0, 1,
 * 2, ..., coeff_size - 1,
 *
 * 1> Calculate the output signal dst[n] from the adaptive filter as
 *          dst[n] = f[0] * src[n] + f[1] * dst[n - 1] + ...
 *                  + f[coeff_size - 1] * dst[n - coeff_size + 1]
 * 2> Calculate the error signal
 *          e[n] = d[n] (reference signal) - dst[n].
 * 3> Update the filter coefficients
 *          b[k] = b[k - 1] + u[n] * e[n] * x[n - k]
 *          where u[n] = mu / energy and energy = src[n] ^ 2
 *                  + src[n - 1] ^ 2 + ... + src[n - coeff_size + 1] ^ 2.
 *          for k = 0, 1, 2, ..., coeff_size - 1.
 * where <code>mu</code> is the step size and controls the rate of
 * coefficient convergence.
 *
 * The coefficients and state variables for a filter are stored together in
 * an instance data structure which is defined as follow. The state
 * variables are updated after each block of data is processed.
 *
 * <pre>
 *  For the floating-point instruction:
 *      struct {
 *          uint32_t  coeff_size;
 *          float32_t *state;
 *          float32_t *coeff;
 *          float32_t  mu;
 *          float32_t energy;
 *      } riscv_dsp_nlms_f32_t;
 *
 *      struct {
 *          uint32_t  coeff_size;
 *          (type) *state;
 *          (type) *coeff;
 *          (type) mu;
 *          uint8_t shift;
 *          (type) *recip;
 *          (type) energy;
 *      } riscv_dsp_nlms_(type)_t;
 *
 * where coeff_size number of filter coefficients in the filter.
 *           *state points to the state variable array of size
 *                  <code>coeff_size + size - 1</code>.
 *       Ex: {src[n-coeff_size+1], src[n-coeff_size], src[n-coeff_size-1],
 *              src[n-coeff_size-2]....src[0], src[1], ..., x[size-1]}
 *           *coeff points to the coefficient array of size
 *                  <code>coeff_size</code> and the coefficients are stored
 *                  in time reversed order.
 *       Ex: {b[coeff_size-1], b[coeff_size-2], b[N-2], ..., b[1], b[0]}
 *               mu step size that controls filter coefficient updates.
 *            shift bit shift applied to coefficients.
 *           *recip points to the reciprocal initial value table.
 *           energy previous frame energy.
 * </pre>
 */

/**
 * @addtogroup nlms
 * @{
 */

/* Instance Structue */
#define EPSILON 0.000000119209289f //  1/(1^23)  The mantissa in IEEE-754 is 23 bits.
// To prevent from dividing by zero and increase the convergence speed,
// EPSILON should be set close to zero.

/**
 * @brief Function for the floating-point NLMS filter.
 * @param[in]       *instance points to an instance of the LMS structure.
 * @param[in]       *src      points to the input block data.
 * @param[in]       *ref      points to the reference data.
 * @param[out]      *dst      points to the output data.
 * @param[out]      *err      points to the error data.
 * @param[in]       size      number of the blocksize.
 * @return none.
 */

/* function description */
void FUNC_ATTR_NO_HWLOOP
riscv_dsp_nlms_f32(riscv_dsp_nlms_f32_t * FUNC_RESTRICT instance,
               float32_t * FUNC_RESTRICT src, float32_t * FUNC_RESTRICT ref, float32_t * FUNC_RESTRICT dst, float32_t * FUNC_RESTRICT err,
               uint32_t size)
{
    float32_t *pState = instance->state;
    float32_t *px, *pb, *scurr;
    float32_t mu = instance->mu;
    uint32_t coeff_size = instance->coeff_size;
    uint32_t tapcnt;
    float32_t energy = instance->energy;
    float32_t sum, e = 0.0f, w = 0.0f;
    float32_t in;

    scurr = &(instance->state[(coeff_size - 1u)]);

    /* block process */
    while (size != 0u)
    {
        *scurr++ = *src;
        px = pState;
        pb = instance->coeff;
        sum = 0.0f;

        /* filter phase */
        in = *src++;
        energy += in * in;
        tapcnt = coeff_size;

        while (tapcnt != 0u)
        {
            sum += (*px++) * (*pb++);
            tapcnt--;
        }
        *dst++ = sum;

        /* update phase */
        e = (*ref++) - sum;
        *err++ = e;
        w = (e * mu) / (energy + EPSILON);
        px = pState;
        pb = instance->coeff;

        tapcnt = coeff_size;
        while (tapcnt != 0u)
        {
            *pb++ += w * (*px++);
            tapcnt--;
        }

        w = *pState++;
        energy -= w * w;
        size--;
    }

    /* clean up for next frame */
    instance->energy = energy;
    scurr = instance->state;
    tapcnt = (coeff_size - 1u);
    while (tapcnt != 0u)
    {
        *scurr++ = *pState++;
        tapcnt--;
    }
}

/**
 * @} end of nlms
 */
