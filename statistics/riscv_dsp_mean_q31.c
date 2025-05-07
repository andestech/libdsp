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
#include "internal_utils_math.h"    //for union64_t and _riscv_dsp_div_s64_u32

/**
 * @ingroup statistics
 */

/**
 * @addtogroup mean
 * @{
 */

/**
 * @brief Mean value of the q31 vector.
 * @param[in]       *src    points to the input vector.
 * @param[in]       size    size of the vectors.
 * @return mean value.
 *
 * <b>Function notes:</b>
 *
 * The 1.31 format input is accumulated in a 64-bit accumulator in 33.31
 * format and then truncated to yield a result of 1.31 format.
 */

/* function description */
q31_t riscv_dsp_mean_q31(const q31_t *src, uint32_t size)
{
    union64_t sum;
    q63_t dst;
    sum.d0 = 0;
    uint32_t i = size;

    while (i != 0u)
    {
        q31_t sign, carry, val;
        uint32_t temp;

        //  If V3_O3 and unrolling, the insts as well as cycles are 6891/7139, and the performance is better than Keil.(Keil's cycle for CM3_O3 is 7892.)
        val = *src++;
        sign  = val >> 31;
        temp =  sum.b32.uw0 + (uint32_t) val;
        carry = (temp < sum.b32.uw0) ? 1 : 0;
        sum.b32.uw0 = temp;
        sum.b32.w1 += carry;
        sum.b32.w1 += sign;
        i--;
    }
#ifdef NDS_CPU_64
    dst = (sum.d0 / size);
#else
    dst = dsp_div_s64_u32(sum.d0, (int32_t)size);
#endif
    return dst;
}

/**
 * @} end of mean.
 */
