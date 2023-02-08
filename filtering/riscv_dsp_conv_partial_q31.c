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
 * @brief Convolution Partial of q31 vectors.
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
int32_t riscv_dsp_conv_partial_q31(q31_t * FUNC_RESTRICT src1, uint32_t len1,
                                   q31_t * FUNC_RESTRICT src2, uint32_t len2, q31_t * FUNC_RESTRICT dst, uint32_t startindex, uint32_t size)
{
    q31_t *in1 = src1;
    q31_t *in2 = src2;
    q31_t *out = dst;
    q31_t *inx, *iny, *tmpx;
    q31_t *psrc1, *psrc2;
    q63_t sum, acc0, acc1, acc2, acc3;
    q31_t x0, x1, x2, c0, x3;
    uint32_t i, j, k, count = 0u, check, blkcnt;
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
        psrc1 = in2 + startindex;
        iny = psrc1;

        //part1
        while (blockSize1 > 0)
        {
            sum = 0;
            i = count >> 2u;
            //use unroll loops to revmoe load-use data hazard
            while (i > 0u)
            {
                sum += (q63_t) * inx++ * *iny--;
                sum += (q63_t) * inx++ * *iny--;
                sum += (q63_t) * inx++ * *iny--;
                sum += (q63_t) * inx++ * *iny--;
                i--;
            }
            i = count % 0x4u;

            while (i > 0)
            {
                sum += (q63_t) * inx++ * *iny--;
                i--;
            }
            *out++ = (q31_t) (sum >> 31);
            iny = ++psrc1;
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

        if (len2 >= 4u)
        {
            blkcnt = blockSize2 >> 2;
            while (blkcnt > 0)
            {
                acc0 = 0;
                acc1 = 0;
                acc2 = 0;
                acc3 = 0;
                k = len2 >> 2;

                x0 = *(inx++);
                x1 = *(inx++);
                x2 = *(inx++);

                do
                {
                    c0 = *(iny);
                    x3 = *(inx);

                    acc0 += (q63_t) x0 * c0;
                    acc1 += (q63_t) x1 * c0;
                    acc2 += (q63_t) x2 * c0;
                    acc3 += (q63_t) x3 * c0;

                    c0 = *(iny - 1u);
                    x0 = *(inx + 1u);

                    acc0 += (q63_t) x1 * c0;
                    acc1 += (q63_t) x2 * c0;
                    acc2 += (q63_t) x3 * c0;
                    acc3 += (q63_t) x0 * c0;

                    c0 = *(iny - 2u);
                    x1 = *(inx + 2u);

                    acc0 += (q63_t) x2 * c0;
                    acc1 += (q63_t) x3 * c0;
                    acc2 += (q63_t) x0 * c0;
                    acc3 += (q63_t) x1 * c0;

                    c0 = *(iny - 3u);
                    x2 = *(inx + 3u);

                    acc0 += (q63_t) x3 * c0;
                    acc1 += (q63_t) x0 * c0;
                    acc2 += (q63_t) x1 * c0;
                    acc3 += (q63_t) x2 * c0;

                    inx += 4u;
                    iny -= 4u;
                }
                while (--k);

                //k = len2 - (3 * (len2 / 3));
                k = len2 & 0x3u;
                while (k > 0u)
                {
                    c0 = *(iny--);
                    x3 = *(inx++);

                    acc0 += (q63_t) x0 * c0;
                    acc1 += (q63_t) x1 * c0;
                    acc2 += (q63_t) x2 * c0;
                    acc3 += (q63_t) x3 * c0;

                    x0 = x1;
                    x1 = x2;
                    x2 = x3;
                    k--;
                }
                *out++ = (q31_t) (acc0 >> 31);
                *out++ = (q31_t) (acc1 >> 31);
                *out++ = (q31_t) (acc2 >> 31);
                *out++ = (q31_t) (acc3 >> 31);

                count += 4u;

                inx = tmpx + count;
                iny = psrc2;
                blkcnt--;
            }

            //blkcnt = blockSize2 - 3 * (blockSize2 / 3);
            blkcnt = blockSize2 & 0x3u;

            //part2
            while (blkcnt > 0)
            {
                sum = 0;

                i = len2 >> 2u;
                //use unroll loops to revmoe load-use data hazard
                while (i > 0u)
                {
                    sum += (q63_t) * inx++ * *iny--;
                    sum += (q63_t) * inx++ * *iny--;
                    sum += (q63_t) * inx++ * *iny--;
                    sum += (q63_t) * inx++ * *iny--;
                    i--;
                }

                i = len2 % 4u;
                while (i > 0)
                {
                    sum += (q63_t) * inx++ * *iny--;
                    i--;
                }

                *out++ = (q31_t) (sum >> 31);
                count++;
                inx = tmpx + count;
                iny = psrc2;

                blkcnt--;
            }
        }
        else
        {
            //part2
            while (blockSize2 > 0)
            {
                sum = 0;

                i = len2;
                //use unroll loops to revmoe load-use data hazard
                while (i >= 2u)
                {
                    sum += (q63_t) * inx++ * *iny--;
                    sum += (q63_t) * inx++ * *iny--;
                    i = i - 2;
                }

                if (i > 0)
                {
                    sum += (q63_t) * inx++ * *iny--;
                }

                *out++ = (q31_t) (sum >> 31);
                count++;
                inx = tmpx + count;
                iny = psrc2;

                blockSize2--;
            }
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

            i = count >> 2u;
            //use unroll loops to revmoe load-use data hazard
            while (i > 0u)
            {
                sum += (q63_t) * inx++ * *iny--;
                sum += (q63_t) * inx++ * *iny--;
                sum += (q63_t) * inx++ * *iny--;
                sum += (q63_t) * inx++ * *iny--;
                i--;
            }

            i = count % 0x4u;
            while (i > 0)
            {
                sum += (q63_t) * inx++ * *iny--;
                i--;
            }

            *out++ = (q31_t) (sum >> 31);
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
