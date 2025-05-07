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
  | Peak sidelobe level                   |           42.7 dB  |
  | Normalized equivalent noise bandwidth |       1.3628 bins  |
  | 3 dB bandwidth                        |       1.3008 bins  |
  | Flatness                              |        -1.7514 dB  |
  | Recommended overlap                   |              50 %  |
  | ------------------------------------: | -----------------: |
 */

/* function description */
FUNC_ATTR_FFAST_MATH
void riscv_dsp_win_hamming_f64(float64_t * FUNC_RESTRICT dst, uint32_t size)
{
    dsp_weighted_sum_of_cos_1(dst, size, 0.54, -0.46);
}
