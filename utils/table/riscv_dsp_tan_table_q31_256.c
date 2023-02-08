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
 * riscv_dsp_tan_table_q31_256[k] = tan(2 * PI * k / 256)
 * where 0 <= k <= 64.
 */

const q31_t riscv_dsp_tan_table_q31_256[65] =
{
    0x00000000, 0x00032469, 0x000649ca, 0x0009711d,
    0x000c9b5e, 0x000fc98c, 0x0012fcac, 0x001635ca,
    0x001975f6, 0x001cbe4d, 0x00200ff5, 0x00236c20,
    0x0026d410, 0x002a4916, 0x002dcc94, 0x00316003,
    0x003504f3, 0x0038bd0e, 0x003c8a1c, 0x00406e06,
    0x00446adc, 0x004882d8, 0x004cb862, 0x00510e1b,
    0x005586e1, 0x005a25d4, 0x005eee68, 0x0063e466,
    0x00690c01, 0x006e69df, 0x0074032f, 0x0079ddbb,
    0x00800000, 0x0086714f, 0x008d39eb, 0x00946338,
    0x009bf7ec, 0x00a40450, 0x00ac9689, 0x00b5befc,
    0x00bf90c7, 0x00ca2254, 0x00d58e1b, 0x00e1f39c,
    0x00ef789e, 0x00fe4ad6, 0x010ea21d, 0x0120c36c,
    0x013504f3, 0x014bd3d8, 0x0165bc6d, 0x01837627,
    0x01a5f59f, 0x01ce8856, 0x01ff0130, 0x023a0288,
    0x02837fdc, 0x02e1aea3, 0x035ee7ea, 0x040dcbed,
    0x05139b16, 0x06c740f0, 0x0a2d7ff7, 0x145e2456,
    0x7fffffff
};
