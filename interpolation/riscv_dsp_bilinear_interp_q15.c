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
#include "riscv_dsp_interp.h"

/* function description */
int riscv_dsp_bilinear_interp_q15(riscv_dsp_bilinear_interp_q15_t * FUNC_RESTRICT instance,
                                  q31_t * FUNC_RESTRICT src_x, q31_t * FUNC_RESTRICT src_y, q15_t * FUNC_RESTRICT dst, uint32_t size)
{
    q31_t out;                                   /* Temporary output */
    q63_t acc = 0;                               /* output */
    q31_t xfract, yfract;                        /* X, Y fractional parts */
    q15_t x1, x2, y1, y2;                        /* Nearest output values */
    int32_t rI, cI;                              /* Row and column indices */
    const q15_t *py_tbl = instance->tbl_data;
    int32_t Rows = instance->row;
    int32_t Cols = instance->col;
    q31_t in_x, in_y;

    for(uint32_t cnt = 0; cnt < size; cnt++)
    {
        in_x = *src_x++;
        in_y = *src_y++;
        /* Input is in 12.20 format */
        /* 12 bits for the table index */
        /* Index value calculation */
        rI = ((in_x & (q31_t)0xFFF00000) >> 20);

        /* Input is in 12.20 format */
        /* 12 bits for the table index */
        /* Index value calculation */
        cI = ((in_y & (q31_t)0xFFF00000) >> 20);

        /* Care taken for table outside boundary */
        /* Returns zero output when values are outside table boundary */
        if (rI < 0 || rI > (Cols - 2) || cI < 0 || cI > (Rows - 2))
        {
            *dst++ = 0;
        }
        else
        {

            /* 20 bits for the fractional part */
            /* shift left xfract by 11 to keep 1.31 format */
            xfract = (in_x & 0x000FFFFF);

            /* Read two nearest output values from the index */
            x1 = py_tbl[(rI) + (int32_t)Cols * (cI)    ];
            x2 = py_tbl[(rI) + (int32_t)Cols * (cI) + 1];

            /* 20 bits for the fractional part */
            /* yfract should be in 12.20 format */
            yfract = (in_y & 0x000FFFFF);

            /* Read two nearest output values from the index */
            y1 = py_tbl[(rI) + (int32_t)Cols * (cI + 1)    ];
            y2 = py_tbl[(rI) + (int32_t)Cols * (cI + 1) + 1];

            /* x1 is in 1.15(q15), xfract in 12.20 format and out is in 13.35 format */
            /* convert 13.35 to 13.31 by right shifting  and out is in 1.31 */
            out = (q31_t) (((q63_t) x1 * (0x0FFFFF - xfract)) >> 4U);
            acc = ((q63_t) out * (0x0FFFFF - yfract));

            /* x2 * (xfract) * (1-yfract)  in 1.51 and adding to acc */
            out = (q31_t) (((q63_t) x2 * (0x0FFFFF - yfract)) >> 4U);
            acc += ((q63_t) out * (xfract));

            /* y1 * (1 - xfract) * (yfract)  in 1.51 and adding to acc */
            out = (q31_t) (((q63_t) y1 * (0x0FFFFF - xfract)) >> 4U);
            acc += ((q63_t) out * (yfract));

            /* y2 * (xfract) * (yfract)  in 1.51 and adding to acc */
            out = (q31_t) (((q63_t) y2 * (xfract)) >> 4U);
            acc += ((q63_t) out * (yfract));

            /* acc is in 13.51 format and down shift acc by 36 times */
            /* Convert out to 1.15 format */
            *dst++ = ((q15_t)(acc >> 36));
        }
    }
    return 0;
}
