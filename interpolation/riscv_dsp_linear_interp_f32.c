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
int riscv_dsp_linear_interp_f32(riscv_dsp_linear_interp_f32_t * FUNC_RESTRICT instance, float32_t * FUNC_RESTRICT src, float32_t * FUNC_RESTRICT dst, uint32_t size)
{
    float32_t out, in_x;
    float32_t x0, x1, y0, y1;
    uint32_t idx;
    const float32_t *py_tbl = instance->tbl_data;
    float init_x = instance->first_x;
    float xspac = instance->x_spacing;
    float rec_xspac = 1.0 / xspac;
    int32_t table_size = instance->tbl_size;

    for(uint32_t cnt = 0; cnt < size; cnt++)
    {
        in_x = *src++;

        /* Calculation of index */
        idx = (int32_t) ((in_x - init_x) * rec_xspac);

        if (in_x < init_x)
        {
            /* Iniatilize output for below specified range as least output value of table */
            out = py_tbl[0];
        }
        else if ((uint32_t)idx >= (uint32_t)(table_size - 1))
        {
            /* Iniatilize output for above specified range as last output value of table */
            out = py_tbl[table_size - 1];
        }
        else
        {
            /* Calculation of nearest input values */
            x0 = init_x +  idx      * xspac;
            x1 = init_x + (idx + 1) * xspac;

            /* Read of nearest output values */
            y0 = py_tbl[idx];
            y1 = py_tbl[idx + 1];

            /* Calculation of output */
            out = y0 + (in_x - x0) * ((y1 - y0) / (x1 - x0));
        }
        *dst++ = out;
    }
    return 0;
}
