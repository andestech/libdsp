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
int riscv_dsp_bilinear_interp_f16(riscv_dsp_bilinear_interp_f16_t * FUNC_RESTRICT instance,
                                  float16_t * FUNC_RESTRICT src_x, float16_t * FUNC_RESTRICT src_y, float16_t * FUNC_RESTRICT dst, uint32_t size)
{
    float16_t out;
    float16_t f00, f01, f10, f11;
    const float16_t *py_tbl = instance->tbl_data;
    int32_t xIndex, yIndex, idx;
    float16_t xdiff, ydiff;
    float16_t b1, b2, b3, b4;
    float16_t in_x, in_y;
    uint32_t rows = instance->row;
    uint32_t cols = instance->col;

    for(uint32_t cnt = 0; cnt < size; cnt++)
    {
        in_x = *src_x++;
        in_y = *src_y++;

        xIndex = (int32_t) in_x;
        yIndex = (int32_t) in_y;

        /* Care taken for table outside boundary */
        /* Returns zero output when values are outside table boundary */
        if (xIndex < 0 || (int32_t)xIndex > (int32_t)(cols - 2) || yIndex < 0 || (int32_t)yIndex > (int32_t)(rows - 2))
        {
            *dst++ = 0;
        }
        else
        {
            /* Calculation of index for two nearest points in X-direction */
            idx = (xIndex ) + (yIndex ) * cols;

            /* Read two nearest points in X-direction */
            f00 = py_tbl[idx];
            f01 = py_tbl[idx + 1];

            /* Calculation of index for two nearest points in Y-direction */
            idx = (xIndex ) + (yIndex + 1) * cols;

            /* Read two nearest points in Y-direction */
            f10 = py_tbl[idx];
            f11 = py_tbl[idx + 1];

            /* Calculation of intermediate values */
            b1 = f00;
            b2 = f01 - f00;
            b3 = f10 - f00;
            b4 = f00 - f01 - f10 + f11;

            /* Calculation of fractional part in X */
            xdiff = in_x - xIndex;

            /* Calculation of fractional part in Y */
            ydiff = in_y - yIndex;

            /* Calculation of bi-linear interpolated output */
            out = b1 + b2 * xdiff + b3 * ydiff + b4 * xdiff * ydiff;

            /* return to application */
            *dst++ = out;
        }
    }
    return 0;
}
