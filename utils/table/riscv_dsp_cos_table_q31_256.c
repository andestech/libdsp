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
 *   riscv_dsp_cos_table_q31_256[k + 2] = cos(2 * PI * k / 256)
 * #else
 *   riscv_dsp_cos_table_q31_256[k] = cos(2 * PI * k / 256)
 * #endif
 * where 0 <= k <= 64.
 **/

#ifdef HERMITE_INTERPOLATION
const q31_t riscv_dsp_cos_table_q31_256[69] =
{
    0x7fd8878e, 0x7ff62182,
    0x7fffffff, 0x7ff62182, 0x7fd8878e, 0x7fa736b4,
#else
const q31_t riscv_dsp_cos_table_q31_256[65] =
{
    0x7fffffff, 0x7ff62182, 0x7fd8878e, 0x7fa736b4,
#endif
    0x7f62368f, 0x7f0991c4, 0x7e9d55fc, 0x7e1d93ea,
    0x7d8a5f40, 0x7ce3ceb2, 0x7c29fbee, 0x7b5d039e,
    0x7a7d055b, 0x798a23b1, 0x78848414, 0x776c4edb,
    0x7641af3d, 0x7504d345, 0x73b5ebd1, 0x72552c85,
    0x70e2cbc6, 0x6f5f02b2, 0x6dca0d14, 0x6c242960,
    0x6a6d98a4, 0x68a69e81, 0x66cf8120, 0x64e88926,
    0x62f201ac, 0x60ec3830, 0x5ed77c8a, 0x5cb420e0,
    0x5a82799a, 0x5842dd54, 0x55f5a4d2, 0x539b2af0,
    0x5133cc94, 0x4ebfe8a5, 0x4c3fdff4, 0x49b41533,
    0x471cece7, 0x447acd50, 0x41ce1e65, 0x3f1749b8,
    0x3c56ba70, 0x398cdd32, 0x36ba2014, 0x33def287,
    0x30fbc54d, 0x2e110a62, 0x2b1f34eb, 0x2826b928,
    0x25280c5e, 0x2223a4c5, 0x1f19f97b, 0x1c0b826a,
    0x18f8b83c, 0x15e21445, 0x12c8106f, 0x0fab272b,
    0x0c8bd35e, 0x096a9049, 0x0647d97c, 0x03242abf,
#ifdef HERMITE_INTERPOLATION
    0x00000000, 0xfcdbd541, 0xf9b82684
#else
    0x00000000
#endif
};

const q31_t riscv_dsp_cos_table_q31_linear_256[65] =
{
    0x7fffffff, 0x7ff62182, 0x7fd8878e, 0x7fa736b4,
    0x7f62368f, 0x7f0991c4, 0x7e9d55fc, 0x7e1d93ea,
    0x7d8a5f40, 0x7ce3ceb2, 0x7c29fbee, 0x7b5d039e,
    0x7a7d055b, 0x798a23b1, 0x78848414, 0x776c4edb,
    0x7641af3d, 0x7504d345, 0x73b5ebd1, 0x72552c85,
    0x70e2cbc6, 0x6f5f02b2, 0x6dca0d14, 0x6c242960,
    0x6a6d98a4, 0x68a69e81, 0x66cf8120, 0x64e88926,
    0x62f201ac, 0x60ec3830, 0x5ed77c8a, 0x5cb420e0,
    0x5a82799a, 0x5842dd54, 0x55f5a4d2, 0x539b2af0,
    0x5133cc94, 0x4ebfe8a5, 0x4c3fdff4, 0x49b41533,
    0x471cece7, 0x447acd50, 0x41ce1e65, 0x3f1749b8,
    0x3c56ba70, 0x398cdd32, 0x36ba2014, 0x33def287,
    0x30fbc54d, 0x2e110a62, 0x2b1f34eb, 0x2826b928,
    0x25280c5e, 0x2223a4c5, 0x1f19f97b, 0x1c0b826a,
    0x18f8b83c, 0x15e21445, 0x12c8106f, 0x0fab272b,
    0x0c8bd35e, 0x096a9049, 0x0647d97c, 0x03242abf,
    0x00000000
};

/***************************************************************************
 * cosine reciprocal table:                                                *
 *   1 / (2 * cos(x)) where 0 <= x <= PI / 2                               *
 *   riscv_dsp_cos_recip_table_q31_256[k] = 1 / (2 * cos(2 * PI * k / 256))  *
 *   where 0 <= k < 64.                                                   *
 * Note: All values are right-shifted 5 (log2(256) - 3) bits.             *
 *       That is, the return values are Q5.26 numbers.                     *
 ***************************************************************************/

const unsigned riscv_dsp_cos_recip_table_q31_256[64] =
{
    0x04000000, 0x04004efa, 0x04013c25, 0x0402c838,
    0x0404f467, 0x0407c262, 0x040b345c, 0x040f4d0c,
    0x04140fb4, 0x04198029, 0x041fa2d7, 0x04267ccb,
    0x042e13c1, 0x04366e2b, 0x043f9342, 0x04498b16,
    0x04545e9f, 0x046017d3, 0x046cc1bc, 0x047a6898,
    0x048919f4, 0x0498e4d1, 0x04a9d9cf, 0x04bc0b5b,
    0x04cf8de8, 0x04e4782e, 0x04fae371, 0x0512ebdd,
    0x052cb0e6, 0x054855c0, 0x056601ea, 0x0585e1d3,
    0x05a8279a, 0x05cd0bfd, 0x05f4cf6f, 0x061fbb68,
    0x064e2403, 0x068069f4, 0x06b6fcf2, 0x06f25eb7,
    0x073326bc, 0x077a06f7, 0x07c7d1db, 0x081d8215,
    0x087c4495, 0x08e585ce, 0x095b0353, 0x09dee3b3,
    0x0a73d749, 0x0b1d4627, 0x0bdf91b3, 0x0cc0748a,
    0x0dc79258, 0x0eff561f, 0x107655e4, 0x1241a377,
    0x1480d9d0, 0x1765a397, 0x1b42c834, 0x20ad4893,
    0x28cf2702, 0x365fbed6, 0x518522fb, 0xa2fdb3d6
};

