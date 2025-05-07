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
  @brief         Floating-point quaternion conjugates.
  @param[in]     src        points to the input vector of quaternions
  @param[out]    dst        points to the output vector of conjugate quaternions
  @param[in]     size       number of quaternions in each vector
  @return        none
 */

void riscv_dsp_quaternion_conj_f32(const float32_t * FUNC_RESTRICT src, float32_t * FUNC_RESTRICT dst, uint32_t size)
{
    uint32_t i;
    for(i = 0; i < size; i++)
    {

        dst[4 * i + 0] = src[4 * i + 0];
        dst[4 * i + 1] = -src[4 * i + 1];
        dst[4 * i + 2] = -src[4 * i + 2];
        dst[4 * i + 3] = -src[4 * i + 3];
    }
}
