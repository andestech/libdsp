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
#define MM_TYPE float64_t
#define TYPE_PI PI_F64
#define COS_TYPE cos
#include "internal_window.h"

/**
  @par Parameters of the window
  | Parameter                             | Value              |
  | ------------------------------------: | -----------------: |
  | Peak sidelobe level                   |          248.4 dB  |
  | Normalized equivalent noise bandwidth |       5.6512 bins  |
  | 3 dB bandwidth                        |       5.5567 bins  |
  | Flatness                              |         0.0009 dB  |
  | Recommended overlap                   |            84.1 %  |
  | ------------------------------------: | -----------------: |
 */

/* function description */
void riscv_dsp_win_hft248d_f64(float64_t * FUNC_RESTRICT dst, uint32_t size)
{
    dsp_weighted_sum_of_cos_10(dst, size, 1.0, -1.985844164102, 1.791176438506, -1.282075284005, 0.667777530266, -0.240160796576, 0.056656381764, -0.008134974479, 0.000624544650, -0.000019808998, 0.000000132974f);
}
