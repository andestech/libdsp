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
 *   riscv_dsp_cos_table_q15_128[k + 2] = cos(2 * PI * k / 128)
 * #else
 *   riscv_dsp_cos_table_q15_128[k] = cos(2 * PI * k / 128)
 * #endif
 * where 0 <= k <= 32.
 **/

#ifdef HERMITE_INTERPOLATION
const q15_t riscv_dsp_cos_table_q15_128[37] =
{
    0x7f62, 0x7fd9,
    0x7fff, 0x7fd9, 0x7f62, 0x7e9d, 0x7d8a, 0x7c2a, 0x7a7d, 0x7885,
#else
const q15_t riscv_dsp_cos_table_q15_128[33] =
{
    0x7fff, 0x7fd9, 0x7f62, 0x7e9d, 0x7d8a, 0x7c2a, 0x7a7d, 0x7885,
#endif
    0x7642, 0x73b6, 0x70e3, 0x6dca, 0x6a6e, 0x66d0, 0x62f2, 0x5ed7,
    0x5a82, 0x55f6, 0x5134, 0x4c40, 0x471d, 0x41ce, 0x3c57, 0x36ba,
    0x30fc, 0x2b1f, 0x2528, 0x1f1a, 0x18f9, 0x12c8, 0x0c8c, 0x0648,
#ifdef HERMITE_INTERPOLATION
    0x0000, 0xf9b8, 0xf374
#else
    0x0000
#endif
};

const q15_t riscv_dsp_cos_table_q15_linear_128[33] =
{
    0x7fff, 0x7fd9, 0x7f62, 0x7e9d, 0x7d8a, 0x7c2a, 0x7a7d, 0x7885,
    0x7642, 0x73b6, 0x70e3, 0x6dca, 0x6a6e, 0x66d0, 0x62f2, 0x5ed7,
    0x5a82, 0x55f6, 0x5134, 0x4c40, 0x471d, 0x41ce, 0x3c57, 0x36ba,
    0x30fc, 0x2b1f, 0x2528, 0x1f1a, 0x18f9, 0x12c8, 0x0c8c, 0x0648,
    0x0000
};

/***************************************************************************
 * cosine reciprocal table:                                                *
 *   1 / (2 * cos(x)) where 0 <= x <= PI / 2                               *
 *   riscv_dsp_cos_recip_table_q15_128[k] = 1 / (2 * cos(2 * PI * k / 128))  *
 *   where 0 <= k < 32.                                                   *
 * Note: All values are right-shifted 4 (log2(128) - 3) bits.             *
 *       That is, the return values are Q4.11 numbers.                      *
 ***************************************************************************/

const unsigned short riscv_dsp_cos_recip_table_q15_128[32] =
{
    0x0800, 0x0802, 0x080a, 0x0816, 0x0828, 0x083f, 0x085c, 0x087f,
    0x08a9, 0x08da, 0x0912, 0x0954, 0x099f, 0x09f6, 0x0a59, 0x0acc,
    0x0b50, 0x0bea, 0x0c9c, 0x0d6e, 0x0e66, 0x0f90, 0x10f9, 0x12b6,
    0x14e8, 0x17bf, 0x1b8f, 0x20ed, 0x2902, 0x3686, 0x519e, 0xa30a
};

