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

#include "internal_math_types.h"        //for union types

/**
 * @ingroup matrix
 */

/**
 * @addtogroup matrixtrans
 * @{
 */

/**
 * @brief Transpose the q7 matrices.
 * @param[in]       *src  points to the input matrix.
 * @param[out]      *dst  points to the output matrix.
 * @param[in]       row   number of the matrix rows.
 * @param[in]       col   number of the matrix columns.
 * @return none.
 */

/* function description */
void riscv_dsp_mat_trans_q7(const q7_t * FUNC_RESTRICT src, q7_t * FUNC_RESTRICT dst, uint32_t row, uint32_t col)
{
#ifdef NDS_CPU_64
    u32_t i, j;
    suni_t value;

    u32_t remain_col = col & (SIMD_NUM_Q7 - 1);
    const q7_t *tmp_src2 = src;
    q7_t *tmp_dst2 = dst;

    //check if the input row or col is zero
    if ((row <= 0) || (col <= 0))
    {
        return;
    }

    for (i = 0; i < row; i++)
    {
        const q7_t *tmp_src = tmp_src2;
        q7_t *tmp_dst = tmp_dst2;

        for (j = 0; j < (col / SIMD_NUM_Q7); j++)
        {
            value = *(suni_t*)tmp_src;
            *tmp_dst = value.b8.b0;
            tmp_dst += row;
            *tmp_dst = value.b8.b1;
            tmp_dst += row;
            *tmp_dst = value.b8.b2;
            tmp_dst += row;
            *tmp_dst = value.b8.b3;
            tmp_dst += row;
            *tmp_dst = value.b8.b4;
            tmp_dst += row;
            *tmp_dst = value.b8.b5;
            tmp_dst += row;
            *tmp_dst = value.b8.b6;
            tmp_dst += row;
            *tmp_dst = value.b8.b7;
            tmp_dst += row;

            tmp_src += SIMD_NUM_Q7;
        }

        tmp_src2 += col;
        tmp_dst2++;
    }

    if (remain_col)
    {
        src = src + (col - remain_col);
        dst = dst + (col - remain_col) * row;

        for (i = 0; i < row; i++)
        {
            const q7_t *tmp_src = src;
            q7_t *tmp_dst = dst;

            for (j = 0; j < remain_col; j++)
            {
                *tmp_dst = *tmp_src;
                tmp_src++;
                tmp_dst += row;
            }
            src += col;
            dst++;
        }
    }
#else
    q7_t *C;
    uint32_t i, colcnt, rowcnt;

    i = 0u;
    rowcnt = row;
    //check if the input row or col is zero
    if ((row <= 0) || (col <= 0))
    {
        return;
    }

    do
    {
        C = dst + i;

        colcnt = col;
        do
        {
            *C = *src++;
            C += row;
            colcnt--;
        }
        while (colcnt != 0u);

        i++;
        rowcnt--;
    }
    while (rowcnt != 0u);
#endif
}

/**
 * @} end of matrixtrans
 */
