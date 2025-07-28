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

extern float64_t riscv_dsp_dprod_f64(float64_t *src1, float64_t *src2, uint32_t size);
extern float64_t riscv_dsp_pwr_f64(const float64_t *src, uint32_t size);
extern float64_t riscv_dsp_sqrt_f64(float64_t src);

/**
 * @brief        Cosine distance between two vectors
 *
 * The input vectors are modified in place !
 *
 * @param[in]    src1         First vector
 * @param[in]    src2         Second vector
 * @param[in]    size         vector length
 * @return distance
 *
 */
float64_t riscv_dsp_dist_cos_f64(const float64_t * FUNC_RESTRICT src1, const float64_t * FUNC_RESTRICT src2, uint32_t size)
{
    float64_t pwr1 = riscv_dsp_pwr_f64((float64_t *)src1, size);
    float64_t pwr2 = riscv_dsp_pwr_f64((float64_t *)src2, size);
    float64_t dot = riscv_dsp_dprod_f64((float64_t *)src1, (float64_t *)src2, size);
    float64_t sq = riscv_dsp_sqrt_f64(pwr1 * pwr2);
    float64_t out = 1.0f - dot / sq;
    return out;
}

/**
 * @} end of groupSVM group
 */
