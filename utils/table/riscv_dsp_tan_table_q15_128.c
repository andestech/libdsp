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
 * riscv_dsp_tan_table_q31_128[k] = tan(2 * PI * k / 128)
 * where 0 <= k <= 32.
 */

const q15_t riscv_dsp_tan_table_q15_128[33] =
{
    0x0000, 0x000d, 0x0019, 0x0026, 0x0033, 0x0040, 0x004e, 0x005c,
    0x006a, 0x0079, 0x0089, 0x0099, 0x00ab, 0x00be, 0x00d2, 0x00e8,
    0x0100, 0x011a, 0x0138, 0x0159, 0x017f, 0x01ab, 0x01df, 0x021d,
    0x026a, 0x02cb, 0x034c, 0x03fe, 0x0507, 0x06be, 0x0a27, 0x145b,
    0x7fff
};

