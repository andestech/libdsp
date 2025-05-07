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
 *   riscv_dsp_cos_table_q15_256[k + 2] = cos(2 * PI * k / 256)
 * #else
 *   riscv_dsp_cos_table_q15_256[k] = cos(2 * PI * k / 256)
 * #endif
 * where 0 <= k <= 64.
 **/

#ifdef HERMITE_INTERPOLATION
const q15_t riscv_dsp_cos_table_q15_256[69] =
{
    0x7fd9, 0x7ff6,
    0x7fff, 0x7ff6, 0x7fd9, 0x7fa7, 0x7f62, 0x7f0a, 0x7e9d, 0x7e1e,
#else
const q15_t riscv_dsp_cos_table_q15_256[65] =
{
    0x7fff, 0x7ff6, 0x7fd9, 0x7fa7, 0x7f62, 0x7f0a, 0x7e9d, 0x7e1e,
#endif
    0x7d8a, 0x7ce4, 0x7c2a, 0x7b5d, 0x7a7d, 0x798a, 0x7885, 0x776c,
    0x7642, 0x7505, 0x73b6, 0x7255, 0x70e3, 0x6f5f, 0x6dca, 0x6c24,
    0x6a6e, 0x68a7, 0x66d0, 0x64e9, 0x62f2, 0x60ec, 0x5ed7, 0x5cb4,
    0x5a82, 0x5843, 0x55f6, 0x539b, 0x5134, 0x4ec0, 0x4c40, 0x49b4,
    0x471d, 0x447b, 0x41ce, 0x3f17, 0x3c57, 0x398d, 0x36ba, 0x33df,
    0x30fc, 0x2e11, 0x2b1f, 0x2827, 0x2528, 0x2224, 0x1f1a, 0x1c0c,
    0x18f9, 0x15e2, 0x12c8, 0x0fab, 0x0c8c, 0x096b, 0x0648, 0x0324,
#ifdef HERMITE_INTERPOLATION
    0x0000, 0xfcdc, 0xf9b8
#else
    0x0000
#endif
};

const q15_t riscv_dsp_cos_table_q15_linear_256[65] =
{
    0x7fff, 0x7ff6, 0x7fd9, 0x7fa7, 0x7f62, 0x7f0a, 0x7e9d, 0x7e1e,
    0x7d8a, 0x7ce4, 0x7c2a, 0x7b5d, 0x7a7d, 0x798a, 0x7885, 0x776c,
    0x7642, 0x7505, 0x73b6, 0x7255, 0x70e3, 0x6f5f, 0x6dca, 0x6c24,
    0x6a6e, 0x68a7, 0x66d0, 0x64e9, 0x62f2, 0x60ec, 0x5ed7, 0x5cb4,
    0x5a82, 0x5843, 0x55f6, 0x539b, 0x5134, 0x4ec0, 0x4c40, 0x49b4,
    0x471d, 0x447b, 0x41ce, 0x3f17, 0x3c57, 0x398d, 0x36ba, 0x33df,
    0x30fc, 0x2e11, 0x2b1f, 0x2827, 0x2528, 0x2224, 0x1f1a, 0x1c0c,
    0x18f9, 0x15e2, 0x12c8, 0x0fab, 0x0c8c, 0x096b, 0x0648, 0x0324,
    0x0000
};

/***************************************************************************
 * cosine reciprocal table:                                                *
 *   1 / (2 * cos(x)) where 0 <= x <= PI / 2                               *
 *   riscv_dsp_cos_recip_table_q15_256[k] = 1 / (2 * cos(2 * PI * k / 256))  *
 *   where 0 <= k < 64.                                                   *
 * Note: All values are right-shifted 5 (log2(256) - 3) bits.             *
 *       That is, the return values are Q5.10 numbers.                      *
 ***************************************************************************/

const unsigned short riscv_dsp_cos_recip_table_q15_256[64] =
{
    0x0400, 0x0400, 0x0401, 0x0403, 0x0405, 0x0408, 0x040b, 0x040f,
    0x0414, 0x041a, 0x0420, 0x0426, 0x042e, 0x0436, 0x0440, 0x044a,
    0x0454, 0x0460, 0x046d, 0x047a, 0x0489, 0x0499, 0x04aa, 0x04bc,
    0x04d0, 0x04e4, 0x04fb, 0x0513, 0x052d, 0x0548, 0x0566, 0x0586,
    0x05a8, 0x05cd, 0x05f5, 0x0620, 0x064e, 0x0680, 0x06b7, 0x06f2,
    0x0733, 0x077a, 0x07c8, 0x081e, 0x087c, 0x08e6, 0x095b, 0x09df,
    0x0a74, 0x0b1d, 0x0be0, 0x0cc0, 0x0dc8, 0x0eff, 0x1076, 0x1242,
    0x1481, 0x1766, 0x1b43, 0x20ad, 0x28cf, 0x3660, 0x5185, 0xa2fe
};

