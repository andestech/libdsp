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
#include <string.h>   //used to call the memset()

/* function description */
int32_t FUNC_ATTR_FFAST_MATH
riscv_dsp_mat_inv_f16(float16_t * FUNC_RESTRICT src, float16_t * FUNC_RESTRICT dst, uint32_t size)
{
    float16_t *in = src;
    float16_t *pt1, *pt2, *pt3, *pt4;
    float16_t *diagsrc, *rtsrc, *diagdst, *rtdst;
    float16_t tmp, tmp1 = 0.0f, tmp2;
    uint32_t i, j, k, l;
    uint32_t rowcnt, loopcnt;

    if (size == 0)
    {
        return -1;
    }

    //Create an identity matrix in dst.
    pt2 = dst;
    rowcnt = size;

    memset(pt2, 0, sizeof(float16_t)*size * size);
    for (i = 0; i < rowcnt; i++)
    {
        *pt2 = 1.0f;
        pt2 += (rowcnt + 1);
    }

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
