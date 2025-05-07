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
 * @defgroup matrixmul Matrix multiplication
 *
 * Multiplication (often denoted by the cross symbol "x") is the
 * mathematical operation of scaling one number by another. This function
 * multiply an <code>row * col</code> matrix with an <code>col * col2</code>
 * matrix, the ouput results is in an <code>row * col2</code> matrix.
 *
 * <pre>
 *		dst(row, col) = src1(row, col) * src2(row, col);
 * </pre>
 *
 * Ex:
 *       _          _     _          _
 *      |  A11  A12  |   |  B11  B12  |
 *      |            | x |            |
 *      |_ A21  A22 _|   |_ B21  B22 _|
 *                   _                                                _
 *                  |  A11 * B11 + A12 * B21    A11 * B12 + A12 * B22  |
 *                = |                                                  |
 *                  |_ A21 * B11 + A22 + B21    A21 * B12 + A22 + B22 _|
 */

/**
 * @addtogroup matrixmul
 * @{
 */

/**
 * @brief Multiplication of two floating-point matrices.
 * @param[in]       *src1 points to the first input matrix.
 * @param[in]       *src2 points to the second input matrix.
 * @param[out]      *dst  points to the output matrix.
 * @param[in]       row   number of the first input matrix rows.
 * @param[in]       col   number of the first input matrix columns.
 * @param[in]       col2  number of the second input matrix columns.
 * @return none.
 */

/* function description */

void riscv_dsp_mat_mul_f64(const float64_t * FUNC_RESTRICT src1, const float64_t * FUNC_RESTRICT src2,
                       float64_t * FUNC_RESTRICT dst, uint32_t row, uint32_t col, uint32_t col2)
{
    const float64_t *A = src1;
    const float64_t *B = src2;
    const float64_t *InA = A;
    float64_t *C;
    float64_t sum;
    //double sum;
    uint32_t i, colcnt, col2cnt, rowcnt;

    i = 0u;
    rowcnt = row;
    do
    {
        C = dst + i;
        B = src2;

        /* Dot product of each row in src1 with each column in src2 */
        col2cnt = col2;
        do
        {
            sum = 0.0f;
            //sum = 0.0;
            A = InA;

            /* column loop */
            colcnt = col;
            do
            {
                sum += (*A++) * *B;
                B += col2;
                colcnt--;
            }
            while (colcnt != 0u);

            *C++ = sum;
            //*C++ = (float32_t)sum;
            col2cnt--;
            B = src2 + (col2 - col2cnt);
        }
        while (col2cnt != 0u);

        i = i + col2;
        InA = A;
        rowcnt--;
    }
    while (rowcnt != 0u);
}

/**
 * @} end of matrixmul
 */
