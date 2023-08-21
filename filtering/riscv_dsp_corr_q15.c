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
 * @ingroup filtering
 */

/**
 * @addtogroup corr
 * @{
 */

/**
 * @brief Correlation of the q15 vectors.
 * @param[in]       *src1 points to the first input vector.
 * @param[in]       len1  length of the first input vector.
 * @param[in]       *src2 points to the second input vector.
 * @param[in]       len2  length of the second input vector.
 * @param[out]      *dst  points to the output vector where the length is
 *                        2 * max(len1, len2) - 1.
 * @return none.
 *
 * Function notes:
 * Both inputs are in 1.15 format and multiplications yield a 2.30 result.
 * The 2.30 intermediate results are accumulated in a 64-bit accumulator in
 * 34.30 format. The 34.30 result is then truncated to 34.15 format by
 * discarding the low 15 bits and then saturated to 1.15 format.
 */

/* function description */
void riscv_dsp_corr_q15(q15_t * FUNC_RESTRICT src1, uint32_t len1, q15_t * FUNC_RESTRICT src2, uint32_t len2, q15_t * FUNC_RESTRICT dst)
{
    q15_t *pIn1;
    q15_t *pIn2;
    q15_t *pOut = dst;
    q63_t sum;
    q15_t *px;
    q15_t *py;
    q15_t *pSrc1;
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
        sum = 0;
        k = count >> 1u;
        while (k != 0u)
        {
            sum += ((q63_t) * px++ * *py++);
            sum += ((q63_t) * px++ * *py++);
            k--;
        }
        k = count % 0x2u;
        while (k > 0u)
        {
            sum += ((q63_t) * px++ * *py++);
            k--;
        }

        *pOut = (q15_t) NDS_ISA_SATS((sum >> 15u), 16u);
        pOut += inc;

        py = pSrc1 - count;
        px = pIn1;
        count++;
        blockSize1--;
    }

    px = pIn1;
    py = pIn2;
    count = 0u;

    //part2
    blkCnt = blockSize2;
    while (blkCnt > 0u)
    {
        sum = 0;
        k = len2 >> 2u;

        while (k > 0u)
        {
    sum += ((q63_t) * px++ * *py++);
    sum += ((q63_t) * px++ * *py++);
    sum += ((q63_t) * px++ * *py++);
    sum += ((q63_t) * px++ * *py++);
            k--;
        }
        k = len2 % 0x4u;
        while (k != 0u)
        {
    sum += ((q63_t) * px++ * *py++);
            k--;
        }
        *pOut = (q15_t) NDS_ISA_SATS((sum >> 15u), 16u);
        pOut += inc;
        count++;
        px = pIn1 + count;
        py = pIn2;
        blkCnt--;
    }

    //part3
    count = len2 - 1u;
    pSrc1 = (pIn1 + len1) - (len2 - 1u);
    px = pSrc1;
    py = pIn2;

    while (blockSize3 > 0u)
    {
        sum = 0;
        k = count >> 1u;
        while (k != 0u)
        {
            sum += ((q63_t) * px++ * *py++);
            sum += ((q63_t) * px++ * *py++);
            k--;
        }
        k = count % 0x2u;
        while (k > 0u)
        {
            sum += ((q63_t) * px++ * *py++);
            k--;
        }
        *pOut = (q15_t) NDS_ISA_SATS((sum >> 15u), 16u);
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
