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

#include "internal_math_types.h"

/**
 * tangent look-up table:
 * tan(x) where 0 <= x <= PI / 2
 * riscv_dsp_tan_table_q31_64[k] = tan(2 * PI * k / 64)
 * where 0 <= k <= 16.
 */

const q31_t riscv_dsp_tan_table_q31_64[17] =
{
    0x00000000, 0x00326d77, 0x0065d7d8, 0x009b5042,
    0x00d413cd, 0x0111ab72, 0x01561b83, 0x01a43003,
    0x02000000, 0x026fdfb2, 0x02fe431c, 0x03bde277,
    0x04d413cd, 0x0697d67a, 0x0a0dff71, 0x144e6c59,
    0x7fffffff
};
