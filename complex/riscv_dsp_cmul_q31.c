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
 * @ingroup complex
 */

/**
 * @addtogroup clx_mul
 * @{
 */

/**
 * @brief Multiply two q31 complex vector.
 * @param[in]       *src1 the first input complex vector.
 * @param[in]       *src2 the second input complex vector.
 * @param[out]      *dst  output complex vector.
 * @param[in]       size size of the vectors.
 * @return none.
 *
 * <b>Function notes:</b>
 *
 * The function implements 1.31 by 1.31 multiplications and finally output
 * is converted into 3.29 format. Input down scaling is not required.
 */

/* function description */
// y[2 * i] = x0[2 * i] * x1[2 * i] - x0[2 * i + 1] * x1[2 * i + 1].
// y[2 * i + 1] = x0[2 * i] * x1[2 * i + 1] + x0[2 * i + 1] * x1[2 * i].
void riscv_dsp_cmul_q31(const q31_t * FUNC_RESTRICT src1, const q31_t * FUNC_RESTRICT src2,
                  q31_t * FUNC_RESTRICT dst, uint32_t size)
{
    q31_t a, b, c, d;
    uint32_t cnt;
    q31_t mul1, mul2, mul3, mul4;

    cnt = size >> 1u;

    while (cnt != 0u)
    {
        a = *src1++;
        b = *src1++;
        c = *src2++;
        d = *src2++;
        mul1 = (((q31_t) (((q63_t) a * c) >> 32)) >> 1);
        mul2 = (((q31_t) (((q63_t) b * d) >> 32)) >> 1);
        mul3 = (((q31_t) (((q63_t) a * d) >> 32)) >> 1);
        mul4 = (((q31_t) (((q63_t) b * c) >> 32)) >> 1);

        *dst++ = (q31_t) (mul1 - mul2);
        *dst++ = (q31_t) (mul3 + mul4);

        a = *src1++;
        b = *src1++;
        c = *src2++;
        d = *src2++;
        mul1 = (((q31_t) (((q63_t) a * c) >> 32)) >> 1);
        mul2 = (((q31_t) (((q63_t) b * d) >> 32)) >> 1);
        mul3 = (((q31_t) (((q63_t) a * d) >> 32)) >> 1);
        mul4 = (((q31_t) (((q63_t) b * c) >> 32)) >> 1);

        *dst++ = (q31_t) (mul1 - mul2);
        *dst++ = (q31_t) (mul3 + mul4);
        cnt--;
    }

    cnt = size % 0x2u;
    while (cnt != 0u)
    {
        a = *src1++;
        b = *src1++;
        c = *src2++;
        d = *src2++;
        mul1 = (((q31_t) (((q63_t) a * c) >> 32)) >> 1);
        mul2 = (((q31_t) (((q63_t) b * d) >> 32)) >> 1);
        mul3 = (((q31_t) (((q63_t) a * d) >> 32)) >> 1);
        mul4 = (((q31_t) (((q63_t) b * c) >> 32)) >> 1);

        *dst++ = (q31_t) (mul1 - mul2);
        *dst++ = (q31_t) (mul3 + mul4);
        cnt--;
    }
}

/**
 * @} end of clx_mul
 */
