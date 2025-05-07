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

static inline q31_t riscv_dsp_c_kmmac_u (q31_t acc, q31_t x, q31_t y)
{
    union64_t temp;
    temp.d0 = (q63_t)x * y;
    if ((temp.b32.w0 & ((q31_t)1 << 31)) != 0)
    {
        temp.b32.w1 += 1;
    }
    return (acc + temp.b32.w1);
}

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
void riscv_dsp_fir_fast_q31(const riscv_dsp_fir_q31_t * FUNC_RESTRICT instance,
                      q31_t * FUNC_RESTRICT src, q31_t * FUNC_RESTRICT dst, uint32_t size)
{
    q31_t *state = instance->state;
    q31_t *px, *pf, *scurr;
    q31_t acc0, acc1, acc2, acc3, acc4, acc5, acc6, acc7;
    uint32_t coeff_size = instance->coeff_size;
    uint32_t i, tapcnt;
    q31_t *coeff = instance->coeff;
    q31_t state0, state1, state2, state3, state4, state5, state6, state7;
    q31_t c0;

    scurr = &(instance->state[(coeff_size - 1u)]);
    uint32_t sizecnt = size >> 3;

    /* block loop */
    while (sizecnt != 0)
    {
        *scurr++ = *src++;
        *scurr++ = *src++;
        *scurr++ = *src++;
        *scurr++ = *src++;
        *scurr++ = *src++;
        *scurr++ = *src++;
        *scurr++ = *src++;
        *scurr++ = *src++;

        px = state;
        pf = coeff;

        state0 = *(px++);
        state1 = *(px++);
        state2 = *(px++);
        state3 = *(px++);
        state4 = *(px++);
        state5 = *(px++);
        state6 = *(px++);

        acc0 = 0;
        acc1 = 0;
        acc2 = 0;
        acc3 = 0;
        acc4 = 0;
        acc5 = 0;
        acc6 = 0;
        acc7 = 0;

        tapcnt = coeff_size >> 3;
        i = tapcnt;

        while (i != 0)
        {
            c0 = *pf;
            state7 = *px;
    acc0 = riscv_dsp_c_kmmac_u(acc0, state0, c0);
    acc1 = riscv_dsp_c_kmmac_u(acc1, state1, c0);
    acc2 = riscv_dsp_c_kmmac_u(acc2, state2, c0);
    acc3 = riscv_dsp_c_kmmac_u(acc3, state3, c0);
    acc4 = riscv_dsp_c_kmmac_u(acc4, state4, c0);
    acc5 = riscv_dsp_c_kmmac_u(acc5, state5, c0);
    acc6 = riscv_dsp_c_kmmac_u(acc6, state6, c0);
    acc7 = riscv_dsp_c_kmmac_u(acc7, state7, c0);
            c0 = *(pf + 1);
            state0 = *(px + 1);
    acc0 = riscv_dsp_c_kmmac_u(acc0, state1, c0);
    acc1 = riscv_dsp_c_kmmac_u(acc1, state2, c0);
    acc2 = riscv_dsp_c_kmmac_u(acc2, state3, c0);
    acc3 = riscv_dsp_c_kmmac_u(acc3, state4, c0);
    acc4 = riscv_dsp_c_kmmac_u(acc4, state5, c0);
    acc5 = riscv_dsp_c_kmmac_u(acc5, state6, c0);
    acc6 = riscv_dsp_c_kmmac_u(acc6, state7, c0);
    acc7 = riscv_dsp_c_kmmac_u(acc7, state0, c0);
            c0 = *(pf + 2);
            state1 = *(px + 2);
    acc0 = riscv_dsp_c_kmmac_u(acc0, state2, c0);
    acc1 = riscv_dsp_c_kmmac_u(acc1, state3, c0);
    acc2 = riscv_dsp_c_kmmac_u(acc2, state4, c0);
    acc3 = riscv_dsp_c_kmmac_u(acc3, state5, c0);
    acc4 = riscv_dsp_c_kmmac_u(acc4, state6, c0);
    acc5 = riscv_dsp_c_kmmac_u(acc5, state7, c0);
    acc6 = riscv_dsp_c_kmmac_u(acc6, state0, c0);
    acc7 = riscv_dsp_c_kmmac_u(acc7, state1, c0);
            c0 = *(pf + 3);
            state2 = *(px + 3);
    acc0 = riscv_dsp_c_kmmac_u(acc0, state3, c0);
    acc1 = riscv_dsp_c_kmmac_u(acc1, state4, c0);
    acc2 = riscv_dsp_c_kmmac_u(acc2, state5, c0);
    acc3 = riscv_dsp_c_kmmac_u(acc3, state6, c0);
    acc4 = riscv_dsp_c_kmmac_u(acc4, state7, c0);
    acc5 = riscv_dsp_c_kmmac_u(acc5, state0, c0);
    acc6 = riscv_dsp_c_kmmac_u(acc6, state1, c0);
    acc7 = riscv_dsp_c_kmmac_u(acc7, state2, c0);
            c0 = *(pf + 4);
            state3 = *(px + 4);
    acc0 = riscv_dsp_c_kmmac_u(acc0, state4, c0);
    acc1 = riscv_dsp_c_kmmac_u(acc1, state5, c0);
    acc2 = riscv_dsp_c_kmmac_u(acc2, state6, c0);
    acc3 = riscv_dsp_c_kmmac_u(acc3, state7, c0);
    acc4 = riscv_dsp_c_kmmac_u(acc4, state0, c0);
    acc5 = riscv_dsp_c_kmmac_u(acc5, state1, c0);
    acc6 = riscv_dsp_c_kmmac_u(acc6, state2, c0);
    acc7 = riscv_dsp_c_kmmac_u(acc7, state3, c0);
            c0 = *(pf + 5);
            state4 = *(px + 5);
    acc0 = riscv_dsp_c_kmmac_u(acc0, state5, c0);
    acc1 = riscv_dsp_c_kmmac_u(acc1, state6, c0);
    acc2 = riscv_dsp_c_kmmac_u(acc2, state7, c0);
    acc3 = riscv_dsp_c_kmmac_u(acc3, state0, c0);
    acc4 = riscv_dsp_c_kmmac_u(acc4, state1, c0);
    acc5 = riscv_dsp_c_kmmac_u(acc5, state2, c0);
    acc6 = riscv_dsp_c_kmmac_u(acc6, state3, c0);
    acc7 = riscv_dsp_c_kmmac_u(acc7, state4, c0);
            c0 = *(pf + 6);
            state5 = *(px + 6);
    acc0 = riscv_dsp_c_kmmac_u(acc0, state6, c0);
    acc1 = riscv_dsp_c_kmmac_u(acc1, state7, c0);
    acc2 = riscv_dsp_c_kmmac_u(acc2, state0, c0);
    acc3 = riscv_dsp_c_kmmac_u(acc3, state1, c0);
    acc4 = riscv_dsp_c_kmmac_u(acc4, state2, c0);
    acc5 = riscv_dsp_c_kmmac_u(acc5, state3, c0);
    acc6 = riscv_dsp_c_kmmac_u(acc6, state4, c0);
    acc7 = riscv_dsp_c_kmmac_u(acc7, state5, c0);
            c0 = *(pf + 7);
            state6 = *(px + 7);
    acc0 = riscv_dsp_c_kmmac_u(acc0, state7, c0);
    acc1 = riscv_dsp_c_kmmac_u(acc1, state0, c0);
    acc2 = riscv_dsp_c_kmmac_u(acc2, state1, c0);
    acc3 = riscv_dsp_c_kmmac_u(acc3, state2, c0);
    acc4 = riscv_dsp_c_kmmac_u(acc4, state3, c0);
    acc5 = riscv_dsp_c_kmmac_u(acc5, state4, c0);
    acc6 = riscv_dsp_c_kmmac_u(acc6, state5, c0);
    acc7 = riscv_dsp_c_kmmac_u(acc7, state6, c0);
            pf += 8;
            px += 8;
            i--;
        }
        i = coeff_size - (tapcnt * 8);
        while (i != 0)
        {
            c0 = *(pf++);
            state7 = *(px++);
    acc0 = riscv_dsp_c_kmmac_u(acc0, state0, c0);
    acc1 = riscv_dsp_c_kmmac_u(acc1, state1, c0);
    acc2 = riscv_dsp_c_kmmac_u(acc2, state2, c0);
    acc3 = riscv_dsp_c_kmmac_u(acc3, state3, c0);
    acc4 = riscv_dsp_c_kmmac_u(acc4, state4, c0);
    acc5 = riscv_dsp_c_kmmac_u(acc5, state5, c0);
    acc6 = riscv_dsp_c_kmmac_u(acc6, state6, c0);
    acc7 = riscv_dsp_c_kmmac_u(acc7, state7, c0);
            state0 = state1;
            state1 = state2;
            state2 = state3;
            state3 = state4;
            state4 = state5;
            state5 = state6;
            state6 = state7;
            i--;
        }
        state = state + 8;

        *dst++ = (acc0 << 1);
        *dst++ = (acc1 << 1);
        *dst++ = (acc2 << 1);
        *dst++ = (acc3 << 1);
        *dst++ = (acc4 << 1);
        *dst++ = (acc5 << 1);
        *dst++ = (acc6 << 1);
        *dst++ = (acc7 << 1);

        sizecnt--;
    }

    sizecnt = size % 8u;
    while (sizecnt != 0)
    {
        *scurr++ = *src++;
        acc0 = 0;
        px = state;
        pf = coeff;
        i = coeff_size;
        while (i != 0)
        {
            acc0 = riscv_dsp_c_kmmac_u(acc0, (*px++), (*(pf++)));
            i--;
        }
        *dst++ = (acc0 << 1);
        state = state + 1;
        sizecnt--;
    }

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
