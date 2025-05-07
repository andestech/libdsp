/******************************************************************************
 * Copyright (C) 2010-2018 Arm Limited or its affiliates. All rights reserved.*
 * Copyright (C) 2018-2021 Andes Technology Corporation. All rights reserved. *
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

/**
  @brief         Floating-point product of two quaternions.
  @param[in]     src1       first quaternion
  @param[in]     src2       second quaternion
  @param[out]    dst       product of two quaternions
  @return        none
 */

void riscv_dsp_quaternion_product_single_f32(const float32_t *src1, const float32_t *src2, float32_t *dst)
{
    dst[0] = src1[0] * src2[0] - src1[1] * src2[1] - src1[2] * src2[2] - src1[3] * src2[3];
    dst[1] = src1[0] * src2[1] + src1[1] * src2[0] + src1[2] * src2[3] - src1[3] * src2[2];
    dst[2] = src1[0] * src2[2] + src1[2] * src2[0] + src1[3] * src2[1] - src1[1] * src2[3];
    dst[3] = src1[0] * src2[3] + src1[3] * src2[0] + src1[1] * src2[2] - src1[2] * src2[1];
}
