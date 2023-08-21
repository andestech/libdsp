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
 * riscv_dsp_tan_table_f32_64[k] = tan(2 * PI * k / 64)
 * where 0 <= k <= 16.
 */

const float32_t riscv_dsp_tan_table_f32_64[17] =
{
    0.000000000000,     0.098491403357,     0.198912367380,     0.303346683607,
    0.414213562373,     0.534511135951,     0.668178637919,     0.820678790829,
    1.000000000000,     1.218503525588,     1.496605762665,     1.870868411789,
    2.414213562373,     3.296558208938,     5.027339492126,    10.153170387609,
    63.000000000000
};

