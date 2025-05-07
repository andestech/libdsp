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
#define MM_TYPE float32_t
#define TYPE_PI PI
#define COS_TYPE cosf
#include "internal_window.h"

/**
  @par Parameters of the window
  | Parameter                             | Value              |
  | ------------------------------------: | -----------------: |
  | Peak sidelobe level                   |          196.2 dB  |
  | Normalized equivalent noise bandwidth |       5.1134 bins  |
  | 3 dB bandwidth                        |       5.0308 bins  |
  | Flatness                              |         0.0013 dB  |
  | Recommended overlap                   |            82.3 %  |
  | ------------------------------------: | -----------------: |
 */

/* function description */
void riscv_dsp_win_hft196d_f32(float32_t * FUNC_RESTRICT dst, uint32_t size)
{
    dsp_weighted_sum_of_cos_8(dst, size, 1.0f, -1.979280420f, 1.710288951f, -1.081629853f, 0.448734314f, -0.112376628f, 0.015122992f, -0.000871252f, 0.000011896f);
}
