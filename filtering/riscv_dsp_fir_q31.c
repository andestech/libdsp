/******************************************************************************
 * Copyright (C) 2010-2025 Arm Limited or its affiliates. All rights reserved.*
 * Copyright (C) 2018-2025 Andes Technology Corporation. All rights reserved. *
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
 * @addtogroup fir
 * @{
 */

/**
 * @brief Function for the q31 FIR filter.
 * @param[in]       *instance points to an instance of the FIR structure.
 * @param[in]       *src      points to the input block data.
 * @param[out]      *dst      points to the output block data.
 * @param[in]       size      number of the blocksize.
 * @return none.
 *
 * Function notes:
 * Both coefficients and state variables are represented in 1.31 format
 * and multiplications yield a 2.62 result. The 2.62 results are accumulated
 * in a 64-bit accumulator and is right shifted by 31 bits and saturated to
 * 1.31 formatthe to yield the final result. In order to avoid overflows
 * completely the input signal must be scaled down by log2(coeff_size) bits.
 */

/* function description */
void riscv_dsp_fir_q31(const riscv_dsp_fir_q31_t * FUNC_RESTRICT instance,
                 q31_t * FUNC_RESTRICT src, q31_t * FUNC_RESTRICT dst, uint32_t size)
{
    q31_t *state = instance->state;
    q31_t *px, *pf, *scurr;
#ifdef ENA_HIGHER_PERFORMANCE_FIR
    q63_t acc0, acc1, acc2, acc3;
    q31_t state0, state1, state2, state3;
    q31_t c0;
    uint32_t sizecnt = size >> 2;
#else
    q63_t acc;
#endif
    uint32_t coeff_size = instance->coeff_size;
    uint32_t i, tapcnt;
    q31_t *coeff = instance->coeff;

    scurr = &(instance->state[(coeff_size - 1u)]);

    /* block loop */
#ifdef ENA_HIGHER_PERFORMANCE_FIR
    while (sizecnt != 0)
    {
        *scurr++ = *src++;
        *scurr++ = *src++;
        *scurr++ = *src++;
        *scurr++ = *src++;
        acc0 = 0;
        acc1 = 0;
        acc2 = 0;
        acc3 = 0;

        px = state;
        pf = coeff;

        state0 = *(px++);
        state1 = *(px++);
        state2 = *(px++);

        tapcnt = coeff_size >> 2;
        i = tapcnt;

        while (i != 0)
        {
            c0 = *pf;
            state3 = *px;
            acc0 += (q63_t) state0 * c0;
            acc1 += (q63_t) state1 * c0;
            acc2 += (q63_t) state2 * c0;
            acc3 += (q63_t) state3 * c0;
            c0 = *(pf + 1);
            state0 = *(px + 1);
            acc0 += (q63_t) state1 * c0;
            acc1 += (q63_t) state2 * c0;
            acc2 += (q63_t) state3 * c0;
            acc3 += (q63_t) state0 * c0;
            c0 = *(pf + 2);
            state1 = *(px + 2);
            acc0 += (q63_t) state2 * c0;
            acc1 += (q63_t) state3 * c0;
            acc2 += (q63_t) state0 * c0;
            acc3 += (q63_t) state1 * c0;
            c0 = *(pf + 3);
            state2 = *(px + 3);
            acc0 += (q63_t) state3 * c0;
            acc1 += (q63_t) state0 * c0;
            acc2 += (q63_t) state1 * c0;
            acc3 += (q63_t) state2 * c0;
            pf += 4;
            px += 4;
            i--;
        }

        i = coeff_size - (tapcnt * 4);
        while (i != 0)
        {
            c0 = *(pf++);
            state3 = *(px++);
            acc0 += (q63_t) state0 * c0;
            acc1 += (q63_t) state1 * c0;
            acc2 += (q63_t) state2 * c0;
            acc3 += (q63_t) state3 * c0;
            state0 = state1;
            state1 = state2;
            state2 = state3;
            i--;
        }
        state = state + 4;

        *dst++ = (q31_t) (acc0 >> 31u);
        *dst++ = (q31_t) (acc1 >> 31u);
        *dst++ = (q31_t) (acc2 >> 31u);
        *dst++ = (q31_t) (acc3 >> 31u);

        sizecnt--;
    }

    sizecnt = size % 4u;
    while (sizecnt != 0)
    {
        *scurr++ = *src++;
        acc0 = 0;
        px = state;
        pf = coeff;
        i = coeff_size;
        while (i != 0)
        {
            acc0 += (q63_t) * px++ **pf++;
            i--;
        }
        *dst++ = (q31_t) (acc0 >> 31u);
        state = state + 1;
        sizecnt--;
    }
#else
    while (size != 0u)
    {
        *scurr++ = *src++;
        acc = 0;

        /* Initialize filter coefficient and state pointer */
        px = state;
        pf = coeff;

        i = coeff_size;
        while (i >= 2)
        {
            acc += (q63_t) * px++ **pf++;
            acc += (q63_t) * px++ **pf++;
            i -= 2;
        }

        if (i > 0)
        {
            acc += (q63_t) * px++ **pf++;
        }

        *dst++ = (q31_t) (acc >> 31u);
        state = state + 1;
        size--;
    }
#endif
    scurr = instance->state;

    tapcnt = (coeff_size - 1u);
    while (tapcnt != 0u)
    {
        *scurr++ = *state++;
        tapcnt--;
    }
}

/**
 * @} end of fir
 */
