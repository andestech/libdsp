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

#include <config.h>
//#include <stdlib.h>
#include "internal_math_types.h"
#include "internal_utils_math.h"

/**
 * @ingroup utils
 */

/**
 * @addtogroup utilscos
 * @{
 */

/**
 * @brief Cosine value of q31 input.
 * @param[in]   src     the input value.
 * @return cosine value of the input.
 */

/* function description */
q31_t riscv_dsp_cos_q31(q31_t src)
{
    int32_t index;
    q31_t y0;

    /* input x is in radians scaled by 4 * PI if FOUR_PI_RANGE defined or
     ** 2 * PI, otherwise. */
#ifndef PERF_TEST
    int32_t neg = 0;
    if (src == INT32_MIN)
    {
        /* x == -PI -> cos(x) == -1.0f */
        return INT32_MIN;
    }
    src = dsp_abs(src);
#endif

#ifndef PERF_TEST
    if (src > HALF_PI_Q31)
    {
        src = (uint32_t)PI_Q31 - src;
        neg++;
    }
#endif

    /* range [0,PI/2], get index*/
    index = src / STEP_UNIT_Q31;

#ifdef NO_INTERPOLATION
    y0 = riscv_dsp_cos_table_q31[index];
#else
    q31_t fract;
    q31_t y1;
    fract = src - (q31_t)index * STEP_UNIT_Q31;
    fract *= (RES_N >> 1);
    /* get table pointer for two nearest entries */
    const q31_t *ptr;
#ifdef HERMITE_INTERPOLATION

#ifdef ENA_CS_Q_HMT_TBL_LNR_INTP
    /* linear interpolation uses Hermit table */
    ptr = &riscv_dsp_cos_table_q31_linear[index];
    y0 = ptr[0];
    y1 = ptr[1];

    y0 += (q31_t)(((q63_t)fract * (y1 - y0)) >> 31);

#else

    ptr = &riscv_dsp_cos_table_q31[index];
    q31_t y2, y3;
    y0 = ptr[1];
    y1 = ptr[2];
    y2 = ptr[3];
    y3 = ptr[4];

    /* Hermite interpolation */
    q63_t tmp = (q63_t)y2 - y1;
    q63_t tmp2 = (q63_t)y3 - y0 - 3 * tmp;
    y3 = (q31_t)((tmp2 * fract) >> 31);
    tmp += (q63_t)y0 - y1 - tmp2 + y3;
    y0 = y1 + (((y2 - (q63_t)y0 + ((tmp * fract) >> 31)) * fract) >> 32);
#endif
#else
    /* linear interpolation */
    ptr = &riscv_dsp_cos_table_q31[index];
    y0 = ptr[0];
    y1 = ptr[1];

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
 * @} end of utilscos
 */
