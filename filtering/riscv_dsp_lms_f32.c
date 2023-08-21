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
 * @defgroup lms Least Mean Square (LMS) Filters
 *
 * The LMS algorithm performs the following operations to update the
 * coefficients of an adaptive filter: With input signal array
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
 *          b[k] = b[k - 1] + mu * e[n] * x[n - k]
 *                  for k = 0, 1, 2, ..., coeff_size - 1.
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
 *      } riscv_dsp_lms_f32_t;
 *
 * For the q15 and q31 instruction:
 *      struct {
 *          uint32_t  coeff_size;
 *          (type) *state;
 *          (type) *coeff;
 *          (type) mu;
 *          (type) shift;
 *      } riscv_dsp_lms_(type)_t;
 *
 * where coeff_size number of filter coefficients.
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
 * </pre>
 */

/**
 * @addtogroup lms
 * @{
 */

/**
 * @brief Function for the floating-point LMS filter.
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
riscv_dsp_lms_f32(const riscv_dsp_lms_f32_t * FUNC_RESTRICT instance,
              float32_t * FUNC_RESTRICT src, float32_t * FUNC_RESTRICT ref, float32_t * FUNC_RESTRICT dst, float32_t * FUNC_RESTRICT err,
              uint32_t size)
{
    float32_t *state = instance->state;
    float32_t *px, *pf, *scurr;
    float32_t mu = instance->mu;
    uint32_t coeff_size = instance->coeff_size;
    uint32_t tapcnt;
    float32_t sum, w = 0.0f, e = 0.0f, d = 0.0f;

    scurr = &(instance->state[(coeff_size - 1u)]);

    /* block process */
    while (size != 0u)
    {
        *scurr++ = *src++;
        px = state;
        pf = instance->coeff;
        sum = 0.0f;

        /* filter phase */
        tapcnt = coeff_size;
        while (tapcnt != 0u)
        {
            sum += (*px++) * (*pf++);
            tapcnt--;
        }
        *dst++ = sum;

        /* update phase */
        d = (float32_t) (*ref++);
        e = d - sum;
        *err++ = e;
        w = e * mu;
        px = state;
        pf = instance->coeff;

        tapcnt = coeff_size;
        while (tapcnt != 0u)
        {
            *pf = *pf + (w * (*px++));
            pf++;
            tapcnt--;
        }

        state = state + 1;
        size--;
    }
    /* end of block process */

    /* clean up for next frame */
    scurr = instance->state;
    tapcnt = (coeff_size - 1u);
    while (tapcnt != 0u)
    {
        *scurr++ = *state++;
        tapcnt--;
    }
}

/**
 * @} end of lms
 */
