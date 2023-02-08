/******************************************************************************
 * Copyright (C) 2010-2023 Arm Limited or its affiliates. All rights reserved.*
 * Copyright (C) 2018-2023 Andes Technology Corporation. All rights reserved. *
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
#include "internal_distance_math.h"
#include "stdio.h"

/**
  @addtogroup BoolDist
  @{
 */
/**
 * @brief        Jaccard distance between two vectors
 * @param[in]    src1         First vector
 * @param[in]    src2         Second vector
 * @param[in]    numofbool    Number of booleans
 * @return distance
 */
float32_t riscv_dsp_bdist_jaccard_u32_f32(const uint32_t * FUNC_RESTRICT src1, const uint32_t * FUNC_RESTRICT src2, uint32_t numofbool)
{
    uint32_t cnt_tt = 0, cnt_tf = 0, cnt_ft = 0;

    _dsp_boolean_dist_TT_TF_FT(src1, src2, numofbool, &cnt_tt, &cnt_tf, &cnt_ft);

    float32_t out = (1.0f * (cnt_tf + cnt_ft) / (cnt_tt + cnt_ft + cnt_tf));
    return out;
}

/**
 * @} end of BoolDist group
 */
