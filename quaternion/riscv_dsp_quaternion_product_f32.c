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
#include "riscv_dsp_quaternion.h"

/**
  @brief         Floating-point elementwise product two quaternions.
  @param[in]     src1                  first array of quaternions
  @param[in]     src2                  second array of quaternions
  @param[out]    dst                   elementwise product of quaternions
  @param[in]     size       number of quaternions in the array
  @return        none
 */

void riscv_dsp_quaternion_product_f32(const float32_t *src1, const float32_t *src2, float32_t *dst, uint32_t size)
{
    uint32_t i;
    for(i = 0; i < size; i++)
    {
        riscv_dsp_quaternion_product_single_f32(src1, src2, dst);
        src1 += 4;
        src2 += 4;
        dst += 4;
    }
}
