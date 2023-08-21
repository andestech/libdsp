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

#include "internal_math_types.h"    //for union64_t

/**
 * @ingroup statistics
 */

/**
 * @addtogroup pwr
 * @{
 */

/**
 * @brief Sum of the squares of the q31 vector.
 * @param[in]       *src    points to the input vector.
 * @param[in]       size    size of the vectors.
 * @return Sum of the squares value.
 *
 * <b>Function notes:</b>
 *
 * The 1.31 format input is multiplied yields a 2.62 format and this result
 * is truncated to 2.48 format by discarding the lower 14 bits. The 2.48
 * result is then added without saturation to a 64-bit accumulator in 16.48
 * format. Finally, the return result is in 16.48 format.
 */

/* function description */
q63_t riscv_dsp_pwr_q31(const q31_t *src, uint32_t size)
{
#ifdef NDS_CPU_64
    q63_t sum = 0;
    q31_t tmp;
    uint32_t i = size;

    while (i != 0u)
    {
        tmp = *src++;
        sum += ((q63_t) tmp * tmp) >> 14u;
        i--;
    }
    return sum;
#else
    union64_t sum, temp;
    q31_t val;
    q31_t carry;
    uint32_t i = size;

    sum.d0 = 0;
    while (i > 0)
    {
        i--;
        val = *src++;
        temp.d0 = (q63_t) val * val;
        temp.d0 >>= 14;
        sum.b32.uw0 += temp.b32.uw0;
        //use slt instruction to check overflow.
        carry = (sum.b32.uw0 < temp.b32.uw0) ? 1 : 0;
        sum.b32.w1 += carry;
        sum.b32.w1 += temp.b32.w1;
    }

    return sum.d0;
#endif
}

/**
 * @} end of pwr.
 */
