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
#include <math.h> //sqrtf
#include <string.h> //memcpy
#include "internal_nds_types.h"

#define SWAP_COLS_F64(A,i,j)    \
  for(w=0;w < n; w++)           \
  {                          \
     float64_t tmp;          \
     tmp = A[w*n + i];       \
     A[w*n + i] = A[w*n + j];\
     A[w*n + j] = tmp;       \
  }

#define SWAP_ROWS_F64(A,i,j) \
  for(w=0;w < n; w++)        \
  {                          \
     float64_t tmp;          \
     tmp = A[i*n + w];       \
     A[i*n + w] = A[j*n + w];\
     A[j*n + w] = tmp;       \
  }

/* function description */
int riscv_dsp_mat_ldlt_f64(float64_t* FUNC_RESTRICT src, float64_t* FUNC_RESTRICT dstl, float64_t* FUNC_RESTRICT dstd,  uint16_t* FUNC_RESTRICT pp, uint32_t row, uint32_t col)
{
    //check if the input row or col is zero, and should be square matrix.
    if ((row <= 0) || (col <= 0) || (row != col))
    {
        return -1;
    }

    const int n = row;
    int fullRank = 1, diag, k;
    float64_t* pA;
    int row_now, d;

    memcpy(dstl, src, n * n * sizeof(float64_t));
    pA = dstl;

    for (k = 0; k < n; k++)
    {
        pp[k] = k;
    }
    for (k = 0; k < n; k++)
    {
        /* Find pivot */
        float64_t m = VEC_F64_MIN, a;
        int j = k;


        int r;
        int w;

        for (r = k; r < n; r++)
        {
            if (pA[r * n + r] > m)
            {
                m = pA[r * n + r];
                j = r;
            }
        }

        if (j != k)
        {
            SWAP_ROWS_F64(pA, k, j);
            SWAP_COLS_F64(pA, k, j);
        }


        pp[k] = j;

        a = pA[k * n + k];

        if (fabs(a) < 1.0e-8)
        {

            fullRank = 0;
            break;
        }

        for (w = k + 1; w < n; w++)
        {
            int x;
            for (x = k + 1; x < n; x++)
            {
                pA[w * n + x] = pA[w * n + x] - pA[w * n + k] * pA[x * n + k] / a;
            }
        }

        for (w = k + 1; w < n; w++)
        {
            pA[w * n + k] = pA[w * n + k] / a;
        }
    }

    diag = k;
    if (!fullRank)
    {
        diag--;
        for (row_now = 0; row_now < n; row_now++)
        {
            int col_now;
            for (col_now = k; col_now < n; col_now++)
            {
                dstl[row_now * n + col_now] = 0.0;
            }
        }
    }

    for (row_now = 0; row_now < n; row_now++)
    {
        int col_now;
        for (col_now = row_now + 1; col_now < n; col_now++)
        {
            dstl[row_now * n + col_now] = 0.0;
        }
    }
    for (d = 0; d < diag; d++)
    {
        dstd[d * n + d] = dstl[d * n + d];
        dstl[d * n + d] = 1.0;
    }
    return 0;
}
