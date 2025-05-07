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

#include "internal_isa.h"

/**
 * @ingroup basic
 */

/**
 * @addtogroup basicshift
 * @{
 */

/**
 * @brief Shifts a q31 vector for a specified shift number.
 * @param[in]       *src the input vector.
 * @param[in]      shift number of shift bits. If (shift > 0) means shifts
 *                       left; (shift < 0) means shifts right.
 * @param[out]      *dst the output vector.
 * @param[in]       size size of the vectors.
 * @return none.
 *
 * The input and output are all saturated to q31 range [0x80000000 0x7FFFFFFF].
 */

/* function description */
void riscv_dsp_shift_q31(q31_t * FUNC_RESTRICT src, int8_t shift, q31_t * FUNC_RESTRICT dst, uint32_t size)
{

    q31_t tmp1, tmp2;
    uint32_t rest_cnt;

    rest_cnt = size & 1;
    size -= rest_cnt;

    //const int8_t sh = shift;

    if (shift < 0)
    {
        while (size != 0u)
        {
            tmp1 = *src++;
            tmp2 = *src++;
            *dst++ = (tmp1 >> (-shift));
            *dst++ = (tmp2 >> (-shift));
            size -= 2;
        }

        if (rest_cnt != 0u)
        {
            tmp1 = *src;
            *dst = (tmp1 >> (-shift));
        }
    }
    else
    {
        q31_t ret, rev_sh_val;
        while (size != 0u)
        {
            tmp1 = *src++;
            tmp2 = *src++;

            ret = tmp1 << shift;
            rev_sh_val = ret >> shift;
            if (tmp1 != rev_sh_val)
            {
                ret = (tmp1 >> 31) ^ 0x7FFFFFFF;
            }
            *dst++ = ret;

            ret = tmp2 << shift;
            rev_sh_val = ret >> shift;
            if (tmp2 != rev_sh_val)
            {
                ret = (tmp2 >> 31) ^ 0x7FFFFFFF;
            }
            *dst++ = ret;
            size -= 2;
        }

        if (rest_cnt != 0u)
        {
            tmp1 = *src;
            ret = tmp1 << shift;
            rev_sh_val = ret >> shift;
            if (tmp1 != rev_sh_val)
            {
                ret = (tmp1 >> 31) ^ 0x7FFFFFFF;
            }
            *dst = ret;
        }
    }


}

/**
 * @} end of basicabs
 */
