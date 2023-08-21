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
 * cosine look-up table:
 *   cos(x) where 0 <= x <= PI / 2
 * #ifdef HERMITE_INTERPOLATION
 *   riscv_dsp_cos_table_q31_128[k + 2] = cos(2 * PI * k / 128)
 * #else
 *   riscv_dsp_cos_table_q31_128[k] = cos(2 * PI * k / 128)
 * #endif
 * where 0 <= k <= 32.
 **/

#ifdef HERMITE_INTERPOLATION
const q31_t riscv_dsp_cos_table_q31_128[37] =
{
    0x7f62368f, 0x7fd8878e,
    0x7fffffff, 0x7fd8878e, 0x7f62368f, 0x7e9d55fc,
#else
const q31_t riscv_dsp_cos_table_q31_128[33] =
{
    0x7fffffff, 0x7fd8878e, 0x7f62368f, 0x7e9d55fc,
#endif
    0x7d8a5f40, 0x7c29fbee, 0x7a7d055b, 0x78848414,
    0x7641af3d, 0x73b5ebd1, 0x70e2cbc6, 0x6dca0d14,
    0x6a6d98a4, 0x66cf8120, 0x62f201ac, 0x5ed77c8a,
    0x5a82799a, 0x55f5a4d2, 0x5133cc94, 0x4c3fdff4,
    0x471cece7, 0x41ce1e65, 0x3c56ba70, 0x36ba2014,
    0x30fbc54d, 0x2b1f34eb, 0x25280c5e, 0x1f19f97b,
    0x18f8b83c, 0x12c8106f, 0x0c8bd35e, 0x0647d97c,
#ifdef HERMITE_INTERPOLATION
    0x00000000, 0xf9b82684, 0xf3742ca2
#else
    0x00000000
#endif
};

const q31_t riscv_dsp_cos_table_q31_linear_128[33] =
{
    0x7fffffff, 0x7fd8878e, 0x7f62368f, 0x7e9d55fc,
    0x7d8a5f40, 0x7c29fbee, 0x7a7d055b, 0x78848414,
    0x7641af3d, 0x73b5ebd1, 0x70e2cbc6, 0x6dca0d14,
    0x6a6d98a4, 0x66cf8120, 0x62f201ac, 0x5ed77c8a,
    0x5a82799a, 0x55f5a4d2, 0x5133cc94, 0x4c3fdff4,
    0x471cece7, 0x41ce1e65, 0x3c56ba70, 0x36ba2014,
    0x30fbc54d, 0x2b1f34eb, 0x25280c5e, 0x1f19f97b,
    0x18f8b83c, 0x12c8106f, 0x0c8bd35e, 0x0647d97c,
    0x00000000
};

/***************************************************************************
 * cosine reciprocal table:                                                *
 *   1 / (2 * cos(x)) where 0 <= x <= PI / 2                               *
 *   riscv_dsp_cos_recip_table_q31_128[k] = 1 / (2 * cos(2 * PI * k / 128))  *
 *   where 0 <= k < 32.                                                   *
 * Note: All values are right-shifted 4 (log2(128) - 3) bits.             *
 *       That is, the return values are Q4.27 numbers.                     *
 ***************************************************************************/

const unsigned riscv_dsp_cos_recip_table_q31_128[32] =
{
    0x08000000, 0x0802784a, 0x0809e8ce, 0x081668b8,
    0x08281f69, 0x083f45ae, 0x085c2782, 0x087f2684,
    0x08a8bd3e, 0x08d98379, 0x091233e9, 0x0953b39e,
    0x099f1bd1, 0x09f5c6e2, 0x0a5961cc, 0x0acc03d5,
    0x0b504f33, 0x0be99edd, 0x0c9c4806, 0x0d6df9e5,
    0x0e664d78, 0x0f8fa3b6, 0x10f8892a, 0x12b606a6,
    0x14e7ae91, 0x17bf2366, 0x1b8f24b0, 0x20ecabc8,
    0x2901b3a1, 0x36859068, 0x519e4e04, 0xa30a45f5
};

