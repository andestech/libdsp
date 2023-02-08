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
 * @ingroup filtering
 */

/**
 * @defgroup corr Correlation
 *
 * Correlation is a mathematical operation on two finite length vectors,
 * producing a third finite length output vector. Correlation is
 * similar to convolution. Correlation is commonly used to measure the
 * similarity between two signals. The algorithm is as follow:
 *
 * Let <code>src1[n]</code> and <code>src2[n]</code> be the input vectors of
 * length <code>len1</code> and <code>len2</code>. Then the correlation
 *
 * <pre>
 *               dst[n] = src1[n] * src2[-n];
 * </pre>
 *
 * is defined as
 *
 * <pre>
 *                  len1
 *         dst[n] = sum (src1[k] x src2[k - n])
 *                  k=0
 * </pre>
 * where <code>dst[n]</code> is of length
 * <code>2 * max(len1, len2) - 1</code> and the interval is defined as
 * <code>n = 0, 1, ..., (2 * max(len1, len2) - 2)</code>.
 *
 * The algorithm implementation is based on the lengths of the inputs.
 * The length <code>src2[n]</code> is always made to slide across
 * <code>src1[n]</code>. So <code>len2</code> is always considered as
 * shorter or equal to <code>len1</code>. But CORR(x, y) is reverse of
 * CORR(y, x). When <code>len2</code> larger then <code>len1</code>,
 * output pointer is made to point to the end of the output buffer and
 * a varaible, <code>inv</code> is set to 1.
 *
 * If lengths are not equal then zero pad has to be done to  make the two
 * inputs of same length. But to improve the performance, we include zeroes
 * in the output instead of zero padding either of the the inputs.
 * If <code>len1</code> larger then <code>len2</code>,
 * <code>len1 - len2</code> zeroes has to include in the ending of the
 * output buffer. Once the zero padding is done the remaining of the output
 * is calculated using convolution but with the shorter signal time shifted.
 */

/**
 * @addtogroup corr
 * @{
 */

/**
 * @brief Correlation of the floating-point vectors.
 * @param[in]       *src1 points to the first input vector.
 * @param[in]       len1  length of the first input vector.
 * @param[in]       *src2 points to the second input vector.
 * @param[in]       len2  length of the second input vector.
 * @param[out]      *dst  points to the output vector where the length is
 *                        2 * max(len1, len2) - 1.
 * @return none.
 */

/* function description */
void riscv_dsp_corr_f32(float32_t * FUNC_RESTRICT src1, uint32_t len1,
                  float32_t * FUNC_RESTRICT src2, uint32_t len2, float32_t * FUNC_RESTRICT dst)
{
    float32_t *pIn1;
    float32_t *pIn2;
    float32_t *pOut = dst;
    float32_t sum;
    float32_t *px;
    float32_t acc0, acc1, acc2, acc3;
    float32_t x0, x1, x2, c0, x3;
    float32_t *py;
    float32_t *pSrc1;
    uint32_t j, k = 0u, count, blkCnt, outBlockSize, blockSize1, blockSize2, blockSize3;
    int32_t inc = 1;

    if (len1 >= len2)
    {
        pIn1 = (src1);
        pIn2 = (src2);
        outBlockSize = (2u * len1) - 1u;
        j = outBlockSize - (len1 + (len2 - 1u));
        pOut += j;

    }
    else
    {
        pIn1 = (src2);
        pIn2 = (src1);
        j = len2;
        len2 = len1;
        len1 = j;
        pOut = dst + ((len1 + len2) - 2u);
        inc = -1;

    }
    blockSize1 = len2 - 1u;
    blockSize2 = len1 - (len2 - 1u);
    blockSize3 = blockSize1;

    //part1
    count = 1u;
    px = pIn1;
    pSrc1 = pIn2 + (len2 - 1u);
    py = pSrc1;

    while (blockSize1 > 0u)
    {
        sum = 0.0f;
        k = count;
        while (k > 0u)
        {
            sum += (* px++ * *py++);
            k--;
        }

        *pOut = sum;
        pOut += inc;

        py = pSrc1 - count;
        px = pIn1;
        count++;
        blockSize1--;
    }

    px = pIn1;
    py = pIn2;
    count = 0u;

    if (len2 >= 4u)
    {
        blkCnt = blockSize2 >> 2;
        while (blkCnt > 0u)
        {
            acc0 = 0.0f;
            acc1 = 0.0f;
            acc2 = 0.0f;
            acc3 = 0.0f;

            x0 = *(px++);
            x1 = *(px++);
            x2 = *(px++);
            k = len2 >> 2;

            do
            {
                c0 = *(py);
                x3 = *(px);
                acc0 += (x0 * c0);
                acc1 += (x1 * c0);
                acc2 += (x2 * c0);
                acc3 += (x3 * c0);
                c0 = *(py + 1u);
                x0 = *(px + 1u);
                acc0 += (x1 * c0);
                acc1 += (x2 * c0);
                acc2 += (x3 * c0);
                acc3 += (x0 * c0);

                c0 = *(py + 2u);
                x1 = *(px + 2u);
                acc0 += (x2 * c0);
                acc1 += (x3 * c0);
                acc2 += (x0 * c0);
                acc3 += (x1 * c0);

                c0 = *(py + 3u);
                x2 = *(px + 3u);
                acc0 += (x3 * c0);
                acc1 += (x0 * c0);
                acc2 += (x1 * c0);
                acc3 += (x2 * c0);

                px += 4u;
                py += 4u;
            }
            while (--k);
            k = len2 & 0x3u;

            while (k > 0u)
            {
                c0 = *(py++);
                x3 = *(px++);

                acc0 += (x0 * c0);
                acc1 += (x1 * c0);
                acc2 += (x2 * c0);
                acc3 += (x3 * c0);
                x0 = x1;
                x1 = x2;
                x2 = x3;
                k--;
            }
            *pOut = acc0;
            pOut += inc;

            *pOut = acc1;
            pOut += inc;

            *pOut = acc2;
            pOut += inc;

            *pOut = acc3;
            pOut += inc;

            count += 4u;

            px = pIn1 + count;
            py = pIn2;
            blkCnt--;
        }


        blkCnt = blockSize2 & 0x3u;
        while (blkCnt > 0u)
        {
            sum = 0;
            k = len2;
            while (k > 0u)
            {
                sum += * px++ * (*py++);
                k--;
            }

            *pOut = sum;
            pOut += inc;
            count++;
            px = pIn1 + count;
            py = pIn2;
            blkCnt--;
        }
    }
    else
    {
        blkCnt = blockSize2;

        while (blkCnt > 0u)
        {
            sum = 0;
            k = len2;
            while (k > 0u)
            {
                sum += * px++ * (*py++);
                k--;
            }

            *pOut = sum;
            pOut += inc;
            count++;
            px = pIn1 + count;
            py = pIn2;
            blkCnt--;
        }
    }

    //part3
    count = len2 - 1u;
    pSrc1 = (pIn1 + len1) - (len2 - 1u);
    px = pSrc1;
    py = pIn2;

    while (blockSize3 > 0u)
    {
        sum = 0.0f;
        k = count;
        while (k > 0u)
        {
            sum += (* px++ * *py++);
            k--;
        }
        *pOut = sum;
        pOut += inc;
        px = ++pSrc1;
        py = pIn2;
        count--;
        blockSize3--;
    }
}

/**
 * @} end of corr
 */
