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
 * @param[in]   src     the input value.
 * @return arctan value of the input.
 *
 * Function notes:
 * The input argument is tangent of the radian to be returned. It is scaled
 * as Q(RES_LOGN).15-(RES_LOGN). For example, it is Q8.7 if RES_N = 256. The
 * returned value falls in the range of [-PI / 2, PI / 2) scaled as
 * depending on global symbol FOUR_PI_RANGE:
 *  1> defined: [-0.25, 0.25)
 *  2> otherwise: [-0.5, 0.5)
 */

/* function description */
#define NORM_SH     (15)
#define CST_1       (1L << NORM_SH)
#define CST_2       (4L << NORM_SH)
#define D2Q(F)      ((q15_t)((float)F * (float)CST_1))
#define MULQ(a, b) ((q15_t)(((q31_t)(a) * (b)) >> 16))

#define OCTANTIFY(_x, _y, _o)   do {                            \
        q31_t _t; _o= 0;                                            \
        if(_y<  0)  {            _x= -_x;   _y= -_y; _o -= 4; }     \
        if(_x<= 0)  { _t= _x;    _x=  _y;   _y= -_t; _o += 2; }     \
        if(_x<=_y)  { _t= _y-_x; _x= _x+_y; _y=  _t; _o += 1; }     \
    } while(0);

static q15_t ga[] =
{
    D2Q(0.00012894702004 / PI), D2Q(0.00410552202930 / PI),
    D2Q(0.01797827343558 / PI), D2Q(0.04487866261926 / PI),
    D2Q(0.08467202719892 / PI), D2Q(0.13495471234832 / PI),
    D2Q(0.19237801112651 / PI), D2Q(0.25366425315298 / PI)
};

static q15_t gb[] =
{
    D2Q(0.99534894623805 / PI * 2), D2Q(0.96541373335686 / PI * 2),
    D2Q(0.91062230899554 / PI * 2), D2Q(0.83915654988516 / PI * 2),
    D2Q(0.75963586412685 / PI * 2), D2Q(0.67915650580081 / PI * 2),
    D2Q(0.60252898580173 / PI * 2), D2Q(0.53241718079412 / PI * 2)
};

static inline q31_t dsp_atan1(const q31_t x)
{
    q31_t xi;
    q31_t ti = x >> 12;
    q31_t a = ga[ti];
    q31_t b = gb[ti];

    xi = a + MULQ(x, b);
    return xi;
}

#define BRAD_PI 0x2000  /* 32768/4 */
q15_t riscv_dsp_atan2_q15(q15_t y, q15_t x)
{
    if (y != 0)
    {
        q31_t phi;
        q31_t t;
        q31_t x1 = x, y1 = y;
        OCTANTIFY(x1, y1, phi);
        phi *= BRAD_PI;
        t = ((uint32_t) (y1) << 15 ) / (uint32_t)x1;
        return (phi + dsp_atan1(t));
    }
    else
        return (x >= 0) ? 0 : INT16_MAX;
}


/**
 * @} end of utilsatan
 */
