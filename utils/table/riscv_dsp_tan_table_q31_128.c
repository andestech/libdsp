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
 * riscv_dsp_tan_table_q31_128[k] = tan(2 * PI * k / 128)
 * where 0 <= k <= 32.
 */

const q31_t riscv_dsp_tan_table_q31_128[33] =
{
    0x00000000, 0x000c9394, 0x001936bc, 0x0025f959,
    0x0032ebec, 0x00401fea, 0x004da821, 0x005b9928,
    0x006a09e6, 0x00791438, 0x0088d5b9, 0x009970c4,
    0x00ab0dc1, 0x00bddccf, 0x00d21801, 0x00e8065e,
    0x01000000, 0x011a73d5, 0x0137efd9, 0x01592d11,
    0x017f218e, 0x01ab1c36, 0x01def13b, 0x021d443b,
    0x026a09e6, 0x02cb78da, 0x034beb3d, 0x03fe0261,
    0x0506ffb9, 0x06bdcfd3, 0x0a27362d, 0x145affed,
    0x7fffffff
};
