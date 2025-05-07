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
#include <stdio.h>  //include this is because __ULong defined in math.h when using BSP320.
//BSP400 doesn't need this file.
#include <math.h>
#include "internal_utils_math.h"

/* function description */
float16_t riscv_dsp_rms_f16(const float16_t *src, uint32_t size)
{
    float16_t sum = 0.0f, tmp, dst;
    uint32_t i = size;

    while (i != 0u)
    {
        tmp = *src++;
        sum += tmp * tmp;
        i--;
    }

    dst = sqrtf(sum / (float16_t) size);
    return dst;

}
