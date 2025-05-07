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

/**
 * @ingroup matrix
 */

/**
 * @defgroup matrixsub Matrix Subtraction
 *
 * Subtraction is defined as to add the two values, and one of them with a
 * negative sign (-) symbol. This function computes the subtraction of
 * two <code>row x col</code> matrices.
 *
 * <pre>
 *		dst(row, col) = src1(row, col) - src2(row, col);
 * </pre>
 *
 * Ex:
 *       _          _     _          _     _                      _
 *      |  A11  A12  |   |  B11  B12  |   |  A11 - B11  A12 - B12  |
 *      |            | - |            | = |                        |
 *      |_ A21  A22 _|   |_ B21  B22 _|   |_ A21 - B21  A22 - B22 _|
 *
 */

/**
 * @addtogroup matrixsub
 * @{
 */

/**
 * @brief Subtraction of two floating-potint matrices.
 * @param[in]       *src1 points to the first input matrix.
 * @param[in]       *src2 points to the second input matrix.
 * @param[out]      *dst  points to the output matrix.
 * @param[in]       row   number of the matrix rows.
 * @param[in]       col   number of the matrix columns.
 * @return none.
 */

/* function description */
void riscv_dsp_mat_sub_f32(const float32_t * FUNC_RESTRICT src1, const float32_t * FUNC_RESTRICT src2,
                       float32_t * FUNC_RESTRICT dst, uint32_t row, uint32_t col)
{
    uint32_t size = row * col;

    do
    {
        /* C(m,n) = A(m,n) + B(m,n) */
        *dst++ = (*src1++) - (*src2++);

        size--;
    }
    while (size != 0u);
}

/**
 * @} end of matrixsub
 */
