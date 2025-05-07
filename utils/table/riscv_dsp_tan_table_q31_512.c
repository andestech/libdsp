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
 * riscv_dsp_tan_table_q31_512[k] = tan(2 * PI * k / 512)
 * where 0 <= k <= 128.
 */

const q31_t riscv_dsp_tan_table_q31_512[129] =
{
    0x00000000, 0x0000c912, 0x00019234, 0x00025b75,
    0x000324e5, 0x0003ee93, 0x0004b88e, 0x000582e8,
    0x00064daf, 0x000718f4, 0x0007e4c6, 0x0008b137,
    0x00097e56, 0x000a4c35, 0x000b1ae5, 0x000bea76,
    0x000cbafb, 0x000d8c85, 0x000e5f26, 0x000f32f2,
    0x001007fa, 0x0010de53, 0x0011b610, 0x00128f46,
    0x00136a08, 0x0014466d, 0x0015248b, 0x00160477,
    0x0016e64a, 0x0017ca1b, 0x0018b002, 0x00199819,
    0x001a827a, 0x001b6f40, 0x001c5e87, 0x001d506d,
    0x001e450e, 0x001f3c8b, 0x00203703, 0x00213499,
    0x0022356e, 0x002339a8, 0x0024416c, 0x00254ce1,
    0x00265c31, 0x00276f86, 0x0028870e, 0x0029a2f6,
    0x002ac370, 0x002be8b0, 0x002d12ea, 0x002e4258,
    0x002f7734, 0x0030b1bc, 0x0031f233, 0x003338dc,
    0x00348600, 0x0035d9ec, 0x003734f0, 0x00389760,
    0x003a0198, 0x003b73f5, 0x003ceedd, 0x003e72bb,
    0x00400000, 0x00419724, 0x004338a7, 0x0044e512,
    0x00469cf5, 0x004860ec, 0x004a319c, 0x004c0fb6,
    0x004dfbf6, 0x004ff729, 0x00520228, 0x00541ddd,
    0x00564b44, 0x00588b6e, 0x005adf7e, 0x005d48b3,
    0x005fc864, 0x00626004, 0x0065112a, 0x0067dd8d,
    0x006ac70d, 0x006dcfb9, 0x0070f9ce, 0x007447c3,
    0x0077bc4f, 0x007b5a6d, 0x007f256b, 0x008320f1,
    0x0087510f, 0x008bba4c, 0x009061b6, 0x00954cfb,
    0x009a827a, 0x00a00967, 0x00a5e9ec, 0x00ac2d52,
    0x00b2de36, 0x00ba08c7, 0x00c1bb13, 0x00ca056a,
    0x00d2facf, 0x00dcb199, 0x00e7442b, 0x00f2d1ef,
    0x00ff8098, 0x010d7dc8, 0x011d0144, 0x012e4ff1,
    0x0141bfee, 0x0157be4f, 0x0170d752, 0x018dc25c,
    0x01af73f5, 0x01d7398d, 0x0206e5f6, 0x02411b7b,
    0x0289cd8b, 0x02e7318b, 0x0363a078, 0x0411ba80,
    0x0516bffb, 0x06c99c65, 0x0a2f122b, 0x145eed68,
    0x7fffffff
};
