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

/**
 * @ingroup basic
 */

/**
 * @addtogroup basicmul
 * @{
 */

/**
 * @brief Multiplication of q31 vectors.
 * @param[in]       *src1 points to the first input vector.
 * @param[in]       *src2 points to the second input vector.
 * @param[out]      *dst  points to the output vector.
 * @param[in]       size  size of the vectors.
 * @return none.
 *
 * Ouput results will be saturated in Q31 range [0x80000000 0x7FFFFFFF].
 */

/* function description */
void riscv_dsp_mul_q31(q31_t * FUNC_RESTRICT src1, q31_t * FUNC_RESTRICT src2, q31_t * FUNC_RESTRICT dst, uint32_t size)
{
    while (size != 0u)
    {

        q31_t ret;
        q31_t a = *src1++;
        q31_t b = *src2++;

        if ((a == (q31_t)0x80000000) && (b == (q31_t)0x80000000))
        {
            ret = (q31_t)0x7FFFFFFF;
        }
        else
        {
            q63_t temp = (q63_t)a * b;
            ret = ((q31_t)(temp >> 32) << 1);
            //ret |= NDS_ISA_BTST((q31_t)temp, 31);
            ret |= ((uint32_t)temp) >> 31;  //Unlike btst, this can run at v3/v3m
        }
        *dst++ = ret;
        size--;
    }
}

/**
 * @} end of basicmul
 */
