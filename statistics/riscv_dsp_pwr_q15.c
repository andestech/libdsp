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
 * @brief Sum of the squares of the q15 vector.
 * @param[in]       *src    points to the input vector.
 * @param[in]       size    size of the vectors.
 * @return Sum of the squares value.
 *
 * <b>Function notes:</b>
 *
 * The 1.15 format input is multiplied yields a 2.30 format, and then added
 * without saturation to a 64-bit accumulator in 34.30 format. Finally,
 * the return result is in 34.30 format.
 */
/* function description */
q63_t riscv_dsp_pwr_q15(const q15_t *src, uint32_t size)
{
    union64_t sum;
    q15_t val;
    q31_t temp, carry;
    uint32_t i = size;

    sum.d0 = 0;
    while (i > 0u)
    {
        i--;
        val = *src++;
        temp = (q31_t) val * val;
        sum.b32.uw0 += (uint32_t)temp;
        //use slt instruction to check overflow.
        carry = (sum.b32.uw0 < (uint32_t)temp) ? 1 : 0;
        sum.b32.w1 += carry;
    }
    return sum.d0;

}

/**
 * @} end of pwr.
 */
