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
#include <math.h> // for logf

extern float16_t riscv_dsp_lse_f16(const float16_t * src, uint32_t size);
extern void riscv_dsp_add_f16(float16_t * src1, float16_t * src2, float16_t * dst, uint32_t size);

/* function description */
FUNC_ATTR_FFAST_MATH
float16_t riscv_dsp_lse_dprod_f16(const float16_t * FUNC_RESTRICT src1, const float16_t * FUNC_RESTRICT src2, uint32_t size, float16_t * FUNC_RESTRICT buffer)
{
    riscv_dsp_add_f16((float16_t*)src1, (float16_t*)src2, buffer, size);
    float16_t out = riscv_dsp_lse_f16(buffer, size);
    return(out);
}
