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
//#include <stdlib.h>
#include "internal_math_types.h"
#include "internal_utils_math.h"
/**
 * @ingroup utils
 */

/**
 * @addtogroup utilssin
 * @{
 */

/**
 * @brief Sine value of q31 input.
 * @param[in]   src     the input value.
 * @return sine value of the input.
 *
 * Function notes:
 * The input argument <code>src</code> is in radians represented in Q31
 * format. It is encoded to represent a value in [-PI, PI) or
 * [-2 * PI, 2 * PI) (if FOUR_PI_RANGE is defined) as [-1, 1).
 */

/* function description */
q31_t riscv_dsp_sin_q31(q31_t src)
{
    int32_t index;
    q31_t y0;

    /* input x is in radians scaled by 4 * PI if FOUR_PI_RANGE defined or
     ** 2 * PI, otherwise. */
    /* x == -2 * PI -> sin(x) == 0.0f, x == -PI -> sin(x) == 0.0f */
#ifndef PERF_TEST
    int32_t neg;
    if (src == INT32_MIN)
    {
        return 0;
    }

    neg = src >> 31;
    src = _dsp_abs(src);
#endif

#ifndef PERF_TEST
    if (src > HALF_PI_Q31)
    {
        src = (uint32_t)PI_Q31 - src;
    }
#endif

    /* range [0, PI / 2], get index */
    index = src / STEP_UNIT_Q31;
#ifdef NO_INTERPOLATION
    index = (RES_N >> 2) - index;
    y0 = riscv_dsp_cos_table_q31[index];
#else
    q31_t fract;
    fract = src - (q31_t)index * STEP_UNIT_Q31;

    fract *= (RES_N >> 1);

    /* get table pointer for two nearest entries */
    q31_t y1;
    const q31_t *ptr;
#ifdef HERMITE_INTERPOLATION

#ifdef ENA_CS_Q_HMT_TBL_LNR_INTP
    /* linear interpolation uses Hermit table */
    index = (RES_N >> 2) - index - 1;
    ptr = &riscv_dsp_cos_table_q31_linear[index];
    y0 = ptr[1];
    y1 = ptr[0];

    y0 += (q31_t)(((q63_t)fract * (y1 - y0)) >> 31);

#else

    q31_t y2, y3;
    index = (RES_N >> 2) - index;
    ptr = &riscv_dsp_cos_table_q31[index];
    y0 = ptr[3];
    y1 = ptr[2];
    y2 = ptr[1];
    y3 = ptr[0];


    /* Hermite interpolation */
    q63_t tmp = (q63_t)y2 - y1;
    q63_t tmp2 = (q63_t)y3 - y0 - 3 * tmp;
    tmp += (q63_t)y0 - y1 - tmp2;
    y0 = y1 + (((q63_t)(y2 - y0) * fract) >> 32);
#endif
#else
    /* linear interpolation */
    index = (RES_N >> 2) - index - 1;
    ptr = &riscv_dsp_cos_table_q31[index];
    y0 = ptr[1];
    y1 = ptr[0];

    y0 += (q31_t)(((q63_t)fract * (y1 - y0)) >> 31);
#endif /* HERMITE_INTERPOLATION */
#endif /* NO_INTERPOLATION */

#ifndef PERF_TEST
    if (neg)
    {
        return -y0;
    }
    else
    {
        return y0;
    }
#else
    return y0;
#endif

}

/**
 * @} end of utilssin
 */
