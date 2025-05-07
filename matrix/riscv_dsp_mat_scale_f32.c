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
 * @defgroup matrixscale Matrix Scale
 *
 * This scale function is defined as to multiply a <code>row * col</code>
 * matrix by a specify scalar value.
 * The function process each element of the matrix.
 *
 * For floating-point format, the algorithm is:
 *
 * <pre>
 *		dst(row, col) = src(row, col) * scale;
 * </pre>
 *
 * In the q15, and q31 functions, <code>scale</code> is a fractional
 * multiplication <code>scaleFract</code> and an arithmetic shift
 * <code>shift</code>. The shift allows the gain of the scaling operation
 * to exceed 1.0.
 *
 * The algorithm in Q7, Q15, and Q31 is:
 *
 * <pre>
 *		dst(row, col) = (src(row, col) * scale_fract) << shift,
 *                                                   0 <= n < size.
 * </pre>
 *
 * The overall scale factor is applied to:
 *
 * <pre>
 *		scale = scale_fract * (2 ^ shift).
 * </pre>

 * Ex:
 *       _          _             _                          _
 *      |  A11  A12  |           |  A11 * scale  A12 * scale  |
 *      |            | * scale = |                            |
 *      |_ A21  A22 _|           |_ A21 * scale  A22 * scale _|
 *
 */

/**
 * @addtogroup matrixscale
 * @{
 */

/**
 * @brief Multiply a scale value of floating-potint matrix.
 * @param[in]       *src points to the input matrix.
 * @param[in]       scale is the factor to be multiplied.
 * @param[out]      *dst  points to the output matrix.
 * @param[in]       row   number of the matrix rows.
 * @param[in]       col   number of the matrix columns.
 * @return none.
 */

/* function description */
void riscv_dsp_mat_scale_f32(const float32_t * FUNC_RESTRICT src, float32_t scale,
                         float32_t * FUNC_RESTRICT dst, uint32_t row, uint32_t col)
{
    //check if the input row or col is zero
    uint32_t size;

    if ((row <= 0) || (col <= 0))
    {
        return;
    }

    size = (uint32_t) (row * col);

    do
    {
        /* C(m,n) = A(m,n) * scale */
        *dst++ = (*src++) * scale;

        size--;
    }
    while (size != 0u);
}

/**
 * @} end of matrixscale
 */
