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

#include "internal_math_types.h"		//for union64_t
#include "internal_isa.h"

/**
 * @ingroup matrix
 */

/**
 * @addtogroup matrixmul
 * @{
 */

/**
 * @brief Multiplication of two q15 matrices.
 * @param[in]       *src1 points to the first input matrix.
 * @param[in]       *src2 points to the second input matrix.
 * @param[out]      *dst  points to the output matrix.
 * @param[in]       row   number of the first input matrix rows.
 * @param[in]       col   number of the first input matrix columns.
 * @param[in]       col2  number of the second input matrix columns.
 * @return none.
 *
 * <b>Function notes:</b>
 *
 * The 1.15 format input is multiplied yields a 2.30 format, and then added
 * without saturation to a 64-bit accumulator in 34.30 format. Finally,
 * the added output is truncated to 34.15 format by discarding the lower 15
 * bits, and then saturated to yield a result in 1.15 format.
 */


/* function description */
void riscv_dsp_mat_mul_q15(const q15_t * FUNC_RESTRICT src1, const q15_t * FUNC_RESTRICT src2,
                     q15_t * FUNC_RESTRICT dst, uint32_t row, uint32_t col, uint32_t col2)
{
    uint32_t j;
    union64_t sum;
    const q15_t *Aend = src1 + row * col;
    q31_t carry, val, sign;
    uint32_t temp;
    do
    {
        const q15_t *B = src2;
        /* Dot product of each row in src1 with each column in src2 */
        j = col2;
        do
        {
            const q15_t *A = src1;
            const q15_t *A2 = A + col;
            sum.d0 = 0;
            /* column loop */
            do
            {
                //  If V3_O3 and unrolling, the insts as well as cycles are 42882/45203,
                //  and the performance is better than Keil.(Keil's cycle for CM3_O3 is 45388.)
                val = (q31_t) (*A++) * (*B);
                sign = val >> 31;
                temp = sum.b32.uw0 + (uint32_t) val;
                carry = (temp < sum.b32.uw0) ? 1 : 0;
                sum.b32.w1 += sign;
                sum.b32.w0 = temp;
                sum.b32.w1 += carry;
                B += col2;
            }
            while (A != A2);

            *dst++ = (q15_t)NDS_ISA_SATS((sum.d0 >> 15), 16u);
            B = src2 + col2 - (--j);
        }
        while (j != 0u);

        src1 += col;
    }
    while (src1 != Aend);
}

/**
 * @} end of matrixmul
 */
