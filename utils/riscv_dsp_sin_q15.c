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
 * @brief Sine value of q15 input.
 * @param[in]   src     the input value.
 * @return sine value of the input.
 *
 * Function notes:
 * The input argument <code>src</code> is in radians represented in Q15
 * format. It is encoded to represent a value in [-PI, PI) or
 * [-2 * PI, 2 * PI) (if FOUR_PI_RANGE is defined) as [-1, 1).
 */

/* function description */
q15_t riscv_dsp_sin_q15(q15_t src)
{
    int32_t index;
    q15_t y0;

    /* input x is in radians */
    /* x == -2 * PI -> sin(x) == 0.0f, x == -PI -> sin(x) == 0.0f */
#ifndef PERF_TEST
    long neg;
    if (src == INT16_MIN)
    {
        return 0;
    }

    neg = src >> 15;
    src = _dsp_abs(src);
#endif

#ifndef PERF_TEST
    if (src > HALF_PI_Q15)
    {
        src = PI_Q15 - src;
    }
#endif

    /* range [0,PI / 2], get index */
#if RES_N == 3600
    /* due to the fact of not being power of 2, the propagation of
     ** truncation errors causes problems. thus, use Q31 treatment here to
     ** minimize the errors. */
    index = (((q31_t) src) << 16) / STEP_UNIT_Q31;
#else
    index = src / STEP_UNIT_Q15;
#endif

#ifdef NO_INTERPOLATION
    index = (RES_N >> 2) - index;
    y0 = riscv_dsp_cos_table_q15[index];
#else
    /* get table pointer for two nearest entries */
    q15_t fract;
    const q15_t *ptr;
    q15_t y1;

    fract = src - index * STEP_UNIT_Q15;

    fract *= (RES_N >> 1);

#ifdef HERMITE_INTERPOLATION  // default define HERMITE_INTERPOLATION

#ifdef ENA_CS_Q_HMT_TBL_LNR_INTP // default define ENA_CS_Q_HMT_TBL_LNR_INTP
    /* linear interpolation uses Hermit table */
    index = (RES_N >> 2) - index - 1;
    ptr = &riscv_dsp_cos_table_q15_linear[index];
    y0 = ptr[1];
    y1 = ptr[0];

#if   defined (ENA_DSP_ISA_32)
    y0 = NDS_DSP_KMMAWB(y0, fract2, NDS_DSP_KSUBH(y1, y0));
#else
    y0 += (q15_t)(((q31_t)fract * (y1 - y0)) >> 15);
#endif

#else   // default define ENA_CS_Q_HMT_TBL_LNR_INTP

    q15_t y2, y3;
    index = (RES_N >> 2) - index;
    ptr = &riscv_dsp_cos_table_q15[index];
    y0 = ptr[3];
    y1 = ptr[2];
    y2 = ptr[1];
    y3 = ptr[0];

    /* Hermite interpolation */
    q31_t tmp = (q31_t)y2 - y1;
    q31_t tmp2 = (q31_t)y3 - y0 - 3 * tmp;
    tmp += (q31_t)y0 - y1 - tmp2 + ((tmp2 * fract) >> 15);
    y0 = y1 + (((y2 - (q31_t)y0 + ((tmp * fract) >> 15)) * fract) >> 16);
#endif
#else
    /* linear interpolation */
    index = (RES_N >> 2) - index - 1;
    ptr = &riscv_dsp_cos_table_q15[index];
    y0 = ptr[1];
    y1 = ptr[0];

    y0 += (q15_t)(((q31_t)fract * (y1 - y0)) >> 15);
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
