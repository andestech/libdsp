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
#include "internal_math_types.h"
#include "internal_utils_math.h"

/**
 * @ingroup utils
 */

/**
 * @defgroup utilssin Sine
 *
 * This function returns sin(x) by using the table look-up. The symbol
 * <code>USE_COSINE</code> implemente the function returns sin(x) as
 * cos(x - PI / 2). The resolution of the look-up table is pre-defined
 * using <code>RES_N</code>. Application should choose the best resolution
 * based on - Once the BRES is set, the sin(x) for x with:
 * 1. higher resolution is calculated using hermit interpolation method.
 * 2. same or lower resolution is simply retrieved from table.
 * In case 2, then there is simply no need to call this function.
 *
 * <pre>
 *      dst = sin(src) = cos(src - PI / 2);
 * </pre>
 *
 * The symbol <code>HERMITE_INTERPOLATION</code> is defined as to used the
 * Hermit interpolation method to evaluate the sin(x).
 *
 * The Hermit interpolation method is obtained to web site as follow:
 *      http://paulbourke.net/miscellaneous/interpolation/
 *      tension: 1 is high, 0 is normal, -1 is low.
 *      bias: 0 is even, positive is towards first segment, negaitve is
 *      towards the other.
 *
 *      double hermite(double y0, double y1, double y2, double y3,
 *                  double fract, double bias, double tension)
 *      {
 *          double fract2, fract3, m0, m1;
 *          double x0, x1, x2, x3;
 *
 *          fract2 = fract * fract;
 *          fract3 = fract2 * fract;
 *          m0 = (y1 - y0) * (1 + bias) * (1 - tension) / 2
 *              + (y2 - y1) * (1 + bias) * (1 - tension) / 2;
 *          m1 = (y2 - y1) * (1 + bias) * (1 - tension) / 2
 *              + (y3 - y2) * (1 + bias) * (1 - tension) / 2;
 *          x0 = 2 * fract3 - 3 * fract2 + 1;
 *          x1 = fract3 - 2 * fract2 + fract;
 *          x2 = fract3 - fract2;
 *          x3 = -2 * fract3 + 3 * fract2;
 *          return x0 * y1 + x1 * m0 + x2 * m1 + x3 * y2;
 *      }
 *
 *  For tension == 0 and bias == 0:
 *      double hermite(double y0, double y1, double y2, double y3,
 *                  double fract)
 *      {
 *          double fract2, fract3;
 *          double x0, x1, x2, x3;
 *
 *          fract2 = fract * fract;
 *          fract3 = fract2 * fract;
 *          x0 = 2 * fract3 - 3 * fract2 + 1;
 *          x1 = (fract3 - 2 * fract2 + fract) / 2;
 *          x2 = (fract3 - fract2) / 2;
 *          x3 = -2 * fract3 + 3 * fract2;
 *          return -x1 * y0 + (x0 - x2) * y1 + (x1 + x3) * y2 + x2 * y3;
 *      }
 *
 * The input argument is in radians. Though the logic handles all possible
 * <code>src</code> values, it is better that this function be used to
 * handle only fixed range such as [-PI, PI). Then the first while loop can
 * be eliminated. When defined, <code>FOUR_PI_RANGE</code> can be defined
 * for range [-2 * PI, 2 * PI).
 */

/**
 * @addtogroup utilssin
 * @{
 */

/**
 * @brief sin input union.
 */

/**
 * @brief Sine value of floating-point input.
 * @param[in]   src     the input value.
 * @return sine value of the input.
 */

extern float32_t riscv_dsp_cos_f32(float32_t src);

/* function description */
float32_t riscv_dsp_sin_f32(float32_t src)
{
    return sinf(src);
}

/**
 * @} end of utilssin
 */
