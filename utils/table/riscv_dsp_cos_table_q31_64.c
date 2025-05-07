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
 *   riscv_dsp_cos_table_q31_64[k + 2] = cos(2 * PI * k / 64)
 * #else
 *   riscv_dsp_cos_table_q31_64[k] = cos(2 * PI * k / 64)
 * #endif
 * where 0 <= k <= 16.
 **/

#ifdef HERMITE_INTERPOLATION
const q31_t riscv_dsp_cos_table_q31_64[21] =
{
    0x7d8a5f40, 0x7f62368f,
    0x7fffffff, 0x7f62368f, 0x7d8a5f40, 0x7a7d055b,
#else
const q31_t riscv_dsp_cos_table_q31_64[17] =
{
    0x7fffffff, 0x7f62368f, 0x7d8a5f40, 0x7a7d055b,
#endif
    0x7641af3d, 0x70e2cbc6, 0x6a6d98a4, 0x62f201ac,
    0x5a82799a, 0x5133cc94, 0x471cece7, 0x3c56ba70,
    0x30fbc54d, 0x25280c5e, 0x18f8b83c, 0x0c8bd35e,
#ifdef HERMITE_INTERPOLATION
    0x00000000, 0xf3742ca2, 0xe70747c4
#else
    0x00000000
#endif
};

const q31_t riscv_dsp_cos_table_q31_linear_64[17] =
{
    0x7fffffff, 0x7f62368f, 0x7d8a5f40, 0x7a7d055b,
    0x7641af3d, 0x70e2cbc6, 0x6a6d98a4, 0x62f201ac,
    0x5a82799a, 0x5133cc94, 0x471cece7, 0x3c56ba70,
    0x30fbc54d, 0x25280c5e, 0x18f8b83c, 0x0c8bd35e,
    0x00000000
};

/***************************************************************************
 * cosine reciprocal table:                                                *
 *   1 / (2 * cos(x)) where 0 <= x <= PI / 2                               *
 *   riscv_dsp_cos_recip_table_q31_64[k] = 1 / (2 * cos(2 * PI * k / 64))  *
 *   where 0 <= k < 16.                                                   *
 * Note: All values are right-shifted 3 (log2(64) - 3) bits.             *
 *       That is, the return values are Q3.28 numbers.                     *
 ***************************************************************************/

const unsigned riscv_dsp_cos_recip_table_q31_64[16] =
{
    0x10000000, 0x1013d19c, 0x10503ed1, 0x10b84f04,
    0x11517a7c, 0x122467d1, 0x133e37a2, 0x14b2c399,
    0x16a09e66, 0x1938900b, 0x1ccc9af0, 0x21f11255,
    0x29cf5d23, 0x371e4961, 0x52036742, 0xa33c9c07
};

