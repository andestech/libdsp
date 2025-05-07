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
 * cosine look-up table:
 *   cos(x) where 0 <= x <= PI / 2
 * #ifdef HERMITE_INTERPOLATION
 *   riscv_dsp_cos_table_q15_64[k + 2] = cos(2 * PI * k / 64)
 * #else
 *   riscv_dsp_cos_table_q15_64[k] = cos(2 * PI * k / 64)
 * #endif
 * where 0 <= k <= 16.
 **/

#ifdef HERMITE_INTERPOLATION
const q15_t riscv_dsp_cos_table_q15_64[21] =
{
    0x7d8a, 0x7f62,
    0x7fff, 0x7f62, 0x7d8a, 0x7a7d, 0x7642, 0x70e3, 0x6a6e, 0x62f2,
#else
const q15_t riscv_dsp_cos_table_q15_64[17] =
{
    0x7fff, 0x7f62, 0x7d8a, 0x7a7d, 0x7642, 0x70e3, 0x6a6e, 0x62f2,
#endif
    0x5a82, 0x5134, 0x471d, 0x3c57, 0x30fc, 0x2528, 0x18f9, 0x0c8c,
#ifdef HERMITE_INTERPOLATION
    0x0000, 0xf374, 0xe707
#else
    0x0000
#endif
};

const q15_t riscv_dsp_cos_table_q15_linear_64[17] =
{
    0x7fff, 0x7f62, 0x7d8a, 0x7a7d, 0x7642, 0x70e3, 0x6a6e, 0x62f2,
    0x5a82, 0x5134, 0x471d, 0x3c57, 0x30fc, 0x2528, 0x18f9, 0x0c8c,
    0x0000
};

/***************************************************************************
 * cosine reciprocal table:                                                *
 *   1 / (2 * cos(x)) where 0 <= x <= PI / 2                               *
 *   riscv_dsp_cos_recip_table_q15_64[k] = 1 / (2 * cos(2 * PI * k / 64))  *
 *   where 0 <= k < 16.                                                   *
 * Note: All values are right-shifted 3 (log2(64) - 3) bits.             *
 *       That is, the return values are Q3.12 numbers.                      *
 ***************************************************************************/

const unsigned short riscv_dsp_cos_recip_table_q15_64[16] =
{
    0x1000, 0x1014, 0x1050, 0x10b8, 0x1151, 0x1224, 0x133e, 0x14b3,
    0x16a1, 0x1939, 0x1ccd, 0x21f1, 0x29cf, 0x371e, 0x5203, 0xa33d
};

