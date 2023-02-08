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
#include <math.h> // for logf

/**
 * @ingroup statistics
 */

/**
 * @brief Log-Sum-Exp of the floating-potint vector.
 * @param[in]       *src    points to the input vector.
 * @param[in]       size    size of the vectors.
 * @return lse value.
 *
 */

/* function description */
float32_t riscv_dsp_lse_f32(const float32_t * FUNC_RESTRICT src, uint32_t size)
{
    float32_t max, tmp;
    float32_t acc, out;
    const float32_t *in = src;

    uint32_t cnt = size;
    max = *in++;
    cnt--;
    while (cnt != 0)
    {
        tmp = *in++;
        max = (max < tmp) ? tmp : max;
        cnt--;
    }

    cnt = size;
    in = src;
    acc = 0.0f;
    while (cnt != 0)
    {
        tmp = *in++;
        acc += expf(tmp - max);
        cnt--;
    }
    out = max + logf(acc);
    return out;
}

/**
 * @} end of max.
 */
