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

#include <config.h>
#include <math.h>
#include "internal_utils_math.h"
/**
 * @ingroup utils
 */

/**
 * @addtogroup utilsatan
 * @{
 */

/**
 * @brief Arctan value of floating-point input.
 * @param[in]   y and x the input values.
 * @return arctan value of y/x.
 *
 * Function notes:
 * The input arguments are for tangent of the radian to be returned.
 * returned value falls in the range of [-PI, PI)
 */

#ifdef ENA_LUT_ATAN
static float32_t aractanlut[] =
{
    0.00000000, 0.00999967, 0.01999733, 0.02999100, 0.03997869, 0.04995840, 0.05992815, 0.06988600,
    0.07982998, 0.08975818, 0.09966865, 0.10955953, 0.11942893, 0.12927499, 0.13909595, 0.14888996,
    0.15865526, 0.16839015, 0.17809294, 0.18776195, 0.19739556, 0.20699219, 0.21655031, 0.22606839,
    0.23554498, 0.24497867, 0.25436804, 0.26371184, 0.27300870, 0.28225741, 0.29145682, 0.30060568,
    0.30970293, 0.31874758, 0.32773852, 0.33667481, 0.34555560, 0.35437992, 0.36314702, 0.37185606,
    0.38050640, 0.38909721, 0.39762798, 0.40609807, 0.41450688, 0.42285392, 0.43113875, 0.43936089,
    0.44751996, 0.45561567, 0.46364760, 0.47161555, 0.47951928, 0.48735857, 0.49513328, 0.50284320,
    0.51048833, 0.51806855, 0.52558380, 0.53303409, 0.54041952, 0.54774004, 0.55499572, 0.56218672,
    0.56931317, 0.57637519, 0.58337301, 0.59030676, 0.59717667, 0.60398299, 0.61072594, 0.61740589,
    0.62402308, 0.63057780, 0.63707030, 0.64350110, 0.64987046, 0.65617871, 0.66242629, 0.66861355,
    0.67474097, 0.68080884, 0.68681765, 0.69276780, 0.69865978, 0.70449406, 0.71027100, 0.71599114,
    0.72165483, 0.72726268, 0.73281509, 0.73831260, 0.74375558, 0.74914461, 0.75448018, 0.75976276,
    0.76499283, 0.77017093, 0.77529752, 0.78037310, 0.78539819
};

static inline float32_t atan1(float x)
{
    unsigned ti = x * 100;
    float32_t lut1 = aractanlut[ti];
    float32_t lut2 = aractanlut[ti + 1];
    return (lut1 + (x * 100 - ti) * (lut2 - lut1));
}
#endif

/* function description */
float32_t riscv_dsp_atan2_f32(float32_t y, float32_t x)
{

#if !defined(__NDS32_EXT_FPU_SP__) && !defined(__riscv_float_abi_single) && !defined(__riscv_float_abi_double)
    //Not support FPU (v3_d10, v3)
    //MAE:0.00000000, MAXDIFF: 0.00000000, CYCLE: 10735054
    return atan2f(y, x);
#else

#ifdef ENA_LUT_ATAN
    //LUT for atan
    //MAE:0.00000005 MAXDIFF: 0.00000310 CYCLE: 1397526

    float32_t v;
    const float32_t absy = fabsf(y);
    const float32_t absx = fabsf(x);

    if (absx < absy)
        v = absx / absy;
    else
        v = absy / absx;

    v = atan1(v);

    if (absx < absy)
        v = HALF_PI - v;

    if (y < 0.0f)
        v = -v;

    if (x < 0.0f)
    {
        if (y < 0.0f)
            return -PI - v;
        else
            return  PI - v;
    }
    return v;
#else
    //second-order approximation
    //MAE:0.00000036 MAXDIFF: 0.00414830 CYCLE: 1175466

    if ( x == 0.0f )
    {
        if ( y > 0.0f )
            return HALF_PI;
        if ( y == 0.0f )
            return 0.0f;
        return -HALF_PI;
    }

    float32_t atan;
    float32_t z = y / x;

    if ( fabsf( z ) < 1.0f )
    {
        atan = z / (1.0f + 0.28f * z * z);
        if ( x < 0.0f )
        {
            if ( y < 0.0f )
                return atan - PI;
            return atan + PI;
        }
    }
    else
    {
        atan = HALF_PI - z / (z * z + 0.28f);
        if ( y < 0.0f )
            return atan - PI;
    }
    return atan;

#endif // ENA_LUT_ATAN
#endif // NDS32_EXT_FPU_SP
}


/**
 * @} end of utilsatan
 */
