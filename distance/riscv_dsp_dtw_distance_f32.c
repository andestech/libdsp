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
#include "stdio.h"
#include "internal_math_types.h"
#include "math.h"

#define POINT(MAT,R,C) \
  (*((MAT) + col*(R) + (C)))

#define WINDOW(R,C)                                             \
  ((dtw_window == NULL) ? 1 :                                   \
   ((*(dtw_window + col*(R) + (C)))==1))

int riscv_dsp_dtw_distance_f32(const float32_t * FUNC_RESTRICT dtw_src,
                               const q7_t * FUNC_RESTRICT dtw_window,
                               float32_t * FUNC_RESTRICT dtw_out,
                               float32_t * FUNC_RESTRICT dtw_distance,
                               uint32_t row,
                               uint32_t col)
{
    const uint32_t query_length = row;
    const uint32_t template_length = col;
    float32_t result;

    float32_t *temp = dtw_out;
    for(uint32_t q = 0 ; q < query_length; q++)
    {
        for(uint32_t t = 0; t < template_length; t++)
        {
            *temp++ = VEC_F32_MAX;
        }
    }

    dtw_out[0] = POINT(dtw_src, 0, 0);
    for(uint32_t q = 1; q < query_length; q++)
    {
        if (!WINDOW(q, 0))
        {
            continue;
        }
        POINT(dtw_out, q, 0) = POINT(dtw_out, q - 1, 0) + POINT(dtw_src, q, 0);
    }

    for(uint32_t t = 1; t < template_length; t++)
    {
        if (!WINDOW(0, t))
        {
            continue;
        }
        POINT(dtw_out, 0, t) = POINT(dtw_out, 0, t - 1) + POINT(dtw_src, 0, t);
    }

    for(uint32_t q = 1; q < query_length; q++)
    {
        for(uint32_t t = 1; t < template_length; t++)
        {
            if (!WINDOW(q, t))
            {
                continue;
            }
            POINT(dtw_out, q, t) =
                NDSV_MIN(POINT(dtw_out, q - 1, t - 1) + 2.0f * POINT(dtw_src, q, t),
                         NDSV_MIN(POINT(dtw_out, q, t - 1)   +        POINT(dtw_src, q, t),
                                  POINT(dtw_out, q - 1, t)   +        POINT(dtw_src, q, t)));
        }
    }

    if (POINT(dtw_out, query_length - 1, template_length - 1) == VEC_F32_MAX)
    {
        return -1;
    }

    result = POINT(dtw_out, query_length - 1, template_length - 1);
    result = result / (query_length + template_length);
    *dtw_distance = result;

    return 0;
}
