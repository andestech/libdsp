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

#include "internal_isa.h"

/**
 * @ingroup matrix
 */

/**
 * @addtogroup matrixmul
 * @{
 */

/**
 * @brief Multiplication of q7 vetor by matrix.
 * @param[in]       *src1 points to the first input vector.
 * @param[in]       *src2 points to the second input matrix.
 * @param[out]      *dst  points to the output vector.
 * @param[in]       col   number of the first input vector columns.
 * @param[in]       col2  number of the second input matrix columns.
 * @return none.
 *
 * <b>Function notes:</b>
 *
 * The 1.7 format input is multiplied yields a 2.15 format, and then added
 * without saturation to a 32-bit accumulator in 17.15 format. Finally,
 * the added output is truncated to 17.7 format by discarding the lower 7
 * bits, and then saturated to yield a result in 1.7 format.
 */


/* function description */
void riscv_dsp_mat_mul_vxm_q7(const q7_t * FUNC_RESTRICT src1, const q7_t * FUNC_RESTRICT src2,
                        q7_t * FUNC_RESTRICT dst, uint32_t col, uint32_t col2)
{
    uint32_t j;
    const q7_t *Aend = src1 + col;
    do
    {
        const q7_t *B = src2;

        j = col2;
        do
        {
            const q7_t *A = src1;
            const q7_t *A2 = A + col;
            q31_t sum = 0;
            do
            {
                sum += (q15_t) (*A++) * (*B);
                B += col2;
            }
            while (A != A2);
            *dst++ = NDS_ISA_SATS((sum >> 7), 8u);
            B = src2 + col2 - (--j);
        }
        while (j != 0);
        src1 += col;
    }
    while (src1 != Aend);
}

/**
 * @} end of matrixmul
 */
