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

#include "internal_math_types.h"

/**
 * tangent look-up table:
 * tan(x) where 0 <= x <= PI / 2
 * riscv_dsp_tan_table_q31_256[k] = tan(2 * PI * k / 256)
 * where 0 <= k <= 64.
 */

const q15_t riscv_dsp_tan_table_q15_256[65] =
{
    0x0000, 0x0003, 0x0006, 0x0009, 0x000d, 0x0010, 0x0013, 0x0016,
    0x0019, 0x001d, 0x0020, 0x0023, 0x0027, 0x002a, 0x002e, 0x0031,
    0x0035, 0x0039, 0x003d, 0x0040, 0x0044, 0x0049, 0x004d, 0x0051,
    0x0056, 0x005a, 0x005f, 0x0064, 0x0069, 0x006e, 0x0074, 0x007a,
    0x0080, 0x0086, 0x008d, 0x0094, 0x009c, 0x00a4, 0x00ad, 0x00b6,
    0x00c0, 0x00ca, 0x00d6, 0x00e2, 0x00ef, 0x00fe, 0x010f, 0x0121,
    0x0135, 0x014c, 0x0166, 0x0183, 0x01a6, 0x01cf, 0x01ff, 0x023a,
    0x0283, 0x02e2, 0x035f, 0x040e, 0x0514, 0x06c7, 0x0a2d, 0x145e,
    0x7fff
};

