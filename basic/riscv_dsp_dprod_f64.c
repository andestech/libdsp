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

/* function description */
float64_t riscv_dsp_dprod_f64(float64_t * FUNC_RESTRICT src1, float64_t * FUNC_RESTRICT src2, uint32_t size)
{
    float64_t sum = 0.0;

    while (size != 0u)
    {
        /* y = x1[0] * x2[0] + x1[1] * x2[1] + x1[2] * x2[2]
         ** ..... + x1[size-1] * x2[size-1] */
        sum += (*src1++) * (*src2++);

        size--;
    }

    return (sum);
}
