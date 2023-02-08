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
 * @ingroup matrix
 */

/**
 * @defgroup matrixinv Matrix Inverse
 *
 * This function computes the inverse of a matrix.
 * In linear algebra an n-by-n (square) matrix A is called invertible (some
 * peoples use nonsingular) if there exists an n-by-n matrix B such that:
 *
 *                    AB = BA = I (Identity matrix).
 *
 * If this is the case, then the matrix B is uniquely determined by A and
 * is called the inverse of A.
 *
 * In this function is used the Gauss-Jordan method to find the inverse.
 * This algorithm reduce the input matrix to an Identity matrix. Then do
 * the same row-operations to an Identity matrix yields the inverse matrix.
 * The input matrix must have the same number of rows and columns. In the
 * algorithm will check the matrix is singular or not.
 *
 * Gauss-Jordan Method:
 *  1. Combine the input matrix and identity matrix as follow:
 *         _                 _     _                 _
 *        |  A11  A12 | 1  0  |   |  1  0 | B11  B12  |
 *        |           |       | = |       |           |
 *        |_ A21  A22 | 0  1 _|   |_ 0  1 | B21  B22 _|
 *
 *  2. Star from first row m = 1.
 *  3. check to see if the elements (k) in main diagonal is zero. If the
 *     element is zero, exchange that row with a row below it that the
 *     elements in column m is not zero.
 *  4. Divide every elements of row m by k.
 *  5. For every below row, add an appropriate number by a multiple of k.
 *     Lead the elements in column m become zero.
 *  6. Move to the next row and repeat the steps 3 through 6 until the
 *     elements except for the main diagonal are all zero.
 *  7. The matrix B in the right is the output inverse matrix.
 */

/**
 * @addtogroup matrixinv
 t @{
 */

/**
 * @brief Compute the inverse matrix of the floating-point matrix.
 * @param[in]       *src  points to the input matrix.
 * @param[out]      *dst  points to the output matrix.
 * @param[in]       size  number of the matrix row or col.
 * @return the inverse process success or not.
 */

/* function description */
int32_t FUNC_ATTR_NO_HWLOOP
riscv_dsp_mat_inv_f64(float64_t * FUNC_RESTRICT src, float64_t * FUNC_RESTRICT dst, uint32_t size)
{
    float64_t *in = src;
    float64_t *pt1, *pt2, *pt3, *pt4;
    float64_t *diagsrc, *rtsrc, *diagdst, *rtdst;
    float64_t tmp, tmp1 = 0.0f, tmp2;
    uint32_t i, j, k, l;
    uint32_t rowcnt, loopcnt;

    if (size == 0)
    {
        return -1;
    }

    //Create an identity matrix in dst.
    pt2 = dst;
    rowcnt = size;
    do
    {
        j = size - rowcnt;
        while (j > 0u)
        {
            *pt2++ = 0.0f;
            j--;
        }

        *pt2++ = 1.0f;
        j = rowcnt - 1u;
        while (j > 0u)
        {
            *pt2++ = 0.0f;
            j--;
        }
        rowcnt--;
    }
    while (rowcnt != 0u);

    loopcnt = size;
    l = 0u; //l is col pointer
    do
    {
        pt1 = in + (l * size);
        pt3 = dst + (l * size);

        //Check the pivot is zero or not. If yes then swap with next row which pivot is not zero.
        tmp1 = *pt1;
        k = 1u;
        if (tmp1 == 0.0f)
        {
            //Now pivot is zero
            int32_t swapRow = 0;
            i = (l + 1u);
            while (i != size)
            {
                pt2 = pt1 + (size * k);
                pt4 = pt3 + (size * k);

                if (*pt2 != 0.0f)
                {
                    j = 0u;
                    do
                    {
                        tmp = *pt2;
                        *pt2++ = *pt1;
                        *pt1++ = tmp;
                        j++;
                    }
                    while (j != (size - l));

                    j = 0u;
                    do
                    {
                        tmp = *pt4;
                        *pt4++ = *pt3;
                        *pt3++ = tmp;
                        j++;
                    }
                    while (j != size);

                    swapRow = 1;
                    break;
                }
                k++;
                i++;
            }

            if (swapRow != 1)
            {
                return -1;
            }
        }

        //Begin elementary row operation
        pt1 = diagsrc = in + (l * size);
        pt2 = diagdst = dst + (l * size);
        tmp1 = *pt1;

        //Do row division to let pivot become 1.
        j = 0u;
        do
        {
            tmp2 = *pt1 / tmp1;
            *pt1++ = tmp2;
            j++;
        }
        while (j != (size - l));

        j = 0u;
        do
        {
            tmp2 = *pt2 / tmp1;
            *pt2++ = tmp2;
            j++;
        }
        while (j != size);

        //Do row operation to let pivot column at other rows become zero
        pt1 = in;
        pt2 = dst;
        i = 0u;
        do
        {
            if (i == l)
            {
                pt1 += size - l;
                pt2 += size;
            }
            else
            {
                tmp1 = *pt1;
                rtsrc = diagsrc;
                rtdst = diagdst;

                j = 0u;
                do
                {
                    tmp2 = *pt1 - (tmp1 **rtsrc++);
                    *pt1++ = tmp2;
                    j++;
                }
                while (j != (size - l));

                j = 0u;
                do
                {
                    tmp2 = *pt2 - (tmp1 **rtdst++);
                    *pt2++ = tmp2;
                    j++;
                }
                while (j != size);
            }
            pt1 = pt1 + l;
            i++;
        }
        while (i != size);

        in++;
        loopcnt--;
        l++;
    }
    while (loopcnt != 0u);

    return 0;
}

/**
 * @} end of matrixinv.
 */
