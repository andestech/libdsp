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
 * The input arguments are for tangent of the radian to be returned. It is scaled
 * as Q(RES_LOGN).31-(RES_LOGN). For example, it is Q8.23 if RES_N = 256. The
 * returned value falls in the range of [-PI, PI) scaled as
 * depending on global symbol FOUR_PI_RANGE:
 *  1> defined: [-0.5, 0.5)
 *  2> otherwise: [-1.0, 1.0)
 */

/* function description */
#ifndef ENA_HIGHER_PERFORMANCE_ATAN2_Q31
#include <math.h>
#else
#define NORM_SH     (31)
#define CST_1       (1UL << NORM_SH)
#define CST_2       (4UL << NORM_SH)
#define D2Q(F)      ((q31_t)((float)F * (float)CST_1))
#define MULQ(a, b) ((q31_t)(((q63_t)(a) * (b)) >> 32))

#define OCTANTIFY(_x, _y, _o)   do {                            \
        q63_t _t; _o= 0;                                            \
        if(_y<  0)  {            _x= -_x;   _y= -_y; _o -= 4; }     \
        if(_x<= 0)  { _t= _x;    _x=  _y;   _y= -_t; _o += 2; }     \
        if(_x<=_y)  { _t= _y-_x; _x= _x+_y; _y=  _t; _o += 1; }     \
    } while(0);

static q31_t g[] =
{
    0xfaeb2732, 0xf101655d, 0xe7bff755, 0xdf89bde0,
    0xd8a9408f, 0xd336910f, 0xcf3d3b45, 0xccab377a,
    0xcb599b74, 0xcb1de9f0, 0xcbc0a9ec, 0xcd161f74,
    0xcee82ef2, 0xd114cb0d, 0xd3794a94, 0xd5fd248c,

    0x518b1982, 0x522931af, 0x53507029, 0x54d987a4,
    0x56905b61, 0x5842bb6e, 0x59bed6a8, 0x5add6ded,
    0x5b851ffb, 0x5ba5c800, 0x5b3f54e1, 0x5a53fa81,
    0x58f5f642, 0x573171fe, 0x55195fa0, 0x52bdae09,

    0xfffffcca, 0xfffd805b, 0xfff447f7, 0xffe1e27d,
    0xffc685d4, 0xffa4b029, 0xff812c93, 0xff61f698,
    0xff4d2301, 0xff48ac6f, 0xff58cc05, 0xff815aed,
    0xffc3136e, 0x001f0dc0, 0x00945add, 0x0121df1b
};

static inline q63_t dsp_atan1(const q31_t x)
{
    q63_t xi;
    unsigned ti = x >> 27;
    q63_t a = g[ti];
    q63_t b = g[ti + 16];
    q63_t c = g[ti + 32];
    xi = MULQ(MULQ(a, x) + b, x) + c;
    return xi;
}
#define BRAD_QPI 0x20000000  /* 2147483648/4 */
#endif

q31_t riscv_dsp_atan2_q31(q31_t y, q31_t x)
{
    if (y != 0)
    {
#ifdef ENA_HIGHER_PERFORMANCE_ATAN2_Q31
        q63_t phi;
        q31_t t;
        q63_t x1 = x, y1 = y;
        OCTANTIFY(x1, y1, phi);
        phi *= BRAD_QPI;
        y1 <<= 31;

#ifdef NDS_CPU_64
        t = (y1 / x1);
#else
        t = dsp_div_u64_u32((uint64_t)y1, (uint32_t)x1);
#endif
        return (q31_t)(phi + dsp_atan1(t));
#else
        q31_t  ret;
        float32_t fy, fx, retf;

        fx = dsp_convert_q31_to_float(x);
        fy = dsp_convert_q31_to_float(y);

        retf = atan2f(fy, fx);
        ret = dsp_convert_float_to_q31(retf / PI);
        return ret;
#endif
    }
    else
        return (x >= 0) ? 0 : INT32_MIN;
}


/**
 * @} end of utilsatan
 */
