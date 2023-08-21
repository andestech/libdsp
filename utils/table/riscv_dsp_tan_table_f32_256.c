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
 * riscv_dsp_tan_table_f32_256[k] = tan(2 * PI * k / 256)
 * where 0 <= k <= 64.
 */

const float32_t riscv_dsp_tan_table_f32_256[65] =
{
    0.000000000000,     0.024548622109,     0.049126849769,     0.073764431522,
    0.098491403357,     0.123338236137,     0.148335987538,     0.173516460138,
    0.198912367380,     0.224557509317,     0.250486960191,     0.276737270140,
    0.303346683607,     0.330355377344,     0.357805721315,     0.385742566271,
    0.414213562373,     0.443269513891,     0.472964775891,     0.503357699799,
    0.534511135951,     0.566493002730,     0.599376933682,     0.633243016178,
    0.668178637919,     0.704279460865,     0.741650546272,     0.780407659654,
    0.820678790829,     0.862605932257,     0.906347169019,     0.952079146701,
    1.000000000000,     1.050332846240,     1.103329975733,     1.159277907333,
    1.218503525588,     1.281381580037,     1.348343913487,     1.419890903494,
    1.496605762665,     1.579172567960,     1.668399205584,     1.765246870094,
    1.870868411789,     1.986658792343,     2.114322357549,     2.255963851929,
    2.414213562373,     2.592402517738,     2.794812772490,     3.027043204318,
    3.296558208938,     3.613535681307,     3.992223783770,     4.453202224414,
    5.027339492126,     5.763142005119,     6.741452405415,     8.107785803677,
    10.153170387609,    13.556669242352,    20.355467624987,    40.735483872083,
    255.000000000000
};

