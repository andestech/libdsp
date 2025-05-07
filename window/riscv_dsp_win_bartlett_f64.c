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
/*
  | Parameter                             | Value              |
  | ------------------------------------: | -----------------: |
  | Peak sidelobe level                   |           26.5 dB  |
  | Normalized equivalent noise bandwidth |       1.3333 bins  |
  | 3 dB bandwidth                        |       1.2736 bins  |
  | Flatness                              |        -1.8242 dB  |
  | Recommended overlap                   |            50.0 %  |
 */

/* function description */
FUNC_ATTR_FFAST_MATH
void riscv_dsp_win_bartlett_f64(float64_t * FUNC_RESTRICT dst, uint32_t size)
{
    float64_t k = 2.0 / ((float64_t) size);
    float64_t w;

    for(uint32_t i = 0; i < size; i++)
    {
        w = i * k ;
        if (i * k > 1.0f)
        {
            w = 2.0 - w;
        }
        dst[i] = w;
    }
}
