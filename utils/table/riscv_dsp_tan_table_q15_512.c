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
 * riscv_dsp_tan_table_q31_512[k] = tan(2 * PI * k / 512)
 * where 0 <= k <= 128.
 */

const q15_t riscv_dsp_tan_table_q15_512[129] =
{
    0x0000, 0x0001, 0x0002, 0x0002, 0x0003, 0x0004, 0x0005, 0x0006,
    0x0006, 0x0007, 0x0008, 0x0009, 0x0009, 0x000a, 0x000b, 0x000c,
    0x000d, 0x000e, 0x000e, 0x000f, 0x0010, 0x0011, 0x0012, 0x0013,
    0x0013, 0x0014, 0x0015, 0x0016, 0x0017, 0x0018, 0x0019, 0x001a,
    0x001b, 0x001b, 0x001c, 0x001d, 0x001e, 0x001f, 0x0020, 0x0021,
    0x0022, 0x0023, 0x0024, 0x0025, 0x0026, 0x0027, 0x0029, 0x002a,
    0x002b, 0x002c, 0x002d, 0x002e, 0x002f, 0x0031, 0x0032, 0x0033,
    0x0035, 0x0036, 0x0037, 0x0039, 0x003a, 0x003b, 0x003d, 0x003e,
    0x0040, 0x0042, 0x0043, 0x0045, 0x0047, 0x0048, 0x004a, 0x004c,
    0x004e, 0x0050, 0x0052, 0x0054, 0x0056, 0x0059, 0x005b, 0x005d,
    0x0060, 0x0062, 0x0065, 0x0068, 0x006b, 0x006e, 0x0071, 0x0074,
    0x0078, 0x007b, 0x007f, 0x0083, 0x0087, 0x008c, 0x0090, 0x0095,
    0x009b, 0x00a0, 0x00a6, 0x00ac, 0x00b3, 0x00ba, 0x00c2, 0x00ca,
    0x00d3, 0x00dd, 0x00e7, 0x00f3, 0x0100, 0x010d, 0x011d, 0x012e,
    0x0142, 0x0158, 0x0171, 0x018e, 0x01af, 0x01d7, 0x0207, 0x0241,
    0x028a, 0x02e7, 0x0364, 0x0412, 0x0517, 0x06ca, 0x0a2f, 0x145f,
    0x7fff
};

