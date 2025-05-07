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
 * @defgroup matrixtrans Matrix Transpose
 *
 * In linear algebra, the transpose of a matrix A is defined as reflect A
 * over its main diagonal to obtain A', write the rows of A as the columns
 * of A', write the columns of A as the rows of A'.
 *
 * <pre>
 *		dst(col, row) = src(row, col)';
 * </pre>
 *
 * Ex:
 *                               _          _ T
 *       _               _      |  A11  A21  |
 *      |  A11  A12  A13  |     |            |
 *      |                 |  =  |  A12  A22  |
 *      |_ A21  A22  A23 _|     |            |
 *                              |_ A13  A23 _|
 *
 */

/**
 * @addtogroup matrixtrans
 * @{
 */

/**
 * @brief Transpose the floating-potint matrices.
 * @param[in]       *src  points to the input matrix.
 * @param[out]      *dst  points to the output matrix.
 * @param[in]       row   number of the matrix rows.
 * @param[in]       col   number of the matrix columns.
 * @return none.
 */

/* function description */
void riscv_dsp_mat_trans_f32(const float32_t * FUNC_RESTRICT src, float32_t * FUNC_RESTRICT dst, uint32_t row,
                         uint32_t col)
{
    float32_t *C;
    uint32_t i, colcnt, rowcnt;

    i = 0u;
    rowcnt = row;
    //check if the input row or col is zero
    if ((row <= 0) || (col <= 0))
    {
        return;
    }

    do
    {
        C = dst + i;

        colcnt = col;
        do
        {
            *C = *src++;
            C += row;
            colcnt--;
        }
        while (colcnt != 0u);

        i++;
        rowcnt--;
    }
    while (rowcnt != 0u);
}

/**
 * @} end of matrixtrans
 */
