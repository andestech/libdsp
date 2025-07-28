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
 * @brief        Cityblock (Manhattan) distance between two vectors
 * @param[in]    src1         First vector
 * @param[in]    src2         Second vector
 * @param[in]    size         vector length
 * @return distance
 *
 */
float64_t riscv_dsp_dist_city_block_f64(const float64_t * FUNC_RESTRICT src1, const float64_t * FUNC_RESTRICT src2, uint32_t size)
{
    float64_t accsum = 0.0;
    float64_t tmp1, tmp2;

    while (size != 0)
    {
        tmp1 = *src1++;
        tmp2 = *src2++;
        accsum += fabs(tmp1 - tmp2);
        size--;
    }
    return accsum;
}

/**
 * @} end of groupSVM group
 */
