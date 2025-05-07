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
 * @addtogroup dcmfir
 * @{
 */

/**
 * @brief Function for the q31 Decimator FIR filter.
 * @param[in]       *instance points to an instance of the Decimator FIR
 *                            structure.
 * @param[in]       *src      points to the input block data.
 * @param[out]      *dst      points to the output block data.
 * @param[in]       size      number of the size.
 * @return none.
 *
 * Function notes:
 * Both coefficients and state variables are represented in 1.31 format
 * and multiplications yield a 2.62 result. The 2.62 results are accumulated
 * in a 64-bit accumulator. After all multiply-accumulates are performed, the
 * 2.62 accumulator is truncated to 1.32 format and then saturated to 1.31
 * format. In order to avoid overflows completely the input signal must be
 * scaled down by log2(coeff_size) bits.
 */

/* function description */
void riscv_dsp_dcmfir_fast_q31(const riscv_dsp_dcmfir_q31_t * FUNC_RESTRICT instance, q31_t * FUNC_RESTRICT  src,
                         q31_t * FUNC_RESTRICT dst, uint32_t size)
{
    q31_t *state = instance->state;
    q31_t *scurr, *px, *pb;
    q31_t sum;
    q31_t x1, c1;
    q31_t x0, c0;
    uint32_t csize = instance->coeff_size;
    uint32_t i, tapcnt, blkcnt, bsize = size / instance->M;

    scurr = instance->state + (csize - 1u);

    blkcnt = bsize;
    while (blkcnt != 0u)
    {
        i = instance->M;
        while (i)
        {
            LOOP_HINT_NO_HWLOOP();       //M is decimation factor and usually is small
            *scurr++ = *src++;
            i--;
        }

        sum = 0;
        px = state;
        pb = instance->coeff;
        tapcnt = csize;
        while (tapcnt >= 2u)
        {
            c0 = *pb++;
            x0 = *px++;
            c1 = *pb++;
            x1 = *px++;
            sum = (q31_t) ((((q63_t) sum << 32) + ((q63_t) x0 * c0)) >> 32);
            sum = (q31_t) ((((q63_t) sum << 32) + ((q63_t) x1 * c1)) >> 32);
            tapcnt -= 2;
        }
        if (tapcnt > 0)
        {
            c0 = *pb++;
            x0 = *px++;
            sum = (q31_t) ((((q63_t) sum << 32) + ((q63_t) x0 * c0)) >> 32);
        }

        state = state + instance->M;
        *dst++ = (q31_t) (sum << 1);
        blkcnt--;
    }
    scurr = instance->state;

    i = (csize - 1u);
    while (i != 0u)
    {
        *scurr++ = *state++;
        i--;
    }
}

/**
 * @} end of dcmfir
 */
