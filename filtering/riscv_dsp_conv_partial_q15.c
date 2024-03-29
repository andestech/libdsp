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
 * @brief Convolution Partial of the q15 vectors.
 * @param[in]       *src1 points to the first input vector.
 * @param[in]       len1  length of the first input vector.
 * @param[in]       *src2 points to the second input vector.
 * @param[in]       len2  length of the second input vector.
 * @param[out]      *dst  points to the output vector where the length is
 *                        len1 + len2 - 1.
 * @param[in]       startindex is the first output sample to start with.
 * @param[in]       size is the number of output points to be computed.
 * @return          Returns
 *                  0; success
 *                  -1; fail,  the input subset are not between 0 and len1+len2-2.
 */

/* function description */
int32_t riscv_dsp_conv_partial_q15(q15_t * FUNC_RESTRICT src1, uint32_t len1,
                                   q15_t * FUNC_RESTRICT src2, uint32_t len2, q15_t * FUNC_RESTRICT dst, uint32_t startindex, uint32_t size)
{
    q15_t *in1 = src1;
    q15_t *in2 = src2;
    q15_t *out = dst;
    q15_t *inx, *iny, *tmpx;
    q15_t *psrc1, *psrc2;
    q63_t sum;
    uint32_t i, j, k, count = 0u, check;
    int32_t blockSize1, blockSize2, blockSize3;

    if ((startindex + size) > (len1 + len2 - 1u))
    {
        return -1;
    }
    else
    {
        // adjust the length so that len1 >= len2
        if (len1 >= len2)
        {
            in1 = src1;
            in2 = src2;
        }
        else
        {
            in1 = src2;
            in2 = src1;

            j = len2;
            len2 = len1;
            len1 = j;
        }

        check = startindex + size;
        blockSize3 = ((int32_t)check > (int32_t)len1) ? (int32_t)check - (int32_t)len1 : 0;
        blockSize3 = ((int32_t)startindex > (int32_t)len1 - 1) ? blockSize3 - (int32_t)startindex + (int32_t)len1 : blockSize3;
        blockSize1 = ((int32_t) len2 - 1) - (int32_t) startindex;
        blockSize1 = (blockSize1 > 0) ? ((check > (len2 - 1u)) ? blockSize1 : (int32_t) size) : 0;
        blockSize2 = (int32_t) check - ((blockSize3 + blockSize1) + (int32_t) startindex);
        blockSize2 = (blockSize2 > 0) ? blockSize2 : 0;

        out = dst + startindex;
        count = 1u + startindex;

        inx = in1;
        psrc2 = in2 + startindex;
        iny = psrc2;
        //part1
        while (blockSize1 > 0)
        {
            sum = 0;

            i = count;
            while (i > 0u)
            {
                sum += (q15_t) * inx++ * *iny--;
                i--;
            }

            *out++ = NDS_ISA_SATS(sum >> 15u, 16u);
            iny = ++psrc2;
            inx = in1;

            count++;
            blockSize1--;
        }

        if ((int32_t)startindex - (int32_t)len2 + 1 > 0)
        {
            inx = in1 + startindex - len2 + 1;
        }
        else
        {
            inx = in1;
        }

        psrc2 = in2 + (len2 - 1u);
        iny = psrc2;
        count = 0u;
        tmpx = inx;

        //part2
        while (blockSize2 != 0u)
        {
            sum = 0;
            k = len2 >> 2u;
            while (k != 0u)
            {
        sum += (q63_t) * inx++ * *iny--;
        sum += (q63_t) * inx++ * *iny--;
        sum += (q63_t) * inx++ * *iny--;
        sum += (q63_t) * inx++ * *iny--;
                k--;
            }

            k = len2 % 0x4u;
            while (k != 0u)
            {
        sum += (q63_t) (*inx++) * (*iny--);
                k--;
            }

            *out++ = (q15_t) NDS_ISA_SATS((sum >> 15u), 16u);
            count++;
            inx = tmpx + count;
            iny = psrc2;
            blockSize2--;
        }

        //part3
        count = len2 - 1u;
        psrc1 = (in1 + len1) - (len2 - 1u);
        inx = psrc1;

        psrc2 = in2 + (len2 - 1u);
        iny = psrc2;

        while (blockSize3 > 0)
        {
            sum = 0;

            i = count;
            while (i > 0u)
            {
                sum += (q15_t) * inx++ * *iny--;
                i--;
            }

            *out++ = NDS_ISA_SATS(sum >> 15u, 16u);
            inx = ++psrc1;
            iny = psrc2;

            count--;
            blockSize3--;
        }
    }

    return 0;
}

/**
 * @} end of conv partial
*/
