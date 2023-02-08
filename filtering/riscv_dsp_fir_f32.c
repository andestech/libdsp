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
 * @defgroup fir Finite Impulse Response Filters
 *
 * Finite impulse response (FIR) filter is a filter whose impulse response
 * (or response to any finite length input) is of finite duration, because
 * it settles to zero in finite time. For a discrete-time FIR filter, the
 * output is a weighted sum of the current and a finite number of previous
 * values of the input. The operation is described by the following
 * equation:
 *
 * Each filter coefficient <code>f[n]</code> is multiplied by a state
 * variable which equals a previous input sample <code>src[n]</code>, This
 * function operate on blocks of input data <code>src[n]</code> for the
 * length <code>size</code> and each call to the function processes.
 *
 * <pre>
 *      dst[n] =  f[0] * src[n] + f[1] * src[n-1] + f[2] * src[n-2]
 *              + ...+ f[coeff_size - 1] * src[n - coeff_size + 1]
 * </pre>
 *
 * The coefficients and state variables for a filter are stored together in
 * an instance data structure which is defined as follow. The state
 * variables are updated after each block of data is processed and the
 * coefficients are untouched.
 *
 * <pre>
 *      struct {
 *          uint32_t coeff_size;
 *          (type) *state;
 *          (type) *coeff;
 *      } riscv_dsp_fir_(type)_t;
 *
 * where *state points to the state variable array of size
 *              <code>coeff_size + size - 1</code>.
 *       Ex: {src[n-coeff_size+1], src[n-coeff_size], src[n-coeff_size-1],
 *              src[n-coeff_size-2]....src[0], src[1], ..., x[size-1]}
 *       *coeff points to the coefficient array of size
 *              <code>coeff_size</code> and the coefficients are stored
 *              in time reversed order.
 *       Ex: {b[coeff_size-1], b[coeff_size-2], b[N-2], ..., b[1], b[0]}
 *       coeff_size number of filter coefficients in the filter.
 * </pre>
 */

/**
 * @addtogroup fir
 * @{
 */

/**
 * @brief Function for the floating-point FIR filter.
 * @param[in]       *instance points to an instance of the FIR structure.
 * @param[in]       *src      points to the input block data.
 * @param[out]      *dst      points to the output block data.
 * @param[in]       size      number of the blocksize.
 * @return none.
 */

/* function description */
void riscv_dsp_fir_f32(const riscv_dsp_fir_f32_t * FUNC_RESTRICT instance,
                   float32_t * FUNC_RESTRICT src, float32_t * FUNC_RESTRICT dst, uint32_t size)
{
    float32_t *state = instance->state;
    float32_t *px, *pf, *scurr;
    uint32_t coeff_size = instance->coeff_size;
    uint32_t i, tapcnt;
    float32_t acc;

    scurr = &(instance->state[(coeff_size - 1u)]);

    /* block loop */
    while (size != 0u)
    {
        *scurr++ = *src++;
        acc = 0.0f;

        /* Initialize filter coefficient and state pointer */
        px = state;
        pf = instance->coeff;

        i = coeff_size;
        while (i != 0u)
        {
            acc += *px++ **pf++;
            i--;
        }

        *dst++ = acc;
        state = state + 1;
        size--;
    }

    scurr = instance->state;
    tapcnt = coeff_size - 1u;
    while (tapcnt != 0u)
    {
        *scurr++ = *state++;
        tapcnt--;
    }
}

/**
 * @} end of fir
 */
