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
#include <string.h>

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
 * @param[out]      *dst  points to the output matrix.
 * @param[in]       size  number of the matrix row or col. i.e. if size is 5, the matrix size is 5x5.
 * @return none.
 */

/* function description */

int32_t riscv_dsp_mat_pwr2_cache_f64(const float64_t *src, float64_t *dst, uint32_t size)
{
    int testsize = size & 0x3;
    if (testsize != 0)
    {
        return -1;
    }


    const float64_t *A = src;
    float64_t *C = dst;
    memset( C, 0, size*size*sizeof(float64_t) );
    int m = size;
    int i, j, k;
    int ii, jj, kk;
    register int BlockSize = 4;
    double inputa, inputb;

    for (i = 0; i < m; i += (m / BlockSize))
    {
        for (j = 0; j < m; j += (m / BlockSize))
        {
            for (k = 0; k < m; k += (m / BlockSize))
            {
                for (ii = i; ii < i + (m / BlockSize); ii++)
                {
                    for (jj = j; jj < j + (m / BlockSize); jj++)
                    {
                        double prod, sum = 0.0;
                        kk = k;
                        inputa = *(A + ii * m + kk);
                        inputb = *(A + kk * m + jj);

                        for (kk = k; kk < k + (m / BlockSize); kk++)
                        {
                            prod = inputa * inputb; //Result[ii*m+jj] += inputa*inputb;
                            inputa = *(A + ii * m + (kk + 1));
                            inputb = *(A + (kk + 1) * m + jj);
                            sum += prod;
                        }
                        C[ii*m+jj] += sum;
                    }
                }
            }
        }
    }
    return 0;
}

/**
 * @} end of matrixmul
 */
