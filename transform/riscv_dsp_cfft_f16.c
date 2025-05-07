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
#include "internal_transform_math.h"

extern int32_t riscv_dsp_cfft_rd2_f16(float16_t *src, uint32_t m);
extern int32_t riscv_dsp_cfft_rd4_f16(float16_t *src, uint32_t m);
extern int32_t riscv_dsp_cifft_rd2_f16(float16_t *src, uint32_t m);
extern int32_t riscv_dsp_cifft_rd4_f16(float16_t *src, uint32_t m);

void riscv_dsp_cfft_f16(float16_t *src, const uint32_t m)
{
    switch (m)
    {
    case 4:
    case 6:
    case 8:
    case 10:
    case 12:
    case 14:
        riscv_dsp_cfft_rd4_f16(src, m);
        break;
    default :
        riscv_dsp_cfft_rd2_f16(src, m);
        break;
    }
}

void riscv_dsp_cifft_f16(float16_t *src, const uint32_t m)
{
    switch (m)
    {
    case 4:
    case 6:
    case 8:
    case 10:
    case 12:
    case 14:
        riscv_dsp_cifft_rd4_f16(src, m);
        break;
    default :
        riscv_dsp_cifft_rd2_f16(src, m);
        break;
    }
}
