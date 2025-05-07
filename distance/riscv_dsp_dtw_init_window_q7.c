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
#include <stdlib.h>
#include <math.h>
#include "riscv_dsp_distance_math.h"

int32_t riscv_dsp_dtw_init_window_q7(const riscv_dsp_dtw_window_type window_type,
                                     const int32_t window_size,
                                     q7_t * FUNC_RESTRICT dtw_window,
                                     uint32_t row,
                                     uint32_t col)
{
    const int32_t query_length = row;
    const int32_t template_length = col;

    switch(window_type)
    {
    case RISCV_DSP_DTW_SAKOE_CHIBA_WINDOW:
    {
        for(int32_t q = 0; q < query_length; q++)
        {
            for(int32_t t = 0; t < template_length; t++)
            {
                dtw_window[template_length * q + t] = (q7_t)(abs(q - t) <= window_size);
            }
        }
    }
    break;
    case RISCV_DSP_DTW_SLANTED_BAND_WINDOW:
    {
        for(int32_t q = 0; q < query_length; q++)
        {
            for(int32_t t = 0; t < template_length; t++)
            {
                float32_t diag = (1.0f * q * template_length / query_length);
                dtw_window[template_length * q + t] = (q7_t)(fabsf((float32_t)t - diag) <= (float32_t)window_size);
            }
        }
    }
    break;

    default:
        return -1;
    }

    return 0;
}
