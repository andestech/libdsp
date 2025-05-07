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
#include <math.h>

/**
 * @brief        Canberra distance between two vectors
 *
 * This function may divide by zero when samples src1[i] and src2[i] are both zero.
 * The result of the computation will be correct. So the division per zero may be
 * ignored.
 *
 * @param[in]    src1         First vector
 * @param[in]    src2         Second vector
 * @param[in]    size         vector length
 * @return distance
 *
 */
float32_t riscv_dsp_dist_canberra_f32(const float32_t * FUNC_RESTRICT src1, const float32_t * FUNC_RESTRICT src2, uint32_t size)
{
    float32_t tmp1, tmp2, out = 0.0f, diff, sum;

    while (size != 0)
    {
        tmp1 = *src1++;
        tmp2 = *src2++;
        diff = fabsf(tmp1 - tmp2);
        sum = fabsf(tmp1) + fabsf(tmp2);
        if ((tmp1 != 0.0f) || (tmp2 != 0.0f))
        {
            out += (diff / sum);
        }
        size--;
    }
    return out;
}

/**
 * @} end of groupSVM group
 */
