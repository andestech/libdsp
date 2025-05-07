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
#include "riscv_dsp_matrix_math.h"
#include <string.h>

/// @private
#define COPY_COL_T(T, A, ROW, COL, DST)             \
{                                                   \
    uint32_t _row;                                  \
    T *_pb = DST;                                   \
    T *_pa = (A) + ROW * col + COL;                 \
    for(_row = ROW; _row < row; _row++)             \
    {                                               \
        *_pb++ = *_pa;                              \
        _pa += col;                                 \
    }                                               \
}

/* function description */
int riscv_dsp_mat_qr_f64(const float64_t * FUNC_RESTRICT src,
                         const float64_t threshold,
                         float64_t * FUNC_RESTRICT dstR,
                         float64_t * FUNC_RESTRICT dstQ,
                         float64_t * FUNC_RESTRICT tau,
                         float64_t * FUNC_RESTRICT pTmpA,
                         float64_t * FUNC_RESTRICT pTmpB,
                         const uint32_t row,
                         const uint32_t col)
{
    //row should be greater equal than col
    if (row < col)
    {
        return -1;
    }
    uint32_t JJ = 0;
    uint32_t nb, pos;
    float64_t *pa, *ptau;
    float64_t beta;
    float64_t *pv;
    float64_t *pdst;
    float64_t *p;

    memcpy(dstR, src, row * col * sizeof(float64_t));
    p = dstR;
    ptau = tau;
    for(JJ = 0; JJ < col; JJ++)
    {
        uint32_t i, j, k, blkCnt;
        float64_t *pa0, *pa1, *pa2, *pa3;
        COPY_COL_T(float64_t, dstR, JJ, JJ, pTmpA);

        beta = riscv_dsp_mat_householder_f64(pTmpA, threshold, row - JJ, pTmpA);
        *ptau++ = beta;

        pdst = pTmpB;

        /* v.T A(JJ:,JJ:) -> tmpb */
        pv = pTmpA;
        pa = p;
        for(j = 0; j < col - JJ; j++)
        {
            *pdst++ = *pv * *pa++;
        }
        pa += JJ;
        pv++;
        pdst = pTmpB;

        pa0 = pa;
        pa1 = pa0 + col;
        pa2 = pa1 + col;
        pa3 = pa2 + col;

        /* Unrolled loop */
        blkCnt = (row - JJ - 1) >> 2;
        k = 1;
        while(blkCnt > 0)
        {
            float64_t sum;

            for(j = 0; j < col - JJ; j++)
            {
                sum = *pdst;

                sum += pv[0] * *pa0++;
                sum += pv[1] * *pa1++;
                sum += pv[2] * *pa2++;
                sum += pv[3] * *pa3++;

                *pdst++ = sum;
            }
            pa0 += JJ + 3 * col;
            pa1 += JJ + 3 * col;
            pa2 += JJ + 3 * col;
            pa3 += JJ + 3 * col;
            pv  += 4;
            pdst = pTmpB;
            k += 4;
            blkCnt--;
        }

        pa = pa0;
        for(; k < row - JJ; k++)
        {
            for(j = 0; j < col - JJ; j++)
            {
                *pdst++ += *pv * *pa++;
            }
            pa += JJ;
            pv++;
            pdst = pTmpB;
        }

        /* A(JJ:,JJ:) - beta v tmpb */
        pa = p;
        for(j = 0; j < row - JJ; j++)
        {
            float64_t f = beta * pTmpA[j];

            for(i = 0; i < col - JJ; i++)
            {
                *pa = *pa - f * pTmpB[i] ;
                pa++;
            }
            pa += JJ;
        }

        /* Copy Householder reflectors into R matrix */
        pa = p + col;
        for(k = 0; k < row - JJ - 1; k++)
        {
            *pa = pTmpA[k + 1];
            pa += col;
        }

        p += 1 + col;
    }

    /* Generate Q if requested by user matrix */

    if (dstQ != NULL)
    {
        /* Initialize Q matrix to identity */
        memset(dstQ, 0, sizeof(float64_t) * row * row);

        pa = dstQ;
        for(JJ = 0; JJ < row; JJ++)
        {
            *pa = 1.0;
            pa += row + 1;
        }

        nb = row - row + 1;

        ptau = tau + row - 1;
        for(JJ = 0; JJ < row; JJ++)
        {
            uint32_t i, j, k, blkCnt;
            float64_t *pa0, *pa1, *pa2, *pa3;
            pos = row - nb;
            p = dstQ + pos + row * pos ;


            COPY_COL_T(float64_t, dstR, pos, pos, pTmpA);
            pTmpA[0] = 1.0;
            pdst = pTmpB;

            /* v.T A(JJ:,JJ:) -> tmpb */

            pv = pTmpA;
            pa = p;
            for(j = 0; j < row - pos; j++)
            {
                *pdst++ = *pv * *pa++;
            }
            pa += pos;
            pv++;
            pdst = pTmpB;
            pa0 = pa;
            pa1 = pa0 + row;
            pa2 = pa1 + row;
            pa3 = pa2 + row;

            /* Unrolled loop */
            blkCnt = (row - pos - 1) >> 2;
            k = 1;
            while(blkCnt > 0)
            {
                float64_t sum;

                for(j = 0; j < row - pos; j++)
                {
                    sum = *pdst;

                    sum += pv[0] * *pa0++;
                    sum += pv[1] * *pa1++;
                    sum += pv[2] * *pa2++;
                    sum += pv[3] * *pa3++;

                    *pdst++ = sum;
                }
                pa0 += pos + 3 * row;
                pa1 += pos + 3 * row;
                pa2 += pos + 3 * row;
                pa3 += pos + 3 * row;
                pv  += 4;
                pdst = pTmpB;
                k += 4;
                blkCnt--;
            }

            pa = pa0;
            for(; k < row - pos; k++)
            {
                for(j = 0; j < row - pos; j++)
                {
                    *pdst++ += *pv * *pa++;
                }
                pa += pos;
                pv++;
                pdst = pTmpB;
            }

            pa = p;
            beta = *ptau--;
            for(j = 0; j < row - pos; j++)
            {
                float64_t f = beta * pTmpA[j];

                for(i = 0; i < row - pos; i++)
                {
                    *pa = *pa - f * pTmpB[i] ;
                    pa++;
                }
                pa += pos;
            }
            nb++;
        }
    }
    return 0;
}
