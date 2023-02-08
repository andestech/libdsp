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

#include "internal_math_types.h"		//for union64_t
#include "internal_isa.h"

/**
 * @ingroup matrix
 */

/**
 * @addtogroup matrixmul
 * @{
 */

typedef union
{
    struct
    {
        q15_t re;
        q15_t im;
    };
} union_clx_q15_t;

/* function description */
void riscv_dsp_cmat_mul_q15(const q15_t * FUNC_RESTRICT src1, const q15_t * FUNC_RESTRICT src2,
                      q15_t * FUNC_RESTRICT dst, uint32_t row, uint32_t col, uint32_t col2)
{
    uint32_t j;
    const q15_t *Aend = src1 + (row * col * 2);
    union64_t sum_re;
    union64_t sum_im;
    q31_t carry, sign;
    q31_t val_re;
    q31_t val_im;
    uint32_t temp;
    do
    {
        const union_clx_q15_t *B = (union_clx_q15_t *)src2;
        /* Dot product of each row in src1 with each column in src2 */
        j = col2;
        do
        {
            const union_clx_q15_t *A = (union_clx_q15_t *)src1;
            const union_clx_q15_t *A2 = (union_clx_q15_t *)(A + col);
            sum_re.d0 = 0;
            sum_im.d0 = 0;
            /* column loop */
            union_clx_q15_t a, b;
            do
            {
                a = *A++;
                b = *B;
                val_re = (q31_t) (a.re * b.re - a.im * b.im);
                sign = val_re >> 31;
                temp = sum_re.b32.w0 + (uint32_t) val_re;
                carry = (temp < sum_re.b32.uw0) ? 1 : 0;
                sum_re.b32.w1 += sign;
                sum_re.b32.w0 = temp;
                sum_re.b32.w1 += carry;

                val_im = (q31_t) (a.re * b.im + a.im * b.re);
                sign = val_im >> 31;
                temp = sum_im.b32.w0 + (uint32_t) val_im;
                carry = (temp < sum_im.b32.uw0) ? 1 : 0;
                sum_im.b32.w1 += sign;
                sum_im.b32.w0 = temp;
                sum_im.b32.w1 += carry;
                B += (col2);
            }
            while (A != A2);

            *dst++ = (q15_t)NDS_ISA_SATS((sum_re.d0 >> 15), 16u);
            *dst++ = (q15_t)NDS_ISA_SATS((sum_im.d0 >> 15), 16u);
            B = (union_clx_q15_t *)(src2 + 2 * (col2 - (--j)));
        }
        while (j != 0u);

        src1 += (col * 2);
    }
    while (src1 != Aend);
}

/**
 * @} end of matrixmul
 */
