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
#include "internal_svm_math.h"
#include "math.h"

void riscv_dsp_svm_sigmoid_est_f16(
    const riscv_dsp_svm_sigmoid_f16_t * FUNC_RESTRICT instance,
    const float16_t * FUNC_RESTRICT src,
    int32_t * FUNC_RESTRICT result)
{
    float16_t sum = instance->intercept;
    float16_t dot = 0;
    uint32_t i, j;
    const float16_t *pvec = instance->vec;
    uint32_t cntvec = instance->numofvec;
    uint32_t cntdim = instance->dimofvec;
    const float16_t *pcoe = instance->dualcoe;
    float16_t gm = instance->gamma;
    float16_t c0 = instance->coef0;

    for (i = 0; i < cntvec; i++)
    {
        dot = 0;
        for (j = 0; j < cntdim; j++)
        {
            dot = dot + src[j] * *pvec++;
        }
        sum += *pcoe++ * tanhf(gm * dot + c0);
    }
    *result = instance->classes[STEP(sum)];
}

/**
 * @} end of groupSVM group
 */
