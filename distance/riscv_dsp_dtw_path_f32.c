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
#include "internal_math_types.h"
#include "math.h"

#define POINT(MAT,R,C) \
  (*((MAT) + col*(R) + (C)))

void riscv_dsp_dtw_path_f32(const float32_t * FUNC_RESTRICT dtw,
                            int16_t * FUNC_RESTRICT dtw_path,
                            uint32_t * FUNC_RESTRICT dtw_path_length,
                            uint32_t row,
                            uint32_t col)
{
    int q, t;
    float32_t temp;

    *dtw_path_length = 0;
    q = row - 1;
    t = col - 1;
    while((q > 0) || (t > 0))
    {
        int p = -1;
        float32_t current = VEC_F32_MAX;

        if (q > 0)
        {
            temp = POINT(dtw, q - 1, t);
            if (temp < current)
            {
                current = temp;
                p = 2;
            }
        }

        if (t > 0)
        {
            temp = POINT(dtw, q, t - 1);
            if (temp < current)
            {
                current = temp;
                p = 0;
            }
        }

        if ((q > 0) && (t > 0))
        {
            temp = POINT(dtw, q - 1, t - 1);
            if (temp < current)
            {
                current = temp;
                p = 1;
            }
        }

        dtw_path[2 * (*dtw_path_length)] = q;
        dtw_path[2 * (*dtw_path_length) + 1] = t;

        *dtw_path_length = *dtw_path_length + 1;

        switch(p)
        {
        case 0:
            t = t - 1;
            break;
        case 1:
            t = t - 1;
            q = q - 1;
            break;
        case 2:
            q = q - 1;
            break;
        }

    }

    dtw_path[2 * (*dtw_path_length)] = 0;
    dtw_path[2 * (*dtw_path_length) + 1] = 0;
    *dtw_path_length = *dtw_path_length + 1;


    /* Reverse the path */
    int16_t *fh, *sh;
    fh = dtw_path;
    sh = dtw_path + 2 * (*dtw_path_length) - 2;
    int halfLength = (*dtw_path_length) >> 1;
    for(int i = 0; i < halfLength; i++)
    {
        temp = fh[0];
        fh[0] = sh[0];
        sh[0] = temp;

        temp = fh[1];
        fh[1] = sh[1];
        sh[1] = temp;

        fh += 2;
        sh -= 2;
    }
}
