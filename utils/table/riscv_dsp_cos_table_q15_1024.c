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
 *   riscv_dsp_cos_table_q15_1024[k + 2] = cos(2 * PI * k / 1024)
 * #else
 *   riscv_dsp_cos_table_q15_1024[k] = cos(2 * PI * k / 1024)
 * #endif
 * where 0 <= k <= 256.
 **/

#ifdef HERMITE_INTERPOLATION
const q15_t riscv_dsp_cos_table_q15_1024[261] =
{
    0x7ffe, 0x7fff,
    0x7fff, 0x7fff, 0x7ffe, 0x7ffa, 0x7ff6, 0x7ff1, 0x7fea, 0x7fe2,
#else
const q15_t riscv_dsp_cos_table_q15_1024[257] =
{
    0x7fff, 0x7fff, 0x7ffe, 0x7ffa, 0x7ff6, 0x7ff1, 0x7fea, 0x7fe2,
#endif
    0x7fd9, 0x7fce, 0x7fc2, 0x7fb5, 0x7fa7, 0x7f98, 0x7f87, 0x7f75,
    0x7f62, 0x7f4e, 0x7f38, 0x7f22, 0x7f0a, 0x7ef0, 0x7ed6, 0x7eba,
    0x7e9d, 0x7e7f, 0x7e60, 0x7e3f, 0x7e1e, 0x7dfb, 0x7dd6, 0x7db1,
    0x7d8a, 0x7d63, 0x7d3a, 0x7d0f, 0x7ce4, 0x7cb7, 0x7c89, 0x7c5a,
    0x7c2a, 0x7bf9, 0x7bc6, 0x7b92, 0x7b5d, 0x7b27, 0x7aef, 0x7ab7,
    0x7a7d, 0x7a42, 0x7a06, 0x79c9, 0x798a, 0x794a, 0x790a, 0x78c8,
    0x7885, 0x7840, 0x77fb, 0x77b4, 0x776c, 0x7723, 0x76d9, 0x768e,
    0x7642, 0x75f4, 0x75a6, 0x7556, 0x7505, 0x74b3, 0x7460, 0x740b,
    0x73b6, 0x735f, 0x7308, 0x72af, 0x7255, 0x71fa, 0x719e, 0x7141,
    0x70e3, 0x7083, 0x7023, 0x6fc2, 0x6f5f, 0x6efb, 0x6e97, 0x6e31,
    0x6dca, 0x6d62, 0x6cf9, 0x6c8f, 0x6c24, 0x6bb8, 0x6b4b, 0x6add,
    0x6a6e, 0x69fd, 0x698c, 0x691a, 0x68a7, 0x6832, 0x67bd, 0x6747,
    0x66d0, 0x6657, 0x65de, 0x6564, 0x64e9, 0x646c, 0x63ef, 0x6371,
    0x62f2, 0x6272, 0x61f1, 0x616f, 0x60ec, 0x6068, 0x5fe4, 0x5f5e,
    0x5ed7, 0x5e50, 0x5dc8, 0x5d3e, 0x5cb4, 0x5c29, 0x5b9d, 0x5b10,
    0x5a82, 0x59f4, 0x5964, 0x58d4, 0x5843, 0x57b1, 0x571e, 0x568a,
    0x55f6, 0x5560, 0x54ca, 0x5433, 0x539b, 0x5303, 0x5269, 0x51cf,
    0x5134, 0x5098, 0x4ffb, 0x4f5e, 0x4ec0, 0x4e21, 0x4d81, 0x4ce1,
    0x4c40, 0x4b9e, 0x4afb, 0x4a58, 0x49b4, 0x490f, 0x486a, 0x47c4,
    0x471d, 0x4675, 0x45cd, 0x4524, 0x447b, 0x43d1, 0x4326, 0x427a,
    0x41ce, 0x4121, 0x4074, 0x3fc6, 0x3f17, 0x3e68, 0x3db8, 0x3d08,
    0x3c57, 0x3ba5, 0x3af3, 0x3a40, 0x398d, 0x38d9, 0x3825, 0x3770,
    0x36ba, 0x3604, 0x354e, 0x3497, 0x33df, 0x3327, 0x326e, 0x31b5,
    0x30fc, 0x3042, 0x2f87, 0x2ecc, 0x2e11, 0x2d55, 0x2c99, 0x2bdc,
    0x2b1f, 0x2a62, 0x29a4, 0x28e5, 0x2827, 0x2768, 0x26a8, 0x25e8,
    0x2528, 0x2467, 0x23a7, 0x22e5, 0x2224, 0x2162, 0x209f, 0x1fdd,
    0x1f1a, 0x1e57, 0x1d93, 0x1cd0, 0x1c0c, 0x1b47, 0x1a83, 0x19be,
    0x18f9, 0x1833, 0x176e, 0x16a8, 0x15e2, 0x151c, 0x1455, 0x138f,
    0x12c8, 0x1201, 0x113a, 0x1073, 0x0fab, 0x0ee4, 0x0e1c, 0x0d54,
    0x0c8c, 0x0bc4, 0x0afb, 0x0a33, 0x096b, 0x08a2, 0x07d9, 0x0711,
    0x0648, 0x057f, 0x04b6, 0x03ed, 0x0324, 0x025b, 0x0192, 0x00c9,
#ifdef HERMITE_INTERPOLATION
    0x0000, 0xff37, 0xfe6e
#else
    0x0000
#endif
};

const q15_t riscv_dsp_cos_table_q15_linear_1024[257] =
{
    0x7fff, 0x7fff, 0x7ffe, 0x7ffa, 0x7ff6, 0x7ff1, 0x7fea, 0x7fe2,
    0x7fd9, 0x7fce, 0x7fc2, 0x7fb5, 0x7fa7, 0x7f98, 0x7f87, 0x7f75,
    0x7f62, 0x7f4e, 0x7f38, 0x7f22, 0x7f0a, 0x7ef0, 0x7ed6, 0x7eba,
    0x7e9d, 0x7e7f, 0x7e60, 0x7e3f, 0x7e1e, 0x7dfb, 0x7dd6, 0x7db1,
    0x7d8a, 0x7d63, 0x7d3a, 0x7d0f, 0x7ce4, 0x7cb7, 0x7c89, 0x7c5a,
    0x7c2a, 0x7bf9, 0x7bc6, 0x7b92, 0x7b5d, 0x7b27, 0x7aef, 0x7ab7,
    0x7a7d, 0x7a42, 0x7a06, 0x79c9, 0x798a, 0x794a, 0x790a, 0x78c8,
    0x7885, 0x7840, 0x77fb, 0x77b4, 0x776c, 0x7723, 0x76d9, 0x768e,
    0x7642, 0x75f4, 0x75a6, 0x7556, 0x7505, 0x74b3, 0x7460, 0x740b,
    0x73b6, 0x735f, 0x7308, 0x72af, 0x7255, 0x71fa, 0x719e, 0x7141,
    0x70e3, 0x7083, 0x7023, 0x6fc2, 0x6f5f, 0x6efb, 0x6e97, 0x6e31,
    0x6dca, 0x6d62, 0x6cf9, 0x6c8f, 0x6c24, 0x6bb8, 0x6b4b, 0x6add,
    0x6a6e, 0x69fd, 0x698c, 0x691a, 0x68a7, 0x6832, 0x67bd, 0x6747,
    0x66d0, 0x6657, 0x65de, 0x6564, 0x64e9, 0x646c, 0x63ef, 0x6371,
    0x62f2, 0x6272, 0x61f1, 0x616f, 0x60ec, 0x6068, 0x5fe4, 0x5f5e,
    0x5ed7, 0x5e50, 0x5dc8, 0x5d3e, 0x5cb4, 0x5c29, 0x5b9d, 0x5b10,
    0x5a82, 0x59f4, 0x5964, 0x58d4, 0x5843, 0x57b1, 0x571e, 0x568a,
    0x55f6, 0x5560, 0x54ca, 0x5433, 0x539b, 0x5303, 0x5269, 0x51cf,
    0x5134, 0x5098, 0x4ffb, 0x4f5e, 0x4ec0, 0x4e21, 0x4d81, 0x4ce1,
    0x4c40, 0x4b9e, 0x4afb, 0x4a58, 0x49b4, 0x490f, 0x486a, 0x47c4,
    0x471d, 0x4675, 0x45cd, 0x4524, 0x447b, 0x43d1, 0x4326, 0x427a,
    0x41ce, 0x4121, 0x4074, 0x3fc6, 0x3f17, 0x3e68, 0x3db8, 0x3d08,
    0x3c57, 0x3ba5, 0x3af3, 0x3a40, 0x398d, 0x38d9, 0x3825, 0x3770,
    0x36ba, 0x3604, 0x354e, 0x3497, 0x33df, 0x3327, 0x326e, 0x31b5,
    0x30fc, 0x3042, 0x2f87, 0x2ecc, 0x2e11, 0x2d55, 0x2c99, 0x2bdc,
    0x2b1f, 0x2a62, 0x29a4, 0x28e5, 0x2827, 0x2768, 0x26a8, 0x25e8,
    0x2528, 0x2467, 0x23a7, 0x22e5, 0x2224, 0x2162, 0x209f, 0x1fdd,
    0x1f1a, 0x1e57, 0x1d93, 0x1cd0, 0x1c0c, 0x1b47, 0x1a83, 0x19be,
    0x18f9, 0x1833, 0x176e, 0x16a8, 0x15e2, 0x151c, 0x1455, 0x138f,
    0x12c8, 0x1201, 0x113a, 0x1073, 0x0fab, 0x0ee4, 0x0e1c, 0x0d54,
    0x0c8c, 0x0bc4, 0x0afb, 0x0a33, 0x096b, 0x08a2, 0x07d9, 0x0711,
    0x0648, 0x057f, 0x04b6, 0x03ed, 0x0324, 0x025b, 0x0192, 0x00c9,
    0x0000
};

/***************************************************************************
 * cosine reciprocal table:                                                *
 *   1 / (2 * cos(x)) where 0 <= x <= PI / 2                               *
 *   riscv_dsp_cos_recip_table_q15_1024[k] = 1 / (2 * cos(2 * PI * k / 1024))  *
 *   where 0 <= k < 256.                                                   *
 * Note: All values are right-shifted 7 (log2(1024) - 3) bits.             *
 *       That is, the return values are Q7.8 numbers.                      *
 ***************************************************************************/

const unsigned short riscv_dsp_cos_recip_table_q15_1024[256] =
{
    0x0100, 0x0100, 0x0100, 0x0100, 0x0100, 0x0100, 0x0100, 0x0100,
    0x0100, 0x0100, 0x0100, 0x0101, 0x0101, 0x0101, 0x0101, 0x0101,
    0x0101, 0x0101, 0x0102, 0x0102, 0x0102, 0x0102, 0x0102, 0x0103,
    0x0103, 0x0103, 0x0103, 0x0104, 0x0104, 0x0104, 0x0104, 0x0105,
    0x0105, 0x0105, 0x0106, 0x0106, 0x0106, 0x0107, 0x0107, 0x0108,
    0x0108, 0x0108, 0x0109, 0x0109, 0x010a, 0x010a, 0x010b, 0x010b,
    0x010c, 0x010c, 0x010d, 0x010d, 0x010e, 0x010e, 0x010f, 0x010f,
    0x0110, 0x0110, 0x0111, 0x0112, 0x0112, 0x0113, 0x0114, 0x0114,
    0x0115, 0x0116, 0x0117, 0x0117, 0x0118, 0x0119, 0x011a, 0x011a,
    0x011b, 0x011c, 0x011d, 0x011e, 0x011f, 0x011f, 0x0120, 0x0121,
    0x0122, 0x0123, 0x0124, 0x0125, 0x0126, 0x0127, 0x0128, 0x0129,
    0x012a, 0x012c, 0x012d, 0x012e, 0x012f, 0x0130, 0x0131, 0x0133,
    0x0134, 0x0135, 0x0136, 0x0138, 0x0139, 0x013a, 0x013c, 0x013d,
    0x013f, 0x0140, 0x0142, 0x0143, 0x0145, 0x0146, 0x0148, 0x014a,
    0x014b, 0x014d, 0x014f, 0x0150, 0x0152, 0x0154, 0x0156, 0x0158,
    0x015a, 0x015b, 0x015d, 0x015f, 0x0161, 0x0164, 0x0166, 0x0168,
    0x016a, 0x016c, 0x016f, 0x0171, 0x0173, 0x0176, 0x0178, 0x017b,
    0x017d, 0x0180, 0x0182, 0x0185, 0x0188, 0x018b, 0x018e, 0x0191,
    0x0194, 0x0197, 0x019a, 0x019d, 0x01a0, 0x01a3, 0x01a7, 0x01aa,
    0x01ae, 0x01b1, 0x01b5, 0x01b9, 0x01bd, 0x01c1, 0x01c5, 0x01c9,
    0x01cd, 0x01d1, 0x01d5, 0x01da, 0x01df, 0x01e3, 0x01e8, 0x01ed,
    0x01f2, 0x01f7, 0x01fc, 0x0202, 0x0207, 0x020d, 0x0213, 0x0219,
    0x021f, 0x0225, 0x022c, 0x0233, 0x0239, 0x0240, 0x0248, 0x024f,
    0x0257, 0x025f, 0x0267, 0x026f, 0x0278, 0x0281, 0x028a, 0x0293,
    0x029d, 0x02a7, 0x02b1, 0x02bc, 0x02c7, 0x02d3, 0x02df, 0x02eb,
    0x02f8, 0x0305, 0x0313, 0x0321, 0x0330, 0x0340, 0x0350, 0x0360,
    0x0372, 0x0384, 0x0397, 0x03ab, 0x03c0, 0x03d6, 0x03ec, 0x0404,
    0x041e, 0x0438, 0x0454, 0x0471, 0x0490, 0x04b1, 0x04d4, 0x04f9,
    0x0520, 0x054a, 0x0577, 0x05a6, 0x05d9, 0x0610, 0x064c, 0x068b,
    0x06d1, 0x071c, 0x076e, 0x07c8, 0x082b, 0x0899, 0x0913, 0x099b,
    0x0a34, 0x0ae1, 0x0ba8, 0x0c8d, 0x0d98, 0x0ed4, 0x104f, 0x121e,
    0x1461, 0x174a, 0x1b2b, 0x209a, 0x28bf, 0x3654, 0x517d, 0xa2fa
};
