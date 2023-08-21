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
 *   riscv_dsp_cos_table_f64_64[k + 2] = cos(2 * PI * k / 64)
 * #else
 *   riscv_dsp_cos_table_f64_64[k] = cos(2 * PI * k / 64)
 * #endif
 * where 0 <= k <= 16.
 **/

#ifdef HERMITE_INTERPOLATION
const double riscv_dsp_cos_table_f64_64[21] =
{
    0.980785280403230, 0.995184726672197,
    1.000000000000000, 0.995184726672197, 0.980785280403230, 0.956940335732209,
#else
const double riscv_dsp_cos_table_f64_64[17] =
{
    1.000000000000000, 0.995184726672197, 0.980785280403230, 0.956940335732209,
#endif
    0.923879532511287, 0.881921264348355, 0.831469612302545, 0.773010453362737,
    0.707106781186548, 0.634393284163645, 0.555570233019602, 0.471396736825998,
    0.382683432365090, 0.290284677254462, 0.195090322016128, 0.098017140329561,
#ifdef HERMITE_INTERPOLATION
    0.000000000000000, -0.098017140329561, -0.195090322016128
#else
    0.000000000000000
#endif
};

const double riscv_dsp_cos_table_f64_linear_64[17] =
{
    1.000000000000000, 0.995184726672197, 0.980785280403230, 0.956940335732209,
    0.923879532511287, 0.881921264348355, 0.831469612302545, 0.773010453362737,
    0.707106781186548, 0.634393284163645, 0.555570233019602, 0.471396736825998,
    0.382683432365090, 0.290284677254462, 0.195090322016128, 0.098017140329561,
    0.000000000000000
};

/***************************************************************************
 * cosine reciprocal table:                                                *
 *   1 / (2 * cos(x)) where 0 <= x <= PI / 2                               *
 *   riscv_dsp_cos_recip_table_f64_64[k] = 1 / (2 * cos(2 * PI * k / 64))  *
 *   where 0 <= k < 16.                                                   *
 ***************************************************************************/

const double riscv_dsp_cos_recip_table_f64_64[16] =
{
    0.500000000000000, 0.502419286188156, 0.509795579104159, 0.522498614939689,
    0.541196100146197, 0.566944034816358, 0.601344886935045, 0.646821783359990,
     0.70710678118655,  0.78815462345125,  0.89997622313642,  1.06067768599035,
     1.30656296487638,  1.72244709823833,  2.56291544774151,  5.10114861868916,
};

