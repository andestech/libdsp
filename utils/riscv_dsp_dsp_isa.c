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

//#include <config.h>

/**
 * @ingroup util
 */

/**
 * @addtogroup dsp_isa
 * @{
 */

#ifdef ENA_DSP_ISA
#include "internal_math_types.h"    //for ENA_DSP_MSG
#include "internal_dsp_isa.h"       //for importing union data types.

//#ifndef NDS_DSP_ASSUME_ALIGNED
//#if !defined(NDS_DSP_ASSUME_ALIGNED) || (ENA_DSP_BE)
union32_t simd_unalign_val[MAX_SIMD_UNALGN_ENTRY];
//#endif

#ifdef SIM_DSP_ISA_WITH_C
//These C functions simulate the DSP ISA instructions

//Represents the status register (ov)
int riscv_dsp_dsp_ov_flag = 0;

DSP_FUNC_PREF q31_t clip_q63_to_q31(q63_t x)
{
    //return((q31_t) (x >> 32) != ((q31_t) x >> 31)) ?
    //((0x7FFFFFFF ^ ((q31_t) (x >> 63)))) : (q31_t) x;

    q31_t ret;

    if ((q31_t)(x >> 32) != (((q31_t)x) >> 31))
    {
        riscv_dsp_dsp_ov_flag = 1;
        ret = ((q31_t)0x7FFFFFFF ^ ((q31_t) (x >> 63)));
    }
    else
    {
        ret = (q31_t)x;
    }
    return ret;
}

DSP_FUNC_PREF q31_t NDS_DSP_RADDW(q31_t x, q31_t y)
{
    return (q31_t)(((q63_t)x + y) >> 1);
}

DSP_FUNC_PREF uint32_t NDS_DSP_URADDW(uint32_t x, uint32_t y)
{
    return (uint32_t)(((uint64_t)x + y) >> 1);
}

DSP_FUNC_PREF q31_t NDS_DSP_RSUBW(q31_t x, q31_t y)
{
    return (q31_t)(((q63_t)x - y) >> 1);
}

DSP_FUNC_PREF uint32_t NDS_DSP_URSUBW(uint32_t x, uint32_t y)
{
    return (uint32_t)(((uint64_t)x - y) >> 1);
}

DSP_FUNC_PREF q31_t NDS_DSP_RADD16(q31_t x, q31_t y)
{
    union32_t ret;
    union32_t tx;// = (union32_t)x;
    union32_t ty;// = (union32_t)y;
    tx.w0 = x;
    ty.w0 = y;

    ret.b16.h0 = (q15_t)(((q31_t)tx.b16.h0 + ty.b16.h0) >> 1);
    ret.b16.h1 = (q15_t)(((q31_t)tx.b16.h1 + ty.b16.h1) >> 1);

    return ret.w0;
}

DSP_FUNC_PREF q31_t NDS_DSP_KADD16(q31_t x, q31_t y)
{
    q31_t sum;
    q31_t r, s;

    r = (short) x;
    s = (short) y;

    r = NDS_DSP_SATS(r + s, 16);
    s = NDS_DSP_SATS(((q31_t) ((x >> 16) + (y >> 16))), 16) << 16;
    sum = (s & 0xFFFF0000) | (r & 0x0000FFFF);

    return sum;

}

DSP_FUNC_PREF q31_t NDS_DSP_RSUB16(q31_t x, q31_t y)
{
    union32_t ret;
    union32_t tx;// = (union32_t)x;
    union32_t ty;// = (union32_t)y;
    tx.w0 = x;
    ty.w0 = y;

    ret.b16.h0 = (q15_t)(((q31_t)tx.b16.h0 - ty.b16.h0) >> 1);
    ret.b16.h1 = (q15_t)(((q31_t)tx.b16.h1 - ty.b16.h1) >> 1);

    return ret.w0;
}

DSP_FUNC_PREF q31_t NDS_DSP_KSUB16(q31_t x, q31_t y)
{
    q31_t sum;
    q31_t r, s;

    r = (short) x;
    s = (short) y;

    r = NDS_DSP_SATS(r - s, 16);
    s = NDS_DSP_SATS(((q31_t) ((x >> 16) - (y >> 16))), 16) << 16;
    sum = (s & 0xFFFF0000) | (r & 0x0000FFFF);

    return sum;
}

//Rt[31:16] = (Ra[31:16] + Rb[15:0]) (signed)>> 1;
//Rt[15:0]  = (Ra[15:0] - Rb[31:16]) (signed)>> 1;
DSP_FUNC_PREF q31_t NDS_DSP_RCRAS16(q31_t x, q31_t y)
{
    //     q31_t sum;
    // q31_t r, s;

    // r = (short) x;  //Ra[15:0]
    //     s = (short) y;  //Rb[15:0]

    //     r = (r  - (y >> 16)) >> 1;                  //Ra[15:0] - Rb[31:16]
    //     s = (((x >> 16) + s ) >> 1) << 16;          //Ra[31:16] + Rb[15:0]
    //     sum = (s & 0xFFFF0000) | (r & 0x0000FFFF);

    //     return sum;
    // #else

    union32_t ret, tx, ty;

    tx.w0 = x;
    ty.w0 = y;
    ret.b16.h1 = (q15_t)((q31_t)tx.b16.h1 + ty.b16.h0) >> 1;
    ret.b16.h0 = (q15_t)((q31_t)tx.b16.h0 - ty.b16.h1) >> 1;

    return ret.w0;

}

DSP_FUNC_PREF q31_t NDS_DSP_KCRAS16(q31_t x, q31_t y)
{
    union32_t ret, tx, ty;

    tx.w0 = x;
    ty.w0 = y;
    ret.b16.h1 = (q15_t)NDS_DSP_SATS((q31_t)tx.b16.h1 + ty.b16.h0, 16);
    ret.b16.h0 = (q15_t)NDS_DSP_SATS((q31_t)tx.b16.h0 - ty.b16.h1, 16);

    return ret.w0;
}

DSP_FUNC_PREF q31_t NDS_DSP_RCRSA16(q31_t x, q31_t y)
{
    // q31_t sum;
    // q31_t r, s;

    // r = (short) x;
    // s = (short) y;

    // r = (r + (y >> 16)) >> 1;
    // s = (((x >> 16) - s ) >> 1) << 16;

    // sum = (s & 0xFFFF0000) | (r & 0x0000FFFF);

    // return sum;
    union32_t ret, tx, ty;

    tx.w0 = x;
    ty.w0 = y;
    ret.b16.h1 = (q15_t)((q31_t)tx.b16.h1 - ty.b16.h0) >> 1;
    ret.b16.h0 = (q15_t)((q31_t)tx.b16.h0 + ty.b16.h1) >> 1;

    return ret.w0;
}

DSP_FUNC_PREF q31_t NDS_DSP_KCRSA16(q31_t x, q31_t y)
{
    union32_t ret, tx, ty;

    tx.w0 = x;
    ty.w0 = y;
    ret.b16.h1 = (q15_t)NDS_DSP_SATS((q31_t)tx.b16.h1 - ty.b16.h0, 16);
    ret.b16.h0 = (q15_t)NDS_DSP_SATS((q31_t)tx.b16.h0 + ty.b16.h1, 16);

    return ret.w0;
}

DSP_FUNC_PREF q31_t NDS_DSP_KADD8(q31_t x, q31_t y)
{
    union32_t ret, tx, ty;

    tx.w0 = x;
    ty.w0 = y;

    ret.b8.b0 = NDS_DSP_SATS((q15_t)tx.b8.b0 + ty.b8.b0, 8);
    ret.b8.b1 = NDS_DSP_SATS((q15_t)tx.b8.b1 + ty.b8.b1, 8);
    ret.b8.b2 = NDS_DSP_SATS((q15_t)tx.b8.b2 + ty.b8.b2, 8);
    ret.b8.b3 = NDS_DSP_SATS((q15_t)tx.b8.b3 + ty.b8.b3, 8);

    return ret.w0;
}

DSP_FUNC_PREF q31_t NDS_DSP_KSUB8(q31_t x, q31_t y)
{
    union32_t ret, tx, ty;

    tx.w0 = x;
    ty.w0 = y;

    ret.b8.b0 = NDS_DSP_SATS((q15_t)tx.b8.b0 - ty.b8.b0, 8);
    ret.b8.b1 = NDS_DSP_SATS((q15_t)tx.b8.b1 - ty.b8.b1, 8);
    ret.b8.b2 = NDS_DSP_SATS((q15_t)tx.b8.b2 - ty.b8.b2, 8);
    ret.b8.b3 = NDS_DSP_SATS((q15_t)tx.b8.b3 - ty.b8.b3, 8);

    return ret.w0;
}

DSP_FUNC_PREF q31_t NDS_DSP_ADD16(q31_t x, q31_t y)
{
    union32_t ret, tx, ty;

    tx.w0 = x;
    ty.w0 = y;

    ret.b16.h0 = tx.b16.h0 + ty.b16.h0;
    ret.b16.h1 = tx.b16.h1 + ty.b16.h1;

    return ret.w0;
}
// DSP_FUNC_PREF uint32_t NDS_DSP_UADD16(uint32_t x, uint32_t y)
// {
//     union32u_t ret, tx, ty;

//     tx.w0 = x;
//     ty.w0 = y;

//     ret.b16.h0 = tx.b16.h0 + ty.b16.h0;
//     ret.b16.h1 = tx.b16.h1 + ty.b16.h1;

//     return ret.w0;
// }

//Unsigned Reducing-half ADD two 16-bit half words.
DSP_FUNC_PREF uint32_t NDS_DSP_URADD16(uint32_t x, uint32_t y)
{
    union32u_t ret, tx, ty;

    tx.w0 = x;
    ty.w0 = y;

    ret.b16.h0 = (uint16_t)(((uint32_t)tx.b16.h0 + ty.b16.h0) >> 1);
    ret.b16.h1 = (uint16_t)(((uint32_t)tx.b16.h1 + ty.b16.h1) >> 1);

    return ret.w0;
}

//Unsigned Saturating ADD two 16-bit half words.
DSP_FUNC_PREF uint32_t NDS_DSP_UKADD16(uint32_t x, uint32_t y)
{
    union32u_t ret, tx, ty;

    tx.w0 = x;
    ty.w0 = y;

    ret.b16.h0 = (uint16_t)NDS_DSP_SAT((uint32_t)tx.b16.h0 + ty.b16.h0, 16);
    ret.b16.h1 = (uint16_t)NDS_DSP_SAT((uint32_t)tx.b16.h1 + ty.b16.h1, 16);

    return ret.w0;
}

DSP_FUNC_PREF q31_t NDS_DSP_SUB16(q31_t x, q31_t y)
{
    union32_t ret, tx, ty;

    tx.w0 = x;
    ty.w0 = y;

    ret.b16.h0 = tx.b16.h0 - ty.b16.h0;
    ret.b16.h1 = tx.b16.h1 - ty.b16.h1;

    return ret.w0;
}
// DSP_FUNC_PREF uint32_t NDS_DSP_USUB16(uint32_t x, uint32_t y)
// {
//     union32u_t ret, tx, ty;

//     tx.w0 = x;
//     ty.w0 = y;

//     ret.b16.h0 = tx.b16.h0 - ty.b16.h0;
//     ret.b16.h1 = tx.b16.h1 - ty.b16.h1;

//     return ret.w0;
// }
//Unsigned Reducing-half SUB two 16-bit half words.
DSP_FUNC_PREF uint32_t NDS_DSP_URSUB16(uint32_t x, uint32_t y)
{
    union32u_t ret, tx, ty;

    tx.w0 = x;
    ty.w0 = y;

    ret.b16.h0 = (uint16_t)(((uint32_t)tx.b16.h0 - ty.b16.h0) >> 1);
    ret.b16.h1 = (uint16_t)(((uint32_t)tx.b16.h1 - ty.b16.h1) >> 1);

    return ret.w0;
}
//Unsigned Saturating SUB two 16-bit half words.
DSP_FUNC_PREF uint32_t NDS_DSP_UKSUB16(uint32_t x, uint32_t y)
{
    union32u_t ret, tx, ty;

    tx.w0 = x;
    ty.w0 = y;

    ret.b16.h0 = (uint16_t)NDS_DSP_SAT((uint32_t)tx.b16.h0 - ty.b16.h0, 16);
    ret.b16.h1 = (uint16_t)NDS_DSP_SAT((uint32_t)tx.b16.h1 - ty.b16.h1, 16);

    return ret.w0;
}
DSP_FUNC_PREF q31_t NDS_DSP_CRAS16(q31_t x, q31_t y)
{
    // union32_t ret;
    // ret.b16.h0 = (q15_t)x - (q15_t)(y >> 16);
    // ret.b16.h1 = (q15_t)((x >> 16) + (q15_t)y);
    // return ret.w0;

    union32_t ret, tx, ty;

    tx.w0 = x;
    ty.w0 = y;

    ret.b16.h1 = tx.b16.h1 + ty.b16.h0;
    ret.b16.h0 = tx.b16.h0 - ty.b16.h1;
    return ret.w0;

}
// DSP_FUNC_PREF uint32_t NDS_DSP_UCRAS16(uint32_t x, uint32_t y)
// {
//     union32u_t ret;
//     ret.b16.h0 = (uint16_t)x - (uint16_t)(y >> 16);
//     ret.b16.h1 = (uint16_t)((x >> 16) + (uint16_t)y);
//     return ret.w0;
// }

DSP_FUNC_PREF uint32_t NDS_DSP_URCRAS16(uint32_t x, uint32_t y)
{
    // union32u_t ret;
    // ret.b16.h0 = ((uint32_t)((uint16_t)x) - (y >> 16)) >> 1;
    // ret.b16.h1 = ((x >> 16) + (uint16_t)y) >> 1;
    // return ret.w0;

    union32u_t ret, tx, ty;

    tx.w0 = x;
    ty.w0 = y;
    ret.b16.h1 = (uint16_t)(((uint32_t)tx.b16.h1 + ty.b16.h0) >> 1);
    ret.b16.h0 = (uint16_t)(((uint32_t)tx.b16.h0 - ty.b16.h1) >> 1);
    return ret.w0;

}

DSP_FUNC_PREF uint32_t NDS_DSP_UKCRAS16(uint32_t x, uint32_t y)
{
    // union32u_t ret;
    // ret.b16.h0 = (uint16_t)NDS_DSP_SAT(((uint32_t)((uint16_t)x)) - (y >> 16), 16);
    // ret.b16.h1 = (uint16_t)NDS_DSP_SAT((x >> 16) + (uint16_t)y, 16);
    // return ret.w0;

    union32u_t ret, tx, ty;

    tx.w0 = x;
    ty.w0 = y;
    ret.b16.h1 = (uint16_t)NDS_DSP_SAT((uint32_t)tx.b16.h1 + ty.b16.h0, 16);
    ret.b16.h0 = (uint16_t)NDS_DSP_SAT((uint32_t)tx.b16.h0 - ty.b16.h1, 16);
    return ret.w0;
}

DSP_FUNC_PREF q31_t NDS_DSP_CRSA16(q31_t x, q31_t y)
{
    // union32_t ret;
    // ret.b16.h0 = (q15_t)x + (q15_t)(y >> 16);
    // ret.b16.h1 = (q15_t)((x >> 16) - (q15_t)y);
    // return ret.w0;

    union32_t ret, tx, ty;

    tx.w0 = x;
    ty.w0 = y;
    ret.b16.h1 = tx.b16.h1 - ty.b16.h0;
    ret.b16.h0 = tx.b16.h0 + ty.b16.h1;
    return ret.w0;
}

// DSP_FUNC_PREF uint32_t NDS_DSP_UCRSA16(uint32_t x, uint32_t y)
// {
//     union32u_t ret;
//     ret.b16.h0 = (uint16_t)x + (uint16_t)(y >> 16);
//     ret.b16.h1 = (uint16_t)((x >> 16) - (uint16_t)y);
//     return ret.w0;
// }


DSP_FUNC_PREF uint32_t NDS_DSP_URCRSA16(uint32_t x, uint32_t y)
{
    // union32u_t ret;
    // ret.b16.h0 = ((uint32_t)((uint16_t)x) + (y >> 16)) >> 1;
    // ret.b16.h1 = ((x >> 16) - (uint16_t)y) >> 1;
    // return ret.w0;

    union32u_t ret, tx, ty;

    tx.w0 = x;
    ty.w0 = y;
    ret.b16.h1 = (uint16_t)(((uint32_t)tx.b16.h1 - ty.b16.h0) >> 1);
    ret.b16.h0 = (uint16_t)(((uint32_t)tx.b16.h0 + ty.b16.h1) >> 1);
    return ret.w0;
}

DSP_FUNC_PREF uint32_t NDS_DSP_UKCRSA16(uint32_t x, uint32_t y)
{
    // union32u_t ret;
    // ret.b16.h0 = (uint16_t)NDS_DSP_SAT(((uint32_t)((uint16_t)x)) + (y >> 16), 16);
    // ret.b16.h1 = (uint16_t)NDS_DSP_SAT(((x >> 16) - (uint16_t)y), 16);
    // return ret.w0;

    union32u_t ret, tx, ty;

    tx.w0 = x;
    ty.w0 = y;
    ret.b16.h1 = (uint16_t)NDS_DSP_SAT(((uint32_t)tx.b16.h1 - ty.b16.h0), 16);
    ret.b16.h0 = (uint16_t)NDS_DSP_SAT(((uint32_t)tx.b16.h0 + ty.b16.h1), 16);
    return ret.w0;
}

DSP_FUNC_PREF q31_t NDS_DSP_ADD8(q31_t x, q31_t y)
{
    union32_t ret;
    union32_t tx;// = (union32_t)x;
    union32_t ty;// = (union32_t)y;
    tx.w0 = x;
    ty.w0 = y;

    ret.b8.b0 = tx.b8.b0 + ty.b8.b0;
    ret.b8.b1 = tx.b8.b1 + ty.b8.b1;
    ret.b8.b2 = tx.b8.b2 + ty.b8.b2;
    ret.b8.b3 = tx.b8.b3 + ty.b8.b3;

    return ret.w0;
}

DSP_FUNC_PREF q31_t NDS_DSP_RADD8(q31_t x, q31_t y)
{
    union32_t ret;
    union32_t tx;// = (union32_t)x;
    union32_t ty;// = (union32_t)y;
    tx.w0 = x;
    ty.w0 = y;

    ret.b8.b0 = (q7_t)(((q15_t)tx.b8.b0 + ty.b8.b0) >> 1);
    ret.b8.b1 = (q7_t)(((q15_t)tx.b8.b1 + ty.b8.b1) >> 1);
    ret.b8.b2 = (q7_t)(((q15_t)tx.b8.b2 + ty.b8.b2) >> 1);
    ret.b8.b3 = (q7_t)(((q15_t)tx.b8.b3 + ty.b8.b3) >> 1);

    return ret.w0;
}

// DSP_FUNC_PREF uint32_t NDS_DSP_UADD8(uint32_t x, uint32_t y)
// {
//     union32u_t ret;
//     union32u_t tx = (union32u_t) x;
//     union32u_t ty = (union32u_t) y;

//     ret.b8.b0 = tx.b8.b0 + ty.b8.b0;
//     ret.b8.b1 = tx.b8.b1 + ty.b8.b1;
//     ret.b8.b2 = tx.b8.b2 + ty.b8.b2;
//     ret.b8.b3 = tx.b8.b3 + ty.b8.b3;

//     return ret.w0;
// }

DSP_FUNC_PREF uint32_t NDS_DSP_URADD8(uint32_t x, uint32_t y)
{
    union32u_t ret;
    union32u_t tx;// = (union32u_t)x;
    union32u_t ty;// = (union32u_t)y;
    tx.w0 = x;
    ty.w0 = y;

    ret.b8.b0 = (uint8_t)(((uint16_t)tx.b8.b0 + ty.b8.b0) >> 1);
    ret.b8.b1 = (uint8_t)(((uint16_t)tx.b8.b1 + ty.b8.b1) >> 1);
    ret.b8.b2 = (uint8_t)(((uint16_t)tx.b8.b2 + ty.b8.b2) >> 1);
    ret.b8.b3 = (uint8_t)(((uint16_t)tx.b8.b3 + ty.b8.b3) >> 1);

    return ret.w0;
}

DSP_FUNC_PREF uint32_t NDS_DSP_UKADD8(uint32_t x, uint32_t y)
{
    union32u_t ret;
    union32u_t tx;// = (union32u_t)x;
    union32u_t ty;// = (union32u_t)y;
    tx.w0 = x;
    ty.w0 = y;

    ret.b8.b0 = (uint8_t)NDS_DSP_SAT((uint32_t)tx.b8.b0 + ty.b8.b0, 8);
    ret.b8.b1 = (uint8_t)NDS_DSP_SAT((uint32_t)tx.b8.b1 + ty.b8.b1, 8);
    ret.b8.b2 = (uint8_t)NDS_DSP_SAT((uint32_t)tx.b8.b2 + ty.b8.b2, 8);
    ret.b8.b3 = (uint8_t)NDS_DSP_SAT((uint32_t)tx.b8.b3 + ty.b8.b3, 8);

    return ret.w0;
}


DSP_FUNC_PREF q31_t NDS_DSP_SUB8(q31_t x, q31_t y)
{
    union32_t ret;
    union32_t tx;// = (union32_t)x;
    union32_t ty;// = (union32_t)y;
    tx.w0 = x;
    ty.w0 = y;

    ret.b8.b0 = tx.b8.b0 - ty.b8.b0;
    ret.b8.b1 = tx.b8.b1 - ty.b8.b1;
    ret.b8.b2 = tx.b8.b2 - ty.b8.b2;
    ret.b8.b3 = tx.b8.b3 - ty.b8.b3;

    return ret.w0;
}

DSP_FUNC_PREF q31_t NDS_DSP_SRSUB8(q31_t x, q31_t y)
{
    union32_t ret;
    union32_t tx;// = (union32_t)x;
    union32_t ty;// = (union32_t)y;
    tx.w0 = x;
    ty.w0 = y;

    ret.b8.b0 = (q7_t)(((q15_t)tx.b8.b0 - ty.b8.b0) >> 1);
    ret.b8.b1 = (q7_t)(((q15_t)tx.b8.b1 - ty.b8.b1) >> 1);
    ret.b8.b2 = (q7_t)(((q15_t)tx.b8.b2 - ty.b8.b2) >> 1);
    ret.b8.b3 = (q7_t)(((q15_t)tx.b8.b3 - ty.b8.b3) >> 1);

    return ret.w0;
}

// DSP_FUNC_PREF uint32_t NDS_DSP_USUB8(uint32_t x, uint32_t y)
// {
//     union32u_t ret;
//     union32u_t tx = (union32u_t) x;
//     union32u_t ty = (union32u_t) y;

//     ret.b8.b0 = tx.b8.b0 - ty.b8.b0;
//     ret.b8.b1 = tx.b8.b1 - ty.b8.b1;
//     ret.b8.b2 = tx.b8.b2 - ty.b8.b2;
//     ret.b8.b3 = tx.b8.b3 - ty.b8.b3;

//     return ret.w0;
// }

DSP_FUNC_PREF uint32_t NDS_DSP_URSUB8(uint32_t x, uint32_t y)
{
    union32u_t ret;
    union32u_t tx;// = (union32u_t)x;
    union32u_t ty;// = (union32u_t)y;
    tx.w0 = x;
    ty.w0 = y;

    ret.b8.b0 = (q7_t)(((uint16_t)tx.b8.b0 - ty.b8.b0) >> 1);
    ret.b8.b1 = (q7_t)(((uint16_t)tx.b8.b1 - ty.b8.b1) >> 1);
    ret.b8.b2 = (q7_t)(((uint16_t)tx.b8.b2 - ty.b8.b2) >> 1);
    ret.b8.b3 = (q7_t)(((uint16_t)tx.b8.b3 - ty.b8.b3) >> 1);

    return ret.w0;
}

DSP_FUNC_PREF uint32_t NDS_DSP_UKSUB8(uint32_t x, uint32_t y)
{
    union32u_t ret;
    union32u_t tx;// = (union32u_t)x;
    union32u_t ty;// = (union32u_t)y;
    tx.w0 = x;
    ty.w0 = y;

    ret.b8.b0 = (uint8_t)NDS_DSP_SAT((uint16_t)tx.b8.b0 - ty.b8.b0, 8);
    ret.b8.b1 = (uint8_t)NDS_DSP_SAT((uint16_t)tx.b8.b1 - ty.b8.b1, 8);
    ret.b8.b2 = (uint8_t)NDS_DSP_SAT((uint16_t)tx.b8.b2 - ty.b8.b2, 8);
    ret.b8.b3 = (uint8_t)NDS_DSP_SAT((uint16_t)tx.b8.b3 - ty.b8.b3, 8);

    return ret.w0;
}

//Msw MULtiply two words.
DSP_FUNC_PREF q31_t NDS_DSP_SMMUL(q31_t x, q31_t y)
{
    union64_t ret;
    ret.d0 = (q63_t)x * y;
    return ret.b32.w1;
}

//Msw MULtiply two words with Rounding up.
DSP_FUNC_PREF q31_t NDS_DSP_SMMUL_U(q31_t x, q31_t y)
{
    union64_t ret;
    ret.d0 = ((q63_t)x * y);
    if ((ret.b32.w0 >> 31) != 0)
    {
        ret.b32.w1 += 1;
    }

    return ret.b32.w1;
}

//32=K(32+(32*32)>>32)
DSP_FUNC_PREF q31_t NDS_DSP_KMMAC(q31_t acc, q31_t x, q31_t y)
{
    union64_t ret, temp;
    temp.d0 = (q63_t)x * y;
    ret.d0 = (q63_t)acc + temp.b32.w1;
    return clip_q63_to_q31(ret.d0);
}

DSP_FUNC_PREF q31_t NDS_DSP_KMMSB(q31_t acc, q31_t x, q31_t y)
{
    union64_t ret, temp;
    temp.d0 = (q63_t)x * y;
    ret.d0 = (q63_t)acc - temp.b32.w1;
    return clip_q63_to_q31(ret.d0);
}

DSP_FUNC_PREF q31_t NDS_DSP_KMMAC_U(q31_t acc, q31_t x, q31_t y)
{
    union64_t ret, temp;
    temp.d0 = (q63_t)x * y;
    if ((temp.b32.w0 & ((q31_t)1 << 31)) != 0)
    {
        temp.b32.w1 += 1;
    }
    ret.d0 = (q63_t)acc + temp.b32.w1;
    return clip_q63_to_q31(ret.d0);
}

DSP_FUNC_PREF q31_t NDS_DSP_KMMSB_U(q31_t acc, q31_t x, q31_t y)
{
    union64_t ret, temp;
    temp.d0 = (q63_t)x * y;
    if ((temp.b32.w0 & ((q31_t)1 << 31)) != 0)
    {
        temp.b32.w1 += 1;
    }
    ret.d0 = (q63_t)acc - temp.b32.w1;
    return clip_q63_to_q31(ret.d0);
}

//32=sat((32*32)<<1), set ov
DSP_FUNC_PREF q31_t NDS_DSP_KWMMUL(q31_t x, q31_t y)
{
    q31_t ret;
    union64_t temp;
    if ((x != (q31_t)0x80000000) || (y != (q31_t)0x80000000))
    {
        temp.d0 = ((q63_t)x * y) << 1;
        ret = temp.b32.w1;
    }
    else
    {
        //sat(0x80000000*0x80000000*2) = 0x7FFFFFFF
        riscv_dsp_dsp_ov_flag = 1;
        ret = (q31_t)0x7FFFFFFF;
    }
    return ret;
}

//32=sat((RU(32*32, 30))<<1), set ov
DSP_FUNC_PREF q31_t NDS_DSP_KWMMUL_U(q31_t x, q31_t y)
{
    q31_t ret;
    union64_t temp;
    if ((x != (q31_t)0x80000000) || (y != (q31_t)0x80000000))
    {
        //Passed: (q63_t)0x80000000*0x7FFFFFFF+0x40000000) << 1 = 0x80000001 80000000
        temp.d0 = (((q63_t)x * y) + (q31_t)0x40000000) << 1;
        ret = temp.b32.w1;
    }
    else
    {
        //sat((0x80000000*0x80000000+0x40000000)*2) = 0x7FFFFFFF
        riscv_dsp_dsp_ov_flag = 1;
        ret = (q31_t)0x7FFFFFFF;
    }
    return ret;
}

//32=16*16
//Rt = Ra[15:0] * Rb[15:0];
DSP_FUNC_PREF q31_t NDS_DSP_SMBB(q31_t x, q31_t y)
{
    q31_t ret;
    union32_t tx, ty;

    tx.w0 = x;
    ty.w0 = y;

    ret = (q31_t)tx.b16.h0 * ty.b16.h0;

    return ret;
}

//Rt = Ra[15:0] * Rb[31:16];
DSP_FUNC_PREF q31_t NDS_DSP_SMBT(q31_t x, q31_t y)
{
    q31_t ret;
    union32_t tx, ty;

    tx.w0 = x;
    ty.w0 = y;

    ret = (q31_t)tx.b16.h0 * ty.b16.h1;

    return ret;
}

//Rt = Ra[31:16] * Rb[31:16];
DSP_FUNC_PREF q31_t NDS_DSP_SMTT(q31_t x, q31_t y)
{
    q31_t ret;
    union32_t tx, ty;

    tx.w0 = x;
    ty.w0 = y;

    ret = (q31_t)tx.b16.h1 * ty.b16.h1;

    return ret;
}

//32+=16*16
DSP_FUNC_PREF q31_t NDS_DSP_KMABB(q31_t acc, q31_t x, q31_t y)
{
    q31_t ret;
    union32_t tx, ty;

    tx.w0 = x;
    ty.w0 = y;

    ret = (q31_t)tx.b16.h0 * ty.b16.h0;
    ret = NDS_DSP_KADDW(ret, acc);

    return ret;
}

DSP_FUNC_PREF q31_t NDS_DSP_KMABT(q31_t acc, q31_t x, q31_t y)
{
    q31_t ret;
    union32_t tx, ty;

    tx.w0 = x;
    ty.w0 = y;

    ret = (q31_t)tx.b16.h0 * ty.b16.h1;
    ret = NDS_DSP_KADDW(ret, acc);

    return ret;
}

DSP_FUNC_PREF q31_t NDS_DSP_KMATT(q31_t acc, q31_t x, q31_t y)
{
    q31_t ret;
    union32_t tx, ty;

    tx.w0 = x;
    ty.w0 = y;

    ret = (q31_t)tx.b16.h1 * ty.b16.h1;
    ret = NDS_DSP_KADDW(ret, acc);

    return ret;
}

//32=16*16+16*16
//Rt = (Ra[31:16] * Rb[31:16]) + (Ra[15:0] * Rb[15:0]);
DSP_FUNC_PREF q31_t NDS_DSP_KMDA(q31_t x, q31_t y)
{
    q31_t ret, ta, tb;
    union32_t tx, ty;

    tx.w0 = x;
    ty.w0 = y;

    P16_2(x, y);

    ta = (q31_t)tx.b16.h0 * ty.b16.h0;
    tb = (q31_t)tx.b16.h1 * ty.b16.h1;
    ret = NDS_DSP_KADDW(ta, tb);

    return ret;
}

//Rt = (Ra[31:16] * Rb[15:0]) + (Ra[15:0] * Rb[31:16]);
DSP_FUNC_PREF q31_t NDS_DSP_KMXDA(q31_t x, q31_t y)
{
    q31_t ret, ta, tb;
    union32_t tx, ty;

    tx.w0 = x;
    ty.w0 = y;

    P16_2(x, y);

    ta = (q31_t)tx.b16.h0 * ty.b16.h1;
    tb = (q31_t)tx.b16.h1 * ty.b16.h0;
    ret = NDS_DSP_KADDW(ta, tb);

    return ret;
}

//32=16*16-16*16
//Rt = (Ra[31:16] * Rb[31:16]) - (Ra[15:0] * Rb[15:0]);
DSP_FUNC_PREF q31_t NDS_DSP_SMDS(q31_t x, q31_t y)
{
    q31_t ret, ta, tb;
    union32_t tx, ty;

    tx.w0 = x;
    ty.w0 = y;

    ta = (q31_t)tx.b16.h0 * ty.b16.h0;
    tb = (q31_t)tx.b16.h1 * ty.b16.h1;
    ret = tb - ta;      //Never overflow.

    return ret;
}

//32=16*16-16*16
//Rt = (Ra[15:0] * Rb[15:0]) - (Ra[31:16] * Rb[31:16]);
DSP_FUNC_PREF q31_t NDS_DSP_SMDRS(q31_t x, q31_t y)
{
    q31_t ret, ta, tb;
    union32_t tx, ty;

    tx.w0 = x;
    ty.w0 = y;

    P16_2(x, y);

    ta = (q31_t)tx.b16.h0 * ty.b16.h0;
    tb = (q31_t)tx.b16.h1 * ty.b16.h1;
    ret = ta - tb;      //Never overflow.

    return ret;
}

//Rt = (Ra[31:16] * Rb[15:0]) - (Ra[15:0] * Rb[31:16]);
DSP_FUNC_PREF q31_t NDS_DSP_SMXDS(q31_t x, q31_t y)
{
    // q31_t ret, ta, tb;
    // union32_t tx, ty;

    // tx.w0 = x;
    // ty.w0 = y;

    // ta = (q31_t)tx.b16.h0 * ty.b16.h1;
    // tb = (q31_t)tx.b16.h1 * ty.b16.h0;
    // ret = ta - tb;      //Never overflow.

    // return ret;

    q31_t ret, ta, tb;
    union32_t tx, ty;

    tx.w0 = x;
    ty.w0 = y;

    P16_2(x, y);

    ta = (q31_t)tx.b16.h1 * ty.b16.h0;
    tb = (q31_t)tx.b16.h0 * ty.b16.h1;
    ret = ta - tb;      //Never overflow.

    return ret;

}

//32=32+16*16+16*16
//res = Rt + (Ra[31:16] * Rb[31:16]) + (Ra[15:0] * Rb[15:0]);
DSP_FUNC_PREF q31_t NDS_DSP_KMADA(q31_t acc, q31_t x, q31_t y)
{
    q31_t ret, ta, tb;
    union32_t tx, ty;
    q63_t temp = (q63_t)acc;

    tx.w0 = x;
    ty.w0 = y;

    ta = (q31_t)tx.b16.h0 * ty.b16.h0;
    tb = (q31_t)tx.b16.h1 * ty.b16.h1;
    temp += (q63_t)ta + tb;
    ret = clip_q63_to_q31(temp);

    return ret;
}

//This is correct and spec1040119 is not correct
//All oprations need be fused and do saturation at final
//32=32-(16*16+16*16)
//Rt = Rt - ((Ra[31:16] * Rb[31:16]) + (Ra[15:0] * Rb[15:0]));
DSP_FUNC_PREF q31_t NDS_DSP_KMSDA(q31_t acc, q31_t x, q31_t y)
{
    q31_t ret, ta, tb;
    union32_t tx, ty;
    q63_t temp = (q63_t)acc;

    tx.w0 = x;
    ty.w0 = y;

    ta = (q31_t)tx.b16.h0 * ty.b16.h0;
    tb = (q31_t)tx.b16.h1 * ty.b16.h1;
    temp -= (q63_t)tb + ta;
    ret = clip_q63_to_q31(temp);

    return ret;
}


//32=32-(16*16+16*16)
//Rt = Rt - ((Ra[31:16] * Rb[15:0]) + (Ra[15:0] * Rb[31:16]));
DSP_FUNC_PREF q31_t NDS_DSP_KMSXDA(q31_t acc, q31_t x, q31_t y)
{
    q31_t ret, ta, tb;
    union32_t tx, ty;
    q63_t temp = (q63_t)acc;

    tx.w0 = x;
    ty.w0 = y;

    ta = (q31_t)tx.b16.h0 * ty.b16.h1;
    tb = (q31_t)tx.b16.h1 * ty.b16.h0;
    temp -= (q63_t)tb + ta;
    ret = clip_q63_to_q31(temp);

    return ret;
}

//Rt = Rt + (Ra[31:16] * Rb[15:0]) + (Ra[15:0] * Rb[31:16]);
DSP_FUNC_PREF q31_t NDS_DSP_KMAXDA(q31_t acc, q31_t x, q31_t y)
{
    q31_t ret, ta, tb;
    union32_t tx, ty;
    q63_t temp = (q63_t)acc;

    tx.w0 = x;
    ty.w0 = y;

    ta = (q31_t)tx.b16.h0 * ty.b16.h1;
    tb = (q31_t)tx.b16.h1 * ty.b16.h0;
    temp += (q63_t)tb + ta;
    ret = clip_q63_to_q31(temp);

    return ret;
}

//32=32+16*16-16*16
//Rt = Rt + (Ra[31:16] * Rb[31:16]) - (Ra[15:0] * Rb[15:0]);
DSP_FUNC_PREF q31_t NDS_DSP_KMADS(q31_t acc, q31_t x, q31_t y)
{
    q31_t ret, ta, tb;
    union32_t tx, ty;

    tx.w0 = x;
    ty.w0 = y;

    ta = (q31_t)tx.b16.h0 * ty.b16.h0;
    tb = (q31_t)tx.b16.h1 * ty.b16.h1;
    ret = tb - ta;      //Never overflow
    ret = NDS_DSP_KADDW(acc, ret);

    return ret;
}

//Rt = Rt + (Ra[15:0] * Rb[15:0]) - (Ra[31:16] * Rb[31:16]);
DSP_FUNC_PREF q31_t NDS_DSP_KMADRS(q31_t acc, q31_t x, q31_t y)
{
    q31_t ret, ta, tb;
    union32_t tx, ty;

    tx.w0 = x;
    ty.w0 = y;

    ta = (q31_t)tx.b16.h0 * ty.b16.h0;
    tb = (q31_t)tx.b16.h1 * ty.b16.h1;
    ret = ta - tb;      //Never overflow
    ret = NDS_DSP_KADDW(acc, ret);

    return ret;
}

//Rt = Rt + (Ra[31:16] * Rb[15:0]) - (Ra[15:0] * Rb[31:16]);
DSP_FUNC_PREF q31_t NDS_DSP_KMAXDS(q31_t acc, q31_t x, q31_t y)
{
    q31_t ret, ta, tb;
    union32_t tx, ty;

    tx.w0 = x;
    ty.w0 = y;

    ta = (q31_t)tx.b16.h0 * ty.b16.h1;
    tb = (q31_t)tx.b16.h1 * ty.b16.h0;
    ret = tb - ta;      //Never overflow
    ret = NDS_DSP_KADDW(acc, ret);

    return ret;
}

//32=(32*16)>>16
//Mres[47:0] = Ra * Rb[15:0];
DSP_FUNC_PREF q31_t NDS_DSP_SMMWB(q31_t x, q31_t y)
{
    union64_t ret;
    union32_t ty;

    ty.w0 = y;
    ret.d0 = (q63_t)x * ty.b16.h0;

    return (q31_t)(ret.d0 >> 16);
}

//32=R(32*16,15)>>16
DSP_FUNC_PREF q31_t NDS_DSP_SMMWB_U(q31_t x, q31_t y)
{
    union64_t ret;
    union32_t ty;
    q31_t rnd_val;

    ty.w0 = y;
    ret.d0 = (q63_t)x * ty.b16.h0;
    rnd_val = (ret.b32.w0 & (1UL << 15)) ? (1 << 16) : 0;
    ret.d0 += rnd_val;

    return (q31_t)(ret.d0 >> 16);
}

//32=(32*16)>>16
//Mres[47:0] = Ra * Rb[31:16];
DSP_FUNC_PREF q31_t NDS_DSP_SMMWT(q31_t x, q31_t y)
{
    union64_t ret;
    union32_t ty;

    ty.w0 = y;

    ret.d0 = (q63_t)x * ty.b16.h1;

    return (q31_t)(ret.d0 >> 16);
}

//32=R(32*16,15)>>16
DSP_FUNC_PREF q31_t NDS_DSP_SMMWT_U(q31_t x, q31_t y)
{
    union64_t ret;
    union32_t ty;
    q31_t rnd_val;

    ty.w0 = y;
    ret.d0 = (q63_t)x * ty.b16.h1;
    rnd_val = (ret.b32.w0 & (1UL << 15)) ? (1 << 16) : 0;
    ret.d0 += rnd_val;
    return (q31_t)(ret.d0 >> 16);
}

//32=K(32+(32*16)>>16)
//Rt = Rt + (Ra * Rb[15:0]) >> 16;
DSP_FUNC_PREF q31_t NDS_DSP_KMMAWB(q31_t acc, q31_t x, q31_t y)
{
    q31_t ret;
    union64_t ta;
    union32_t ty;

    ty.w0 = y;

    ta.d0 = ((q63_t)x * ty.b16.h0) >> 16;
    ret = NDS_DSP_KADDW((q31_t)ta.b32.w0, acc);

    return ret;
}

//32=K(32+R(32*16,15)>>16)
DSP_FUNC_PREF q31_t NDS_DSP_KMMAWB_U(q31_t acc, q31_t x, q31_t y)
{
    q31_t ret, rnd_val;
    union64_t ta;
    union32_t ty;

    ty.w0 = y;
    ta.d0 = (q63_t)x * ty.b16.h0;
    rnd_val = (ta.b32.w0 & (1UL << 15)) ? (1L << 16) : 0;
    ta.d0 += rnd_val;
    ret = NDS_DSP_KADDW((q31_t)(ta.d0 >> 16), acc);

    return ret;
}

//32=K(32+(32*16)>>16)
DSP_FUNC_PREF q31_t NDS_DSP_KMMAWT(q31_t acc, q31_t x, q31_t y)
{
    q31_t ret;
    union64_t ta;
    union32_t ty;

    ty.w0 = y;
    ta.d0 = (q63_t)x * ty.b16.h1;
    ret = NDS_DSP_KADDW((q31_t)(ta.d0 >> 16), acc);

    return ret;
}

//32=K(32+R(32*16,15)>>16)
DSP_FUNC_PREF q31_t NDS_DSP_KMMAWT_U(q31_t acc, q31_t x, q31_t y)
{
    q31_t ret, rnd_val;
    union64_t ta;
    union32_t ty;

    ty.w0 = y;
    ta.d0 = (q63_t)x * ty.b16.h1;
    rnd_val = (ta.b32.w0 & (1UL << 15)) ? (1L << 16) : 0;
    ta.d0 += rnd_val;
    ret = NDS_DSP_KADDW((q31_t)(ta.d0 >> 16), acc);

    return ret;
}

//64+=16*16
//64 = 64 + Ra[15:0] * Rb[15:0]
DSP_FUNC_PREF q63_t NDS_DSP_SMALBB(q63_t acc, q31_t x, q31_t y)
{
    union32_t tx, ty;
    q63_t ret = acc;

    tx.w0 = x;
    ty.w0 = y;

    ret += (q31_t)tx.b16.h0 * ty.b16.h0;

    return ret;
}

//64 = 64 + Ra[15:0] * Rb[31:16];
DSP_FUNC_PREF q63_t NDS_DSP_SMALBT(q63_t acc, q31_t x, q31_t y)
{
    union32_t tx, ty;
    q63_t ret = acc;

    tx.w0 = x;
    ty.w0 = y;

    ret += (q31_t)tx.b16.h0 * ty.b16.h1;

    return ret;
}

//64 = 64 + Ra[31:16] * Rb[31:16];
DSP_FUNC_PREF q63_t NDS_DSP_SMALTT(q63_t acc, q31_t x, q31_t y)
{
    union32_t tx, ty;
    q63_t ret = acc;

    tx.w0 = x;
    ty.w0 = y;

    ret += (q31_t)tx.b16.h1 * ty.b16.h1;

    return ret;
}

//64 = 64 + (Ra[31:16] * Rb[31:16]) + (Ra[15:0] * Rb[15:0]);
DSP_FUNC_PREF q63_t NDS_DSP_SMALDA(q63_t acc, q31_t x, q31_t y)
{
    union32_t tx, ty;
    q63_t ret = acc;

    tx.w0 = x;
    ty.w0 = y;

    P16_2(x, y);

    ret += (q31_t)tx.b16.h1 * ty.b16.h1;
    ret += (q31_t)tx.b16.h0 * ty.b16.h0;

    return ret;
}

//64 = 64 + (Ra[31:16] * Rb[15:0]) + (Ra[15:0] * Rb[31:16]);
DSP_FUNC_PREF q63_t NDS_DSP_SMALXDA(q63_t acc, q31_t x, q31_t y)
{
    union32_t tx, ty;
    q63_t ret = acc;

    tx.w0 = x;
    ty.w0 = y;

    ret += (q31_t)tx.b16.h1 * ty.b16.h0;
    ret += (q31_t)tx.b16.h0 * ty.b16.h1;

    return ret;
}

//64+=(16*16-16*16)
//64 = 64 + ((Ra[31:16] * Rb[31:16]) - (Ra[15:0] * Rb[15:0]));
DSP_FUNC_PREF q63_t NDS_DSP_SMALDS(q63_t acc, q31_t x, q31_t y)
{
    union32_t tx, ty;
    q31_t temp1, temp2;
    q63_t ret = acc;

    tx.w0 = x;
    ty.w0 = y;

    temp1 = (q31_t)tx.b16.h1 * ty.b16.h1;
    temp2 = (q31_t)tx.b16.h0 * ty.b16.h0;
    ret += temp1 - temp2;

    return ret;
}

//64+=(16*16-16*16)
//64 = 64 + (Ra[15:0] * Rb[15:0]) - (Ra[31:16] * Rb[31:16]);
DSP_FUNC_PREF q63_t NDS_DSP_SMALDRS(q63_t acc, q31_t x, q31_t y)
{
    union32_t tx, ty;
    q31_t temp1, temp2;
    q63_t ret = acc;

    tx.w0 = x;
    ty.w0 = y;

    temp1 = (q31_t)tx.b16.h0 * ty.b16.h0;
    temp2 = (q31_t)tx.b16.h1 * ty.b16.h1;
    ret += temp1 - temp2;

    return ret;
}

//64=64-(16*16+16*16)
//64 = 64 - ((Ra[31:16] * Rb[31:16]) + (Ra[15:0] * Rb[15:0]));
DSP_FUNC_PREF q63_t NDS_DSP_SMSLDA(q63_t acc, q31_t x, q31_t y)
{
    union32_t tx, ty;
    q31_t temp1, temp2;
    q63_t ret = acc;

    tx.w0 = x;
    ty.w0 = y;

    temp1 = (q31_t)tx.b16.h1 * ty.b16.h1;
    temp2 = (q31_t)tx.b16.h0 * ty.b16.h0;
    ret -= temp1 + temp2;

    return ret;
}

//64=64-(16*16+16*16)
//64 = 64 - ((Ra[31:16] * Rb[15:0]) + (Ra[15:0] * Rb[31:16]));
DSP_FUNC_PREF q63_t NDS_DSP_SMSLXDA(q63_t acc, q31_t x, q31_t y)
{
    union32_t tx, ty;
    q31_t temp1, temp2;
    q63_t ret = acc;

    tx.w0 = x;
    ty.w0 = y;

    temp1 = (q31_t)tx.b16.h1 * ty.b16.h0;
    temp2 = (q31_t)tx.b16.h0 * ty.b16.h1;
    ret -= temp1 + temp2;

    return ret;
}

//64 = 64 + (Ra[31:16] * Rb[15:0]) - (Ra[15:0] * Rb[31:16]);
DSP_FUNC_PREF q63_t NDS_DSP_SMALXDS(q63_t acc, q31_t x, q31_t y)
{
    union32_t tx, ty;
    q31_t temp1, temp2;
    q63_t ret = acc;

    tx.w0 = x;
    ty.w0 = y;

    temp1 = (q31_t)tx.b16.h1 * ty.b16.h0;
    temp2 = (q31_t)tx.b16.h0 * ty.b16.h1;
    ret += temp1 - temp2;

    return ret;
}

// DSP_FUNC_PREF q63_t NDS_DSP_MDALU(q63_t acc, q31_t x, q31_t y)
// {
//     q63_t ret;
//     union64_t tz;

//     tz.d0 = acc;

//     ret = (q63_t)x * y;
//     ret += tz.b32.w1;
//     ret += tz.b32.w0;

//     return ret;
// }

//U64=U64+U32*U32
DSP_FUNC_PREF uint64_t NDS_DSP_UMAR64(uint64_t acc, uint32_t x, uint32_t y)
{
    uint64_t ret = acc;
    ret += (uint64_t)x * y;
    return ret;
}

//64=64+32*32
DSP_FUNC_PREF q63_t NDS_DSP_SMAR64(q63_t acc, q31_t x, q31_t y)
{
    q63_t ret = acc;
    ret += (q63_t)x * y;
    return ret;
}

//U64=U64-U32*U32
DSP_FUNC_PREF uint64_t NDS_DSP_UMSR64(uint64_t acc, uint32_t x, uint32_t y)
{
    uint64_t ret = acc;
    ret -= (uint64_t)x * y;
    return ret;
}

//64=64-32*32
DSP_FUNC_PREF q63_t NDS_DSP_SMSR64(q63_t acc, q31_t x, q31_t y)
{
    q63_t ret;
    ret = acc - (q63_t)x * y;
    return ret;
}

//U16=U16>>imm;U16=U16>>imm;
DSP_FUNC_PREF q31_t NDS_DSP_SRLI16(q31_t x, IMM uint8_t sh)
{
    union32u_t ret, temp;

    temp.w0 = x;
    ret.b16.h0 = temp.b16.h0 >> sh;
    ret.b16.h1 = temp.b16.h1 >> sh;

    return (q31_t)ret.w0;
}

//U16=R(U16>>imm);U16=R(U16>>imm);
DSP_FUNC_PREF q31_t NDS_DSP_SRLI16_U(q31_t x, IMM uint8_t sh)
{
    union32u_t ret, temp;
    int rnd_val;
    uint32_t rnd_mask = (1UL << (sh - 1));

    temp.w0 = x;

    rnd_val = temp.b16.h0 & rnd_mask ? 1 : 0;
    ret.b16.h0 = (temp.b16.h0 >> sh) + rnd_val;

    rnd_val = temp.b16.h1 & rnd_mask ? 1 : 0;
    ret.b16.h1 = (temp.b16.h1 >> sh) + rnd_val;

    return (q31_t)ret.w0;
}

//U16=U16>>sh;U16=U16>>sh;
DSP_FUNC_PREF q31_t NDS_DSP_SRL16(q31_t x, uint8_t sh)
{
    union32u_t ret, temp;

    temp.w0 = x;
    ret.b16.h0 = temp.b16.h0 >> sh;
    ret.b16.h1 = temp.b16.h1 >> sh;

    return (q31_t)ret.w0;
}

//U16=R(U16>>)sh;U16=R(U16>>sh);
DSP_FUNC_PREF q31_t NDS_DSP_SRL16_U(q31_t x, uint8_t sh)
{
    union32u_t ret, temp;
    int rnd_val;
    uint32_t rnd_mask = (1UL << (sh - 1));

    temp.w0 = x;

    rnd_val = temp.b16.h0 & rnd_mask ? 1 : 0;
    ret.b16.h0 = (temp.b16.h0 >> sh) + rnd_val;

    rnd_val = temp.b16.h1 & rnd_mask ? 1 : 0;
    ret.b16.h1 = (temp.b16.h1 >> sh) + rnd_val;

    return (q31_t)ret.w0;
}

//16=16>>imm;16=16>>imm;
DSP_FUNC_PREF q31_t NDS_DSP_SRAI16(q31_t x, IMM uint8_t sh)
{
    union32_t ret, temp;

    temp.w0 = x;
    ret.b16.h0 = temp.b16.h0 >> sh;
    ret.b16.h1 = temp.b16.h1 >> sh;

    return ret.w0;
}

//Rounding Shift Right Arithmatic Immediate 16
//16=R(16>>imm);16=R(16>>imm);
DSP_FUNC_PREF q31_t NDS_DSP_SRAI16_U(q31_t x, IMM uint8_t sh)
{
    union32_t ret, temp;
    q15_t rnd_val;
    uint32_t rnd_mask = (1UL << (sh - 1));

    temp.w0 = x;

    rnd_val = temp.b16.h0 & rnd_mask ? 1 : 0;
    ret.b16.h0 = (temp.b16.h0 >> sh) + rnd_val;

    rnd_val = temp.b16.h1 & rnd_mask ? 1 : 0;
    ret.b16.h1 = (temp.b16.h1 >> sh) + rnd_val;

    return ret.w0;
}

//16=16>>sh;16=16>>sh;
DSP_FUNC_PREF q31_t NDS_DSP_SRA16(q31_t x, uint8_t sh)
{
    union32_t ret, temp;

    temp.w0 = x;
    ret.b16.h0 = temp.b16.h0 >> sh;
    ret.b16.h1 = temp.b16.h1 >> sh;

    return ret.w0;
}

//16=R(16>>sh);16=R(16>>sh);
DSP_FUNC_PREF q31_t NDS_DSP_SRA16_U(q31_t x, uint8_t sh)
{
    union32_t ret, temp;
    q15_t rnd_val;
    uint32_t rnd_mask = (1UL << (sh - 1));

    temp.w0 = x;

    rnd_val = temp.b16.h0 & rnd_mask ? 1 : 0;
    ret.b16.h0 = (temp.b16.h0 >> sh) + rnd_val;

    rnd_val = temp.b16.h1 & rnd_mask ? 1 : 0;
    ret.b16.h1 = (temp.b16.h1 >> sh) + rnd_val;

    return ret.w0;
}

//16=16<<imm;16=16<<imm;
DSP_FUNC_PREF q31_t NDS_DSP_SLLI16(q31_t x, IMM uint8_t sh)
{
    union32_t ret, temp;

    temp.w0 = x;
    ret.b16.h0 = temp.b16.h0 << sh;
    ret.b16.h1 = temp.b16.h1 << sh;

    return (q31_t)ret.w0;
}

//16=16<<sh;16=16<<sh;  //sh[3:0]
DSP_FUNC_PREF q31_t NDS_DSP_SLL16(q31_t x, uint8_t sh)
{
    union32_t ret, temp;

    temp.w0 = x;
    ret.b16.h0 = temp.b16.h0 << sh;
    ret.b16.h1 = temp.b16.h1 << sh;

    return (q31_t)ret.w0;
}

//four 8-bit shifts
// DSP_FUNC_PREF q31_t NDS_DSP_SRLI8(q31_t x, IMM uint8_t sh)
// {
//     union32u_t ret, temp;

//     temp.w0 = x;
//     ret.b8.b0 = temp.b8.b0 >> sh;
//     ret.b8.b1 = temp.b8.b1 >> sh;
//     ret.b8.b2 = temp.b8.b2 >> sh;
//     ret.b8.b3 = temp.b8.b3 >> sh;

//     return (q31_t)ret.w0;
// }

// DSP_FUNC_PREF q31_t NDS_DSP_SRAI8(q31_t x, IMM uint8_t sh)
// {
//     union32_t ret, temp;

//     temp.w0 = x;
//     ret.b8.b0 = temp.b8.b0 >> sh;
//     ret.b8.b1 = temp.b8.b1 >> sh;
//     ret.b8.b2 = temp.b8.b2 >> sh;
//     ret.b8.b3 = temp.b8.b3 >> sh;

//     return ret.w0;
// }

// DSP_FUNC_PREF q31_t NDS_DSP_SLLI8(q31_t x, IMM uint8_t sh)
// {
//     union32u_t ret, temp;

//     temp.w0 = x;
//     ret.b8.b0 = (uint8_t)NDS_DSP_SAT(((uint32_t)temp.b8.b0) << sh, 8);
//     ret.b8.b1 = (uint8_t)NDS_DSP_SAT(((uint32_t)temp.b8.b1) << sh, 8);
//     ret.b8.b2 = (uint8_t)NDS_DSP_SAT(((uint32_t)temp.b8.b2) << sh, 8);
//     ret.b8.b3 = (uint8_t)NDS_DSP_SAT(((uint32_t)temp.b8.b3) << sh, 8);

//     return (q31_t)ret.w0;
// }

//Rt[31:0] = CONCAT(Ra[31:16], Rb[31:16]);
DSP_FUNC_PREF q31_t NDS_DSP_PKTT16(q31_t x, q31_t y)
{
    union32_t ret, temp;

    temp.w0 = x;
    ret.b16.h1 = temp.b16.h1;
    temp.w0 = y;
    ret.b16.h0 = temp.b16.h1;

    return ret.w0;
}

//Rt[31:0] = CONCAT(Ra[15:0], Rb[15:0]);
DSP_FUNC_PREF q31_t NDS_DSP_PKBB16(q31_t x, q31_t y)
{
    union32_t ret, temp;

    P16_2(x, y);

    temp.w0 = x;
    ret.b16.h1 = temp.b16.h0;
    temp.w0 = y;
    ret.b16.h0 = temp.b16.h0;

    return ret.w0;
}

//Rt[31:0] = CONCAT(Ra[15:0], Rb[31:16]);
DSP_FUNC_PREF q31_t NDS_DSP_PKBT16(q31_t x, q31_t y)
{
    union32_t ret, temp;

    temp.w0 = x;
    ret.b16.h1 = temp.b16.h0;
    temp.w0 = y;
    ret.b16.h0 = temp.b16.h1;

    return ret.w0;
}

//Rt[31:0] = CONCAT(Ra[31:16], Rb[15:0]);
DSP_FUNC_PREF q31_t NDS_DSP_PKTB16(q31_t x, q31_t y)
{
    union32_t ret, temp;

    temp.w0 = x;
    ret.b16.h1 = temp.b16.h1;
    temp.w0 = y;
    ret.b16.h0 = temp.b16.h0;

    return ret.w0;
}


//Unpacking
// DSP_FUNC_PREF q31_t NDS_DSP_SEB1(q31_t x)
// {
//     union32_t ret;

//     ret.w0 = x;
//     return (q31_t)ret.b8.b1;
// }

// DSP_FUNC_PREF q31_t NDS_DSP_SEB2(q31_t x)
// {
//     union32_t ret;

//     ret.w0 = x;
//     return (q31_t)ret.b8.b2;
// }

// DSP_FUNC_PREF q31_t NDS_DSP_SEB3(q31_t x)
// {
//     union32_t ret;

//     ret.w0 = x;
//     return (q31_t)ret.b8.b3;
// }

// DSP_FUNC_PREF uint32_t NDS_DSP_ZEB1(uint32_t x)
// {
//     union32u_t ret;

//     ret.w0 = x;
//     return (uint32_t)ret.b8.b1;
// }

// DSP_FUNC_PREF uint32_t NDS_DSP_ZEB2(uint32_t x)
// {
//     union32u_t ret;

//     ret.w0 = x;
//     return (uint32_t)ret.b8.b2;
// }

// DSP_FUNC_PREF uint32_t NDS_DSP_ZEB3(uint32_t x)
// {
//     union32u_t ret;

//     ret.w0 = x;
//     return (uint32_t)ret.b8.b3;
// }

//two 16bit to two 32bit
// DSP_FUNC_PREF void NDS_DSP_SEDH(q31_t x, q31_t *v0, q31_t *v1)
// {
//     union32_t temp;

//     temp.w0 = x;
//     *v0 = (q31_t)temp.b16.h0;
//     *v1 = (q31_t)temp.b16.h1;
// }

// DSP_FUNC_PREF void NDS_DSP_ZEDH(uint32_t x, uint32_t *v0, uint32_t *v1)
// {
//     union32u_t temp;

//     temp.w0 = x;
//     *v0 = (uint32_t)temp.b16.h0;
//     *v1 = (uint32_t)temp.b16.h1;
// }

//Choose tow 8-bit from a word and save to two 16-bit
//16=(32<<8)>>24;16=(32<<24)>>24
//Rt[31:16] = CONCAT(8`bRa[23], Ra[23:16]);
//Rt[15:0]  = CONCAT(8`bRa[7], Ra[7:0]);
DSP_FUNC_PREF q31_t NDS_DSP_SUNPKD820(q31_t x)
{
    union32_t ret, temp;

    temp.w0 = x;
    ret.b16.h1 = (q15_t)temp.b8.b2;
    ret.b16.h0 = (q15_t)temp.b8.b0;

    return ret.w0;
}

//16=32>>24;16=(32<<16)>>24
DSP_FUNC_PREF q31_t NDS_DSP_SUNPKD831(q31_t x)
{
    union32_t ret, temp;

    temp.w0 = x;
    ret.b16.h0 = (q15_t)temp.b8.b1;
    ret.b16.h1 = (q15_t)temp.b8.b3;

    return ret.w0;
}

//16=(32<<16)>>24;16=(32<<24)>>24
DSP_FUNC_PREF q31_t NDS_DSP_SUNPKD810(q31_t x)
{
    union32_t ret, temp;

    temp.w0 = x;
    ret.b16.h1 = (q15_t)temp.b8.b1;
    ret.b16.h0 = (q15_t)temp.b8.b0;

    return ret.w0;
}

//16=32>>24;16=(32<<24)>>24
//Rt[31:16] = CONCAT(8`bRa[31], Ra[31:24]);
//Rt[15:0]  = CONCAT(8`bRa[7], Ra[7:0]);
DSP_FUNC_PREF q31_t NDS_DSP_SUNPKD830(q31_t x)
{
    union32_t ret, temp;

    temp.w0 = x;
    ret.b16.h1 = (q15_t)temp.b8.b3;
    ret.b16.h0 = (q15_t)temp.b8.b0;

    return ret.w0;
}

// DSP_FUNC_PREF q31_t NDS_DSP_SEDB2(q31_t x)
// {
//     union32_t ret, temp;

//     temp.w0 = x;
//     ret.b16.h0 = (q15_t)temp.b8.b2;
//     ret.b16.h1 = (q15_t)temp.b8.b0;

//     return ret.w0;
// }

// DSP_FUNC_PREF q31_t NDS_DSP_SEDB3(q31_t x)
// {
//     union32_t ret, temp;

//     temp.w0 = x;
//     ret.b16.h0 = (q15_t)temp.b8.b3;
//     ret.b16.h1 = (q15_t)temp.b8.b1;

//     return ret.w0;
// }

//16=(U32<<8)>>24;16=(U32<<24)>>24
DSP_FUNC_PREF uint32_t NDS_DSP_ZUNPKD820(uint32_t x)
{
    union32u_t ret, temp;

    temp.w0 = x;
    ret.b16.h1 = (uint16_t)temp.b8.b2;
    ret.b16.h0 = (uint16_t)temp.b8.b0;

    return ret.w0;
}

//16=U32>>24;16=(U32<<16)>>24
DSP_FUNC_PREF uint32_t NDS_DSP_ZUNPKD831(uint32_t x)
{
    union32u_t ret, temp;

    temp.w0 = x;
    ret.b16.h1 = (uint16_t)temp.b8.b3;
    ret.b16.h0 = (uint16_t)temp.b8.b1;

    return ret.w0;
}

//16=(U32<<16)>>24;16=U32>>24
DSP_FUNC_PREF uint32_t NDS_DSP_ZUNPKD810(uint32_t x)
{
    union32u_t ret, temp;

    temp.w0 = x;
    ret.b16.h1 = (uint16_t)temp.b8.b1;
    ret.b16.h0 = (uint16_t)temp.b8.b0;

    return ret.w0;
}

//16=U32>>24;16=(U32<<24)>>24
DSP_FUNC_PREF uint32_t NDS_DSP_ZUNPKD830(uint32_t x)
{
    union32u_t ret, temp;

    temp.w0 = x;
    ret.b16.h1 = (uint16_t)temp.b8.b3;
    ret.b16.h0 = (uint16_t)temp.b8.b0;

    return ret.w0;
}

// DSP_FUNC_PREF uint32_t NDS_DSP_ZEDB2(uint32_t x)
// {
//     union32u_t ret, temp;

//     temp.w0 = x;
//     ret.b16.h0 = (uint16_t)temp.b8.b2;
//     ret.b16.h1 = (uint16_t)temp.b8.b0;

//     return ret.w0;
// }

// DSP_FUNC_PREF uint32_t NDS_DSP_ZEDB3(uint32_t x)
// {
//     union32u_t ret, temp;

//     temp.w0 = x;
//     ret.b16.h0 = (uint16_t)temp.b8.b3;
//     ret.b16.h1 = (uint16_t)temp.b8.b1;

//     return ret.w0;
// }

//BIT REVerse and adjust to bit_size width from lsb.
//bit_size can be set 2 to 32.
DSP_FUNC_PREF uint32_t NDS_DSP_BITREV(uint32_t x, uint8_t bit_size)
{
    x = (((x & 0xaaaaaaaa) >> 1) | ((x & 0x55555555) << 1));
    x = (((x & 0xcccccccc) >> 2) | ((x & 0x33333333) << 2));
    x = (((x & 0xf0f0f0f0) >> 4) | ((x & 0x0f0f0f0f) << 4));
    x = (((x & 0xff00ff00) >> 8) | ((x & 0x00ff00ff) << 8));
    x = ((x >> 16) | (x << 16));
    return x >> (32 - bit_size);
}

//BIT REVerse Immediate and adjust to bit_size width from lsb.
//bit_size is immediate and can be set 2 to 32.
DSP_FUNC_PREF uint32_t NDS_DSP_BITREVI(uint32_t x, IMM uint8_t bit_size)
{
    return NDS_DSP_BITREV(x, bit_size);
}

//Rounding
// rnd_bit: bit 0 to bit 30.
// rnd_mode: immediate value from 0 to 3 (see roundMode_t)
// DSP_FUNC_PREF q31_t NDS_DSP_RNDW(q31_t x, uint8_t rnd_bit, IMM roundMode_t rnd_mode)
// {
//     q31_t ret = x;
//     q31_t sh = rnd_bit + 1;
//     q63_t rnd_val = 1 << rnd_bit;

//     if (rnd_bit > 30)
//     {
//         return ret;
//     }

//     switch (rnd_mode)
//     {
//     case round_up:
//         ret = clip_q63_to_q31((q63_t)ret + rnd_val);
//         ret = (ret >> sh) << sh;
//         break;
//     case round_near:
//         if (x >= 0)
//         {
//             ret = clip_q63_to_q31((q63_t)ret + rnd_val);
//         }
//         else
//         {
//             ret = clip_q63_to_q31((q63_t)ret - rnd_val);
//         }
//         ret = (ret >> sh) << sh;
//         break;
//     case round_zero:
//         if (x < 0)
//         {
//             ret = clip_q63_to_q31((q63_t)ret + rnd_val);
//         }
//         ret = (ret >> sh) << sh;
//         break;
//     default:     //round down
//         ret = (ret >> sh) << sh;
//         break;
//     }
//     return ret;
// }

//Rounding
// rnd_bit: bit 0 to bit 30.
// rnd_mode: from 0 to 3 (see roundMode_t)
// DSP_FUNC_PREF q31_t NDS_DSP_RNDWI(q31_t x, IMM uint8_t rnd_bit, IMM roundMode_t rnd_mode)
// {
//     return NDS_DSP_RNDW(x, rnd_bit, rnd_mode);
// }

// rnd_bit: bit 0 to bit 62.
// rnd_mode: from 0 to 3 (see roundMode_t)
// DSP_FUNC_PREF q63_t NDS_DSP_RNDDW(q63_t x, uint8_t rnd_bit, IMM roundMode_t rnd_mode)
// {
//     q63_t ret = x;
//     q31_t sh = rnd_bit + 1;
//     q63_t rnd_val = 1 << rnd_bit;
//     uint8_t orig_sgn;

//     if (rnd_bit > 62)
//     {
//         return ret;
//     }

//     //0 for positive, 1 for negative.
//     orig_sgn = (x < 0) ? 1 : 0;

//     switch (rnd_mode)
//     {
//     case round_up:
//         ret = ret + rnd_val;
//         if (orig_sgn ^ (ret < 0))
//         {
//             //overflow
//             riscv_dsp_dsp_ov_flag = 1;
//             ret = (orig_sgn == 1) ? INT64_MIN : INT64_MAX;
//         }
//         else
//         {
//             ret = (ret >> sh) << sh;
//         }
//         break;
//     case round_near:
//         if (orig_sgn == 0)
//         {
//             ret = ret + rnd_val;
//         }
//         else
//         {
//             ret = ret - rnd_val;
//         }

//         if (orig_sgn ^ (ret < 0))
//         {
//             //overflow
//             riscv_dsp_dsp_ov_flag = 1;
//             ret = (orig_sgn == 1) ? INT64_MIN : INT64_MAX;
//         }
//         else
//         {
//             ret = (ret >> sh) << sh;
//         }
//         break;
//     case round_zero:
//         if (orig_sgn == 0)
//         {
//             ret = (ret >> sh) << sh;
//         }
//         else
//         {
//             ret = ret + rnd_val;
//             if (orig_sgn ^ (ret < 0))
//             {
//                 //overflow
//                 riscv_dsp_dsp_ov_flag = 1;
//                 ret = (orig_sgn == 1) ? INT64_MIN : INT64_MAX;
//             }
//             else
//             {
//                 ret = (ret >> sh) << sh;
//             }
//         }
//         break;
//     default:    //round down
//         ret = (ret >> sh) << sh;
//         break;
//     }
//     return ret;
// }

// rnd_bit: bit 0 to bit 62.
// rnd_mode: from 0 to 3 (see roundMode_t)
// DSP_FUNC_PREF q63_t NDS_DSP_RNDDWI(q63_t x, IMM uint8_t rnd_bit, IMM roundMode_t rnd_mode)
// {
//     return NDS_DSP_RNDDW(x, rnd_bit, rnd_mode);
// }

//Unsigned clipping for one 32-bit word at bit_idx, if ov set ov_flag.
//bit_idx: from bit 0 to bit 31.
DSP_FUNC_PREF uint32_t NDS_DSP_UCLIP32(uint32_t x, IMM uint8_t bit_idx)
{
    return NDS_DSP_SAT(x, bit_idx + 1);
}


//Signed clipping for one 32-bit word at bit_idx, if ov set ov_flag.
//bit_idx: from bit 0 to bit 31.
DSP_FUNC_PREF q31_t NDS_DSP_SCLIP32(q31_t x, IMM uint8_t bit_idx)
{
    return NDS_DSP_SATS(x, bit_idx + 1);
}

//unsigned clipping for two 16 at bit bit_idx separately.
//bit_idx: from bit 0 to bit 15 for half-word.
DSP_FUNC_PREF uint32_t NDS_DSP_UCLIP16(uint32_t x, IMM uint8_t bit_idx)
{
    union32u_t ret;

    ret.w0 = x;
    ret.b16.h1 = (uint16_t)NDS_DSP_SAT(ret.b16.h1, bit_idx + 1);
    ret.b16.h0 = (uint16_t)NDS_DSP_SAT(ret.b16.h0, bit_idx + 1);

    return ret.w0;
}

//signed clipping for two 16 at bit bit_idx separately.
//bit_idx: from bit 0 to bit 15 for half-word.
DSP_FUNC_PREF q31_t NDS_DSP_SCLIP16(q31_t x, IMM uint8_t bit_idx)
{
    union32_t ret;

    ret.w0 = x;
    ret.b16.h1 = (q15_t)NDS_DSP_SATS(ret.b16.h1, bit_idx + 1);
    ret.b16.h0 = (q15_t)NDS_DSP_SATS(ret.b16.h0, bit_idx + 1);

    return ret.w0;
}

////unsigned clipping for two 16 at bit bit_idx separately.
////num_bit: from 1 to 16 bits for half-word.
//DSP_FUNC_PREF uint32_t NDS_DSP_SAT16(uint32_t x, IMM uint8_t num_bit)
//{
//    union32u_t ret;
//
//    ret.w0 = x;
//    ret.b16.h0 = (uint16_t)NDS_DSP_SAT(ret.b16.h0, num_bit);
//    ret.b16.h1 = (uint16_t)NDS_DSP_SAT(ret.b16.h1, num_bit);
//
//    return ret.w0;
//}
//
////signed clipping for two 16 at bit bit_idx separately.
////num_bit: from 1 to 16 bits for half-word.
//DSP_FUNC_PREF q31_t NDS_DSP_SATS16(q31_t x, IMM uint8_t num_bit)
//{
//    union32_t ret;
//
//    ret.w0 = x;
//    ret.b16.h0 = (q15_t)NDS_DSP_SATS(ret.b16.h0, num_bit);
//    ret.b16.h1 = (q15_t)NDS_DSP_SATS(ret.b16.h1, num_bit);
//
//    return ret.w0;
//}

//Signed CoMPare EQual to the corresponding four 8-bit between two operariscv_dsp.
//return: four 8-bit 0xFF/0x00 results. if eq then 0xFF else 0x00.
//Rt[31:24] = (Ra[31:24] == Rb[31:24])? 0xff : 0x0;
//Rt[23:16] = (Ra[23:16] == Rb[23:16])? 0xff : 0x0;
//Rt[15:8]  = (Ra[15:8]  == Rb[15:8]) ? 0xff : 0x0;
//Rt[7:0]   = (Ra[7:0]   == Rb[7:0])  ? 0xff : 0x0;
DSP_FUNC_PREF uint32_t NDS_DSP_CMPEQ8(q31_t x, q31_t y)
{
    const q7_t const_true = 0xFF;
    union32_t tx;// = (union32_t)x;
    union32_t ty;// = (union32_t)y;
    tx.w0 = x;
    ty.w0 = y;
    union32u_t ret;

    ret.w0 = 0;
    if (tx.b8.b3 == ty.b8.b3) ret.b8.b3 = const_true;
    if (tx.b8.b2 == ty.b8.b2) ret.b8.b2 = const_true;
    if (tx.b8.b1 == ty.b8.b1) ret.b8.b1 = const_true;
    if (tx.b8.b0 == ty.b8.b0) ret.b8.b0 = const_true;

    return ret.w0;
}

//Signed CoMPare EQual to the corresponding two 16-bit between two operariscv_dsp.
//return: two 16-bit 0xFFFF/0x0000 results. if eq then 0xFFFF else 0x0000.
//Rt[31:16] = (Ra[31:16] == Rb[31:16])? 0xffff : 0x0;
//Rt[15:0] = (Ra[15:0] == Rb[15:0]) ? 0xffff : 0x0;
DSP_FUNC_PREF uint32_t NDS_DSP_CMPEQ16(q31_t x, q31_t y)
{
    const q15_t const_true = 0xFFFF;
    union32_t tx;// = (union32_t)x;
    union32_t ty;// = (union32_t)y;
    tx.w0 = x;
    ty.w0 = y;
    union32u_t ret;

    ret.w0 = 0;
    if (tx.b16.h1 == ty.b16.h1) ret.b16.h1 = const_true;
    if (tx.b16.h0 == ty.b16.h0) ret.b16.h0 = const_true;

    return ret.w0;
}

//Unsigned CoMPare EQual to the corresponding four 8-bit between two operariscv_dsp.
//return: four 8-bit 0xFF/0x00 results. if eq then 0xFF else 0x00.
// DSP_FUNC_PREF uint32_t NDS_DSP_UCMPEQ8(uint32_t x, uint32_t y)
// {
//     const uint8_t const_true = 0xFF;
//     union32u_t tempx = (union32u_t) x;
//     union32u_t tempy = (union32u_t) y;
//     union32u_t ret;

//     ret.w0 = 0;
//     if (tempx.b8.b0 == tempy.b8.b0) ret.b8.b0 = const_true;
//     if (tempx.b8.b1 == tempy.b8.b1) ret.b8.b1 = const_true;
//     if (tempx.b8.b2 == tempy.b8.b2) ret.b8.b2 = const_true;
//     if (tempx.b8.b3 == tempy.b8.b3) ret.b8.b3 = const_true;

//     return ret.w0;
// }

//Unsigned CoMPare EQual to the corresponding two 16-bit between two operariscv_dsp.
//return: two 16-bit 0xFFFF/0x0000 results. if eq then 0xFFFF else 0x0000.
// DSP_FUNC_PREF uint32_t NDS_DSP_UCMPEQ16(uint32_t x, uint32_t y)
// {
//     const uint16_t const_true = 0xFFFF;
//     union32u_t tempx = (union32u_t) x;
//     union32u_t tempy = (union32u_t) y;
//     union32u_t ret;

//     ret.w0 = 0;
//     if (tempx.b16.h0 == tempy.b16.h0) ret.b16.h0 = const_true;
//     if (tempx.b16.h1 == tempy.b16.h1) ret.b16.h1 = const_true;

//     return ret.w0;
// }

//Signed CoMPare Less Than the corresponding four 8-bit between two operariscv_dsp.
//return: four 8-bit 0xFF/0x00 results. if LT then 0xFF else 0x00.
//Rt[31:24] = (Ra[31:24] < Rb[31:24])? 0xff : 0x0;
//Rt[23:16] = (Ra[23:16] < Rb[23:16])? 0xff : 0x0;
//Rt[15:8]  = (Ra[15:8]  < Rb[15:8]) ? 0xff : 0x0;
//Rt[7:0]   = (Ra[7:0]   < Rb[7:0])  ? 0xff : 0x0;
DSP_FUNC_PREF uint32_t NDS_DSP_SCMPLT8(q31_t x, q31_t y)
{
    const q7_t const_true = 0xFF;
    union32_t tx;// = (union32_t)x;
    union32_t ty;// = (union32_t)y;
    tx.w0 = x;
    ty.w0 = y;
    union32u_t ret;

    ret.w0 = 0;
    if (tx.b8.b3 < ty.b8.b3) ret.b8.b3 = const_true;
    if (tx.b8.b2 < ty.b8.b2) ret.b8.b2 = const_true;
    if (tx.b8.b1 < ty.b8.b1) ret.b8.b1 = const_true;
    if (tx.b8.b0 < ty.b8.b0) ret.b8.b0 = const_true;

    return ret.w0;
}

//Signed CoMPare Less Than the corresponding two 16-bit between two operariscv_dsp.
//return: two 16-bit 0xFFFF/0x0000 results. if LT then 0xFFFF else 0x0000.
DSP_FUNC_PREF uint32_t NDS_DSP_SCMPLT16(q31_t x, q31_t y)
{
    const q15_t const_true = 0xFFFF;
    union32_t tx;// = (union32_t)x;
    union32_t ty;// = (union32_t)y;
    tx.w0 = x;
    ty.w0 = y;
    union32u_t ret;

    ret.w0 = 0;
    if (tx.b16.h1 < ty.b16.h1) ret.b16.h1 = const_true;
    if (tx.b16.h0 < ty.b16.h0) ret.b16.h0 = const_true;

    return ret.w0;
}

//Unsigned CoMPare Less Than the corresponding four 8-bit between two operariscv_dsp.
//return: four 8-bit 0xFF/0x00 results. if LT then 0xFF else 0x00.
DSP_FUNC_PREF uint32_t NDS_DSP_UCMPLT8(uint32_t x, uint32_t y)
{
    const uint8_t const_true = 0xFF;
    union32u_t tx;// = (union32u_t)x;
    union32u_t ty;// = (union32u_t)y;
    tx.w0 = x;
    ty.w0 = y;
    union32u_t ret;

    ret.w0 = 0;
    if (tx.b8.b3 < ty.b8.b3) ret.b8.b3 = const_true;
    if (tx.b8.b2 < ty.b8.b2) ret.b8.b2 = const_true;
    if (tx.b8.b1 < ty.b8.b1) ret.b8.b1 = const_true;
    if (tx.b8.b0 < ty.b8.b0) ret.b8.b0 = const_true;

    return ret.w0;
}

//Unsigned CoMPare Less Than the corresponding two 16-bit between two operariscv_dsp.
//return: two 16-bit 0xFFFF/0x0000 results. if LT then 0xFFFF else 0x0000.
DSP_FUNC_PREF uint32_t NDS_DSP_UCMPLT16(uint32_t x, uint32_t y)
{
    const uint16_t const_true = 0xFFFF;
    union32u_t tx;// = (union32u_t)x;
    union32u_t ty;// = (union32u_t)y;
    tx.w0 = x;
    ty.w0 = y;
    union32u_t ret;

    ret.w0 = 0;
    if (tx.b16.h1 < ty.b16.h1) ret.b16.h1 = const_true;
    if (tx.b16.h0 < ty.b16.h0) ret.b16.h0 = const_true;

    return ret.w0;
}

//Signed CoMPare Less than or Equal to the corresponding four 8-bit between two operariscv_dsp.
//return: four 8-bit 0xFF/0x00 results. if LE then 0xFF else 0x00.
DSP_FUNC_PREF uint32_t NDS_DSP_SCMPLE8(q31_t x, q31_t y)
{
    const q7_t const_true = 0xFF;
    union32_t tx;// = (union32_t)x;
    union32_t ty;// = (union32_t)y;
    tx.w0 = x;
    ty.w0 = y;
    union32u_t ret;

    ret.w0 = 0;
    if (tx.b8.b3 <= ty.b8.b3) ret.b8.b3 = const_true;
    if (tx.b8.b2 <= ty.b8.b2) ret.b8.b2 = const_true;
    if (tx.b8.b1 <= ty.b8.b1) ret.b8.b1 = const_true;
    if (tx.b8.b0 <= ty.b8.b0) ret.b8.b0 = const_true;

    return ret.w0;
}

//Signed CoMPare Less than or Equal to the corresponding two 16-bit between two operariscv_dsp.
//return: two 16-bit 0xFFFF/0x0000 results. if LE then 0xFFFF else 0x0000.
DSP_FUNC_PREF uint32_t NDS_DSP_SCMPLE16(q31_t x, q31_t y)
{
    const q15_t const_true = 0xFFFF;
    union32_t tx;// = (union32_t)x;
    union32_t ty;// = (union32_t)y;
    tx.w0 = x;
    ty.w0 = y;
    union32u_t ret;

    ret.w0 = 0;
    if (tx.b16.h1 <= ty.b16.h1) ret.b16.h1 = const_true;
    if (tx.b16.h0 <= ty.b16.h0) ret.b16.h0 = const_true;

    return ret.w0;
}

//Unsigned CoMPare Less than or Equal to the corresponding four 8-bit between two operariscv_dsp.
//return: four 8-bit 0xFF/0x00 results. if LE then 0xFF else 0x00.
DSP_FUNC_PREF uint32_t NDS_DSP_UCMPLE8(uint32_t x, uint32_t y)
{
    const uint8_t const_true = 0xFF;
    union32u_t tx;// = (union32u_t)x;
    union32u_t ty;// = (union32u_t)y;
    tx.w0 = x;
    ty.w0 = y;
    union32u_t ret;

    ret.w0 = 0;
    if (tx.b8.b3 <= ty.b8.b3) ret.b8.b3 = const_true;
    if (tx.b8.b2 <= ty.b8.b2) ret.b8.b2 = const_true;
    if (tx.b8.b1 <= ty.b8.b1) ret.b8.b1 = const_true;
    if (tx.b8.b0 <= ty.b8.b0) ret.b8.b0 = const_true;

    return ret.w0;
}

//Unsigned CoMPare Less than or Equal to the corresponding two 16-bit between two operariscv_dsp.
//return: two 16-bit 0xFFFF/0x0000 results. if LE then 0xFFFF else 0x0000.
DSP_FUNC_PREF uint32_t NDS_DSP_UCMPLE16(uint32_t x, uint32_t y)
{
    const uint16_t const_true = 0xFFFF;
    union32u_t tx;// = (union32u_t)x;
    union32u_t ty;// = (union32u_t)y;
    tx.w0 = x;
    ty.w0 = y;
    union32u_t ret;

    ret.w0 = 0;
    if (tx.b16.h1 <= ty.b16.h1) ret.b16.h1 = const_true;
    if (tx.b16.h0 <= ty.b16.h0) ret.b16.h0 = const_true;

    return ret.w0;
}

//Pick True bit from x based on sel_ctl.
//return: x & sel_ctl
// DSP_FUNC_PREF uint32_t NDS_DSP_PICKT(q31_t x, uint32_t sel_ctl)
// {
//     return ((uint32_t)x & sel_ctl);
// }

//Pick False bit from x based on sel_ctl.
//return: x & (sel_ctl ^ 0xFFFFFFFF)
// DSP_FUNC_PREF uint32_t NDS_DSP_PICKF(q31_t x, uint32_t sel_ctl)
// {
//     uint32_t temp_ctl = sel_ctl ^ 0xFFFFFFFF;

//     return ((uint32_t)x & temp_ctl);
// }

//Bit Pick from x or y based on sel_ctl.
//return: pick bit from x if corresponding bit in sel_ctl is 1, otherwise from y.
DSP_FUNC_PREF uint32_t NDS_DSP_BPICK(q31_t x, q31_t y, uint32_t sel_ctl)
{
    uint32_t temp_ctl = ~sel_ctl;

    return (uint32_t)((x & sel_ctl) | (y & temp_ctl));
}

#include <assert.h>
//Insert byte o of y into one of 4 bytes of x
DSP_FUNC_PREF q31_t NDS_DSP_INSB(q31_t x, q31_t y, int sel)
{
    q31_t ret;
    q31_t mask_x = 0xFFFFFF00;  //default is byte 0
    q31_t temp_y = y;

    assert((sel >= 0) && (sel <= 3));

    if (sel == 1)
    {
        mask_x = 0xFFFF00FF;
        temp_y <<= 8;
    }
    else if (sel == 2)
    {
        mask_x = 0xFF00FFFF;
        temp_y <<= 16;
    }
    else if (sel == 3)
    {
        mask_x = 0x00FFFFFF;
        temp_y <<= 24;
    }
    ret = (x & mask_x) | (temp_y & (~mask_x));

    return ret;
}

// Rt[31:24] = (Ra[31:24] < Rb[31:24])? Ra[31:24] : Rb[31:24];
// Rt[23:16] = (Ra[23:16] < Rb[23:16])? Ra[23:16] : Rb[23:16];
// Rt[15:8]  = (Ra[15:8]  < Rb[15:8]) ? Ra[15:8]  : Rb[15:8];
// Rt[7:0]   = (Ra[7:0]   < Rb[7:0])  ? Ra[7:0]   : Rb[7:0];
DSP_FUNC_PREF q31_t NDS_DSP_SMIN8(q31_t x, q31_t y)
{
    union32_t tempx;// = (union32_t)x;
    union32_t tempy;// = (union32_t)y;
    tempx.w0 = x;
    tempy.w0 = y;
    union32_t ret;

    ret.b8.b3 = (tempx.b8.b3 < tempy.b8.b3) ? tempx.b8.b3 : tempy.b8.b3;
    ret.b8.b2 = (tempx.b8.b2 < tempy.b8.b2) ? tempx.b8.b2 : tempy.b8.b2;
    ret.b8.b1 = (tempx.b8.b1 < tempy.b8.b1) ? tempx.b8.b1 : tempy.b8.b1;
    ret.b8.b0 = (tempx.b8.b0 < tempy.b8.b0) ? tempx.b8.b0 : tempy.b8.b0;

    return ret.w0;
}

DSP_FUNC_PREF uint32_t NDS_DSP_UMIN8(uint32_t x, uint32_t y)
{
    union32u_t tempx;// = (union32_t)x;
    union32u_t tempy;// = (union32_t)y;
    tempx.w0 = x;
    tempy.w0 = y;
    union32u_t ret;

    ret.b8.b3 = (tempx.b8.b3 < tempy.b8.b3) ? tempx.b8.b3 : tempy.b8.b3;
    ret.b8.b2 = (tempx.b8.b2 < tempy.b8.b2) ? tempx.b8.b2 : tempy.b8.b2;
    ret.b8.b1 = (tempx.b8.b1 < tempy.b8.b1) ? tempx.b8.b1 : tempy.b8.b1;
    ret.b8.b0 = (tempx.b8.b0 < tempy.b8.b0) ? tempx.b8.b0 : tempy.b8.b0;

    return ret.w0;
}

// Rt[31:16] = (Ra[31:16] < Rb[31:16])? Ra[31:16] : Rb[31:16];
// Rt[15:0]  = (Ra[15:0]  < Rb[15:0]) ? Ra[15:0]  : Rb[15:0];
DSP_FUNC_PREF q31_t NDS_DSP_SMIN16(q31_t x, q31_t y)
{
    union32_t tempx;// = (union32_t)x;
    union32_t tempy;// = (union32_t)y;
    tempx.w0 = x;
    tempy.w0 = y;
    union32_t ret;

    ret.b16.h1 = (tempx.b16.h1 < tempy.b16.h1) ? tempx.b16.h1 : tempy.b16.h1;
    ret.b16.h0 = (tempx.b16.h0 < tempy.b16.h0) ? tempx.b16.h0 : tempy.b16.h0;

    return ret.w0;
}

DSP_FUNC_PREF uint32_t NDS_DSP_UMIN16(uint32_t x, uint32_t y)
{
    union32u_t tempx;// = (union32_t)x;
    union32u_t tempy;// = (union32_t)y;
    tempx.w0 = x;
    tempy.w0 = y;
    union32u_t ret;

    ret.b16.h1 = (tempx.b16.h1 < tempy.b16.h1) ? tempx.b16.h1 : tempy.b16.h1;
    ret.b16.h0 = (tempx.b16.h0 < tempy.b16.h0) ? tempx.b16.h0 : tempy.b16.h0;

    return ret.w0;
}

// Rt[31:24] = (Ra[31:24] > Rb[31:24])? Ra[31:24] : Rb[31:24];
// Rt[23:16] = (Ra[23:16] > Rb[23:16])? Ra[23:16] : Rb[23:16];
// Rt[15:8]  = (Ra[15:8]  > Rb[15:8]) ? Ra[15:8]  : Rb[15:8];
// Rt[7:0]   = (Ra[7:0]   > Rb[7:0])  ? Ra[7:0]   : Rb[7:0];
DSP_FUNC_PREF q31_t NDS_DSP_SMAX8(q31_t x, q31_t y)
{
    union32_t tempx;// = (union32_t)x;
    union32_t tempy;// = (union32_t)y;
    tempx.w0 = x;
    tempy.w0 = y;
    union32_t ret;

    ret.b8.b3 = (tempx.b8.b3 > tempy.b8.b3) ? tempx.b8.b3 : tempy.b8.b3;
    ret.b8.b2 = (tempx.b8.b2 > tempy.b8.b2) ? tempx.b8.b2 : tempy.b8.b2;
    ret.b8.b1 = (tempx.b8.b1 > tempy.b8.b1) ? tempx.b8.b1 : tempy.b8.b1;
    ret.b8.b0 = (tempx.b8.b0 > tempy.b8.b0) ? tempx.b8.b0 : tempy.b8.b0;

    return ret.w0;
}

DSP_FUNC_PREF uint32_t NDS_DSP_UMAX8(uint32_t x, uint32_t y)
{
    union32u_t tempx;// = (union32_t)x;
    union32u_t tempy;// = (union32_t)y;
    tempx.w0 = x;
    tempy.w0 = y;
    union32u_t ret;

    ret.b8.b3 = (tempx.b8.b3 > tempy.b8.b3) ? tempx.b8.b3 : tempy.b8.b3;
    ret.b8.b2 = (tempx.b8.b2 > tempy.b8.b2) ? tempx.b8.b2 : tempy.b8.b2;
    ret.b8.b1 = (tempx.b8.b1 > tempy.b8.b1) ? tempx.b8.b1 : tempy.b8.b1;
    ret.b8.b0 = (tempx.b8.b0 > tempy.b8.b0) ? tempx.b8.b0 : tempy.b8.b0;

    return ret.w0;
}

// Rt[31:16] = (Ra[31:16] > Rb[31:16])? Ra[31:16] : Rb[31:16];
// Rt[15:0] = (Ra[15:0] > Rb[15:0]) ? Ra[15:0] : Rb[15:0];
DSP_FUNC_PREF q31_t NDS_DSP_SMAX16(q31_t x, q31_t y)
{
    union32_t tempx;// = (union32_t)x;
    union32_t tempy;// = (union32_t)y;
    tempx.w0 = x;
    tempy.w0 = y;
    union32_t ret;

    ret.b16.h1 = (tempx.b16.h1 > tempy.b16.h1) ? tempx.b16.h1 : tempy.b16.h1;
    ret.b16.h0 = (tempx.b16.h0 > tempy.b16.h0) ? tempx.b16.h0 : tempy.b16.h0;

    return ret.w0;
}

DSP_FUNC_PREF uint32_t NDS_DSP_UMAX16(uint32_t x, uint32_t y)
{
    union32u_t tempx;// = (union32_t)x;
    union32u_t tempy;// = (union32_t)y;
    tempx.w0 = x;
    tempy.w0 = y;
    union32u_t ret;

    ret.b16.h1 = (tempx.b16.h1 > tempy.b16.h1) ? tempx.b16.h1 : tempy.b16.h1;
    ret.b16.h0 = (tempx.b16.h0 > tempy.b16.h0) ? tempx.b16.h0 : tempy.b16.h0;

    return ret.w0;
}

//16=K(16<<imm4u);16=K(16<<imm4u);
DSP_FUNC_PREF q31_t NDS_DSP_KSLLI16(q31_t x, IMM uint8_t sh)
{
    union32_t ret, temp;

    temp.w0 = x;
    ret.b16.h1 = (q15_t) NDS_DSP_SATS((q31_t)temp.b16.h1 << sh, 16);
    ret.b16.h0 = (q15_t) NDS_DSP_SATS((q31_t)temp.b16.h0 << sh, 16);

    return (q31_t)ret.w0;
}

//16=K(16<<sh;16=K(16<<sh);     //sh[3:0]
DSP_FUNC_PREF q31_t NDS_DSP_KSLL16(q31_t x, uint8_t sh)
{
    union32_t ret, temp;

    temp.w0 = x;
    ret.b16.h1 = (q15_t) NDS_DSP_SATS((q31_t)temp.b16.h1 << sh, 16);
    ret.b16.h0 = (q15_t) NDS_DSP_SATS((q31_t)temp.b16.h0 << sh, 16);

    return (q31_t)ret.w0;
}

DSP_FUNC_PREF q31_t NDS_DSP_KSLLI(q31_t x, IMM uint8_t sh)
{
    return clip_q63_to_q31((q63_t)x << sh);
}

DSP_FUNC_PREF q31_t NDS_DSP_KSLL(q31_t x, uint8_t sh)
{
    return clip_q63_to_q31((q63_t)x << sh);
}

//32=R(32>>imm)
DSP_FUNC_PREF q31_t NDS_DSP_SRAI_U(q31_t x, IMM uint8_t sh)
{
    q31_t rnd_val;
    uint32_t rnd_mask = (1UL << (sh - 1));

    rnd_val = x & rnd_mask ? 1 : 0;
    return (x >> sh) + rnd_val;
}

//32=R(32>>sh)
DSP_FUNC_PREF q31_t NDS_DSP_SRA_U(q31_t x, uint8_t sh)
{
    q31_t rnd_val;
    uint32_t rnd_mask = (1UL << (sh - 1));

    rnd_val = x & rnd_mask ? 1 : 0;
    return (x >> sh) + rnd_val;
}

DSP_FUNC_PREF q31_t NDS_DSP_KSLRA16(q31_t x, int sh)
{
    union32_t temp;
    temp.w0 = x;

    if (sh < 0 )
    {
        temp.b16.h0 >>= (-sh);
        temp.b16.h1 >>= (-sh);
    }
    else
    {
        temp.b16.h0 = NDS_DSP_SATS(((q31_t)temp.b16.h0) << sh, 16);
        temp.b16.h1 = NDS_DSP_SATS(((q31_t)temp.b16.h1) << sh, 16);
    }

    return temp.w0;
}

//with round
DSP_FUNC_PREF q31_t NDS_DSP_KSLRA16_U(q31_t x, int sh)
{
    union32_t temp;
    temp.w0 = x;

    if (sh < 0 )
    {
        q15_t val;
        int rnd;
        int new_sh = -sh;
        uint32_t mask_new_sh_1 = 1UL << (new_sh - 1);

        val = temp.b16.h0;
        rnd = (val & mask_new_sh_1) ? 1 : 0;
        val >>= new_sh;
        val += rnd;
        temp.b16.h0 = val;

        val = temp.b16.h1;
        rnd = (val & mask_new_sh_1) ? 1 : 0;
        val >>= new_sh;
        val += rnd;
        temp.b16.h1 = val;
    }
    else
    {
        temp.b16.h0 = NDS_DSP_SATS(((q31_t)temp.b16.h0) << sh, 16);
        temp.b16.h1 = NDS_DSP_SATS(((q31_t)temp.b16.h1) << sh, 16);
    }

    return temp.w0;
}

DSP_FUNC_PREF q31_t NDS_DSP_KSLRAW_U(q31_t x, int sh)
{
    q31_t ret;
    if (sh < 0 )
    {
        int rnd;
        int new_sh = -sh;
        uint32_t mask_new_sh_1 = 1UL << (new_sh - 1);

        rnd = (x & mask_new_sh_1) ? 1 : 0;
        ret = x >> new_sh;
        ret += rnd;
    }
    else
    {
        q63_t temp;

        temp = (q63_t)x << sh;
        ret = clip_q63_to_q31(temp);
    }

    return ret;
}

// DSP_FUNC_PREF q31_t NDS_DSP_KSLRA8(q31_t x, int sh)
// {
//     union32_t temp;
//     temp.w0 = x;

//     if (sh < 0 )
//     {
//         temp.b8.b0 >>= (-sh);
//         temp.b8.b1 >>= (-sh);
//         temp.b8.b2 >>= (-sh);
//         temp.b8.b3 >>= (-sh);
//     }
//     else
//     {
//         temp.b8.b0 = NDS_DSP_SATS(((q31_t)temp.b8.b0) << sh, 8);
//         temp.b8.b1 = NDS_DSP_SATS(((q31_t)temp.b8.b1) << sh, 8);
//         temp.b8.b2 = NDS_DSP_SATS(((q31_t)temp.b8.b2) << sh, 8);
//         temp.b8.b3 = NDS_DSP_SATS(((q31_t)temp.b8.b3) << sh, 8);
//     }

//     return temp.w0;
// }

//with round
// DSP_FUNC_PREF q31_t NDS_DSP_KSLRAR8(q31_t x, int sh)
// {
//     union32_t temp;
//     temp.w0 = x;

//     if (sh < 0 )
//     {
//         q15_t val;
//         int rnd;
//         int new_sh = -sh;
//         uint32_t mask_new_sh_1 = 1UL << (new_sh - 1);

//         val = temp.b8.b0;
//         rnd = (val & mask_new_sh_1) ? 1 : 0;
//         val >>= new_sh;
//         val += rnd;
//         temp.b8.b0 = val;

//         val = temp.b8.b1;
//         rnd = (val & mask_new_sh_1) ? 1 : 0;
//         val >>= new_sh;
//         val += rnd;
//         temp.b8.b1 = val;

//         val = temp.b8.b2;
//         rnd = (val & mask_new_sh_1) ? 1 : 0;
//         val >>= new_sh;
//         val += rnd;
//         temp.b8.b2 = val;

//         val = temp.b8.b3;
//         rnd = (val & mask_new_sh_1) ? 1 : 0;
//         val >>= new_sh;
//         val += rnd;
//         temp.b8.b3 = val;
//     }
//     else
//     {
//         temp.b8.b0 = NDS_DSP_SATS(((q31_t)temp.b8.b0) << sh, 8);
//         temp.b8.b1 = NDS_DSP_SATS(((q31_t)temp.b8.b1) << sh, 8);
//         temp.b8.b2 = NDS_DSP_SATS(((q31_t)temp.b8.b2) << sh, 8);
//         temp.b8.b3 = NDS_DSP_SATS(((q31_t)temp.b8.b3) << sh, 8);
//     }

//     return temp.w0;
// }

//32=abs(32)
DSP_FUNC_PREF q31_t NDS_DSP_KABS(q31_t x)
{
    if (x == (q31_t)0x80000000)
    {
        //set ov
        riscv_dsp_dsp_ov_flag = 1;
        return (q31_t)0x7FFFFFFF;
    }
    else
        return (x < 0) ? -x : x;
}

//16=abs(16);16=abs(16);
DSP_FUNC_PREF q31_t NDS_DSP_KABS16(q31_t x)
{
    union32_t temp;

    temp.w0 = x;
    temp.b16.h1 = (q15_t)(NDS_DSP_KABS(((q31_t)temp.b16.h1) << 16) >> 16);
    temp.b16.h0 = (q15_t)(NDS_DSP_KABS(((q31_t)temp.b16.h0) << 16) >> 16);

    return temp.w0;
}

//8=abs(8);8=abs(8);8=abs(8);8=abs(8);
DSP_FUNC_PREF q31_t NDS_DSP_KABS8(q31_t x)
{
    union32_t temp;

    temp.w0 = x;
    temp.b8.b3 = (q7_t)(NDS_DSP_KABS(((q31_t)temp.b8.b3) << 24) >> 24);
    temp.b8.b2 = (q7_t)(NDS_DSP_KABS(((q31_t)temp.b8.b2) << 24) >> 24);
    temp.b8.b1 = (q7_t)(NDS_DSP_KABS(((q31_t)temp.b8.b1) << 24) >> 24);
    temp.b8.b0 = (q7_t)(NDS_DSP_KABS(((q31_t)temp.b8.b0) << 24) >> 24);

    return temp.w0;
}

//16=K(16*16);16=K(16*16);
//Rt[31:16] = Ra[31:16] * Rb[31:16];
//Rt[15:0]  = Ra[15:0]  * Rb[15:0];
DSP_FUNC_PREF q31_t NDS_DSP_KHM16(q31_t x, q31_t y)
{
    union32_t ret;

    ret.b16.h0 = NDS_ISA_KHMBB(x, y);
    ret.b16.h1 = NDS_ISA_KHMTT(x, y);

    return ret.w0;
}

//16=K(16*16);16=K(16*16);
//Rt[31:16] = Ra[31:16] * Rb[15:0];
//Rt[15:0]  = Ra[15:0]  * Rb[31:16];
DSP_FUNC_PREF q31_t NDS_DSP_KHMX16(q31_t x, q31_t y)
{
    union32_t ret;

    ret.b16.h1 = NDS_ISA_KHMTB(x, y);
    ret.b16.h0 = NDS_ISA_KHMBT(x, y);

    return ret.w0;
}

//64+=(x[31..16]*x[15..0])
//Rt[63:0] = Ra[63:0] + Rb[31:16] * Rb[15:0];
DSP_FUNC_PREF q63_t NDS_DSP_SMAL(q63_t acc, q31_t x)
{
    union32_t tx;
    q63_t ret = acc;

    tx.w0 = x;
    ret += (q31_t)tx.b16.h0 * tx.b16.h1;

    return ret;
}

//Word EXTract out 32-bit from 64-bit, start from bit lsb_idx to bit (lsb_idx + 32 -1).
DSP_FUNC_PREF q31_t NDS_DSP_WEXT(q63_t x, uint8_t lsb_idx)
{
    return (q31_t)(((uint64_t)x) >> lsb_idx);
}

//Word EXTract out 32-bit from 64-bit, start from Immediate bit lsb_idx to bit (lsb_idx + 32 -1).
DSP_FUNC_PREF q31_t NDS_DSP_WEXTI(q63_t x, IMM uint8_t lsb_idx)
{
    return NDS_DSP_WEXT(x, lsb_idx);
}

// 64-bit operation
//64=64+64
DSP_FUNC_PREF q63_t NDS_DSP_ADD64(q63_t x, q63_t y)
{
    return (x + y);
}

//64=64-64
DSP_FUNC_PREF q63_t NDS_DSP_SUB64(q63_t x, q63_t y)
{
    return (x - y);
}

//64=(64+64)>>1
DSP_FUNC_PREF q63_t NDS_DSP_RADD64(q63_t x, q63_t y)
{
    q63_t ret;
    q31_t lsb_eq_1 = 1L;

    lsb_eq_1 &= x;
    lsb_eq_1 &= y;
    ret = ((x >> 1) + (y >> 1)) + (q63_t)lsb_eq_1;

    return ret;
}

//64=(64-64)>>1
DSP_FUNC_PREF q63_t NDS_DSP_RSUB64(q63_t x, q63_t y)
{
    q31_t high_x, high_y;
    q63_t high, ret;
    uint64_t low;

    high_x = x < 0 ? 0xffffffff : 0x0;
    high_y = y < 0 ? 0xffffffff : 0x0;
    high = high_x - high_y;
    low = ((uint64_t)x - y);

    if (low > (uint64_t)x)
        high --;

    ret = (low >> 1) | (high & 0x1 ? 0x8000000000000000 : 0x0);

    return ret;
}

//64=(U64+U64)>>1
DSP_FUNC_PREF uint64_t NDS_DSP_URADD64(uint64_t x, uint64_t y)
{
    uint64_t ret;
    uint32_t lsb_eq_1 = 1UL;

    lsb_eq_1 &= x;
    lsb_eq_1 &= y;
    ret = ((x >> 1) + (y >> 1)) + (uint64_t)lsb_eq_1;

    return ret;
}

//64=(U64-U64)>>1
DSP_FUNC_PREF uint64_t NDS_DSP_URSUB64(uint64_t x, uint64_t y)
{
    uint64_t ret;
    uint32_t high = 0;
    uint64_t low = x - y;

    if (low > x)
        high --;

    ret = (low >> 1) | (high & 0x1 ? 0x8000000000000000 : 0x0);

    return ret;
}

//64=K(64+64)
DSP_FUNC_PREF q63_t NDS_DSP_KADD64(q63_t x, q63_t y)
{
    q63_t ret = x + y;
    if ((x > 0) && (y > 0))
    {
        if (ret <= 0)
        {
            riscv_dsp_dsp_ov_flag = 1;
            ret = 0x7FFFFFFFFFFFFFFFLL;
        }
    }
    else if ((x < 0) && (y < 0))
    {
        if (ret >= 0)
        {
            riscv_dsp_dsp_ov_flag = 1;
            ret = 0x8000000000000000LL;
        }

    }

    return ret;
}

//64=K(64-64)
DSP_FUNC_PREF q63_t NDS_DSP_KSUB64(q63_t x, q63_t y)
{
    q63_t ret = x - y;
    if ((x > 0) && (y < 0))
    {
        if (ret <= 0)
        {
            riscv_dsp_dsp_ov_flag = 1;
            ret = 0x7FFFFFFFFFFFFFFFLL;
        }
    }
    else if ((x < 0) && (y > 0))
    {
        if (ret >= 0)
        {
            riscv_dsp_dsp_ov_flag = 1;
            ret = 0x8000000000000000LL;
        }

    }

    return ret;
}

//64=K(U64+U64)
DSP_FUNC_PREF uint64_t NDS_DSP_UKADD64(uint64_t x, uint64_t y)
{
    uint64_t ret = x + y;

    if (ret < x)
    {
        riscv_dsp_dsp_ov_flag = 1;
        ret = 0xFFFFFFFFFFFFFFFFLL;
    }

    return ret;
}

//64=K(U64-U64)
DSP_FUNC_PREF uint64_t NDS_DSP_UKSUB64(uint64_t x, uint64_t y)
{
    uint64_t ret = x - y;

    //if x < y, then result is negtive. but smallest of unsigned value is 0.
    if (x < y)
    {
        riscv_dsp_dsp_ov_flag = 1;
        ret = 0LL;
    }

    return ret;
}

//64=K(64+32*32)
DSP_FUNC_PREF q63_t NDS_DSP_KMAR64(q63_t acc, q31_t x, q31_t y)
{
    q63_t mul_val;

    mul_val = (q63_t)x * y;
    return NDS_DSP_KADD64(acc, mul_val);
}

//64=K(64-32*32)
DSP_FUNC_PREF q63_t NDS_DSP_KMSR64(q63_t acc, q31_t x, q31_t y)
{
    q63_t mul_val;

    mul_val = (q63_t)x * y;
    return NDS_DSP_KSUB64(acc, mul_val);
}

//64=UK(U64+U32*U32)
DSP_FUNC_PREF uint64_t NDS_DSP_UKMAR64(uint64_t acc, uint32_t x, uint32_t y)
{
    uint64_t mul_val;

    mul_val = (uint64_t)x * y;
    return NDS_DSP_UKADD64(acc, mul_val);
}

//64=UK(U64-U32*U32)
DSP_FUNC_PREF uint64_t NDS_DSP_UKMSR64(uint64_t acc, uint32_t x, uint32_t y)
{
    uint64_t mul_val;

    mul_val = (uint64_t)x * y;
    return NDS_DSP_UKSUB64(acc, mul_val);
}

#ifdef ENA_DSP_ISA2
DSP_FUNC_PREF q31_t NDS_DSP_KMMWB2(q31_t x, q31_t y)
{
    q31_t ret;
    union32_t ty;
    ty.w0 = y;

    if ((x == (q31_t)0x80000000) && (ty.b16.h0 == (q15_t)0x8000))
    {
        ret = (q31_t)0x7FFFFFFF;
#ifdef SIM_SAT_OV_FLAG
        riscv_dsp_dsp_ov_flag = 1;
#endif
    }
    else
    {
        ret = (q31_t)(((q63_t)x * ty.b16.h0) >> 15);
    }

    return ret;
}

DSP_FUNC_PREF q31_t NDS_DSP_KMMWB2_U(q31_t x, q31_t y)
{
    q31_t ret;
    union32_t ty;
    ty.w0 = y;

    if ((x == (q31_t)0x80000000) && (ty.b16.h0 == (q15_t)0x8000))
    {
        ret = (q31_t)0x7FFFFFFF;
#ifdef SIM_SAT_OV_FLAG
        riscv_dsp_dsp_ov_flag = 1;
#endif
    }
    else
    {
        ret = (q31_t)(((q63_t)x * ty.b16.h0 + ((q31_t)1 << 14)) >> 15);
    }

    return ret;
}

DSP_FUNC_PREF q31_t NDS_DSP_KMMWT2(q31_t x, q31_t y)
{
    q31_t ret;
    union32_t ty;
    ty.w0 = y;

    if ((x == (q31_t)0x80000000) && (ty.b16.h1 == (q15_t)0x8000))
    {
        ret = (q31_t)0x7FFFFFFF;
#ifdef SIM_SAT_OV_FLAG
        riscv_dsp_dsp_ov_flag = 1;
#endif
    }
    else
    {
        ret = (q31_t)(((q63_t)x * ty.b16.h1) >> 15);
    }

    return ret;
}

DSP_FUNC_PREF q31_t NDS_DSP_KMMWT2_U(q31_t x, q31_t y)
{
    q31_t ret;
    union32_t ty;
    ty.w0 = y;

    if ((x == (q31_t)0x80000000) && (ty.b16.h1 == (q15_t)0x8000))
    {
        ret = (q31_t)0x7FFFFFFF;
#ifdef SIM_SAT_OV_FLAG
        riscv_dsp_dsp_ov_flag = 1;
#endif
    }
    else
    {
        ret = (q31_t)(((q63_t)x * ty.b16.h1 + ((q31_t)1 << 14)) >> 15);
    }

    return ret;
}

DSP_FUNC_PREF q31_t NDS_DSP_KMMAWB2(q31_t acc, q31_t x, q31_t y)
{
    q31_t ret;
    union32_t ty;
    ty.w0 = y;

    if ((x == (q31_t)0x80000000) && (ty.b16.h0 == (q15_t)0x8000))
    {
        ret = (q31_t)0x7FFFFFFF;
#ifdef SIM_SAT_OV_FLAG
        riscv_dsp_dsp_ov_flag = 1;
#endif
    }
    else
    {
        ret = (q31_t)(((q63_t)x * ty.b16.h0) >> 15);
    }

    ret = NDS_DSP_KADDW(ret, acc);
    return ret;
}

DSP_FUNC_PREF q31_t NDS_DSP_KMMAWB2_U(q31_t acc, q31_t x, q31_t y)
{
    q31_t ret;
    union32_t ty;
    ty.w0 = y;

    if ((x == (q31_t)0x80000000) && (ty.b16.h0 == (q15_t)0x8000))
    {
        ret = (q31_t)0x7FFFFFFF;
#ifdef SIM_SAT_OV_FLAG
        riscv_dsp_dsp_ov_flag = 1;
#endif
    }
    else
    {
        ret = (q31_t)(((q63_t)x * ty.b16.h0 + ((q31_t)1 << 14)) >> 15);
    }

    ret = NDS_DSP_KADDW(ret, acc);
    return ret;
}

DSP_FUNC_PREF q31_t NDS_DSP_KMMAWT2(q31_t acc, q31_t x, q31_t y)
{
    q31_t ret;
    union32_t ty;
    ty.w0 = y;

    if ((x == (q31_t)0x80000000) && (ty.b16.h1 == (q15_t)0x8000))
    {
        ret = (q31_t)0x7FFFFFFF;
#ifdef SIM_SAT_OV_FLAG
        riscv_dsp_dsp_ov_flag = 1;
#endif
    }
    else
    {
        ret = (q31_t)(((q63_t)x * ty.b16.h1) >> 15);
    }

    ret = NDS_DSP_KADDW(ret, acc);
    return ret;
}

DSP_FUNC_PREF q31_t NDS_DSP_KMMAWT2_U(q31_t acc, q31_t x, q31_t y)
{
    q31_t ret;
    union32_t ty;
    ty.w0 = y;

    if ((x == (q31_t)0x80000000) && (ty.b16.h1 == (q15_t)0x8000))
    {
        ret = (q31_t)0x7FFFFFFF;
#ifdef SIM_SAT_OV_FLAG
        riscv_dsp_dsp_ov_flag = 1;
#endif
    }
    else
    {
        ret = (q31_t)(((q63_t)x * ty.b16.h1 + ((q31_t)1 << 14)) >> 15);
    }

    ret = NDS_DSP_KADDW(ret, acc);
    return ret;
}

DSP_FUNC_PREF q31_t NDS_DSP_KDMABB(q31_t acc, q31_t x, q31_t y)
{
    q31_t ret;
    union32_t tx, ty;
    tx.w0 = x;
    ty.w0 = y;

    if ((tx.b16.h0 == (q15_t)0x8000) && (ty.b16.h0 == (q15_t)0x8000))
    {
        ret = (q31_t)0x7FFFFFFF;
#ifdef SIM_SAT_OV_FLAG
        riscv_dsp_dsp_ov_flag = 1;
#endif
    }
    else
    {
        ret = (q31_t)(((q31_t)tx.b16.h0 * ty.b16.h0) << 1);
    }
    ret = NDS_DSP_KADDW(ret, acc);
    return ret;
}

DSP_FUNC_PREF q31_t NDS_DSP_KDMABT(q31_t acc, q31_t x, q31_t y)
{
    q31_t ret;
    union32_t tx, ty;
    tx.w0 = x;
    ty.w0 = y;

    if ((tx.b16.h0 == (q15_t)0x8000) && (ty.b16.h1 == (q15_t)0x8000))
    {
        ret = (q31_t)0x7FFFFFFF;
#ifdef SIM_SAT_OV_FLAG
        riscv_dsp_dsp_ov_flag = 1;
#endif
    }
    else
    {
        ret = (q31_t)(((q31_t)tx.b16.h0 * ty.b16.h1) << 1);
    }
    ret = NDS_DSP_KADDW(ret, acc);
    return ret;
}

DSP_FUNC_PREF q31_t NDS_DSP_KDMATT(q31_t acc, q31_t x, q31_t y)
{
    q31_t ret;
    union32_t tx, ty;
    tx.w0 = x;
    ty.w0 = y;

    if ((tx.b16.h1 == (q15_t)0x8000) && (ty.b16.h1 == (q15_t)0x8000))
    {
        ret = (q31_t)0x7FFFFFFF;
#ifdef SIM_SAT_OV_FLAG
        riscv_dsp_dsp_ov_flag = 1;
#endif
    }
    else
    {
        ret = (q31_t)(((q31_t)tx.b16.h1 * ty.b16.h1) << 1);
    }
    ret = NDS_DSP_KADDW(ret, acc);
    return ret;
}

#endif




#endif  //SIM_DSP_ISA_WITH_C


#ifdef ENA_DSP_MSG
void print16_2(q31_t x, q31_t y)
{
    union32_t tx, ty;

    tx.w0 = x;
    ty.w0 = y;
    DSPMSG("x:0x%08x [h0:0x%4x | h1:0x%4x]; y:0x%4x [h0:0x%4x | h1:0x%4x]\n",
           (int)x, (int)tx.b16.h0, (int)tx.b16.h1,
           (int)y, (int)ty.b16.h0, (int)ty.b16.h1);
}
#endif //ENA_DSP_MSG

#endif  //ENA_DSP_ISA

#ifdef ENA_DBG_MSG
void dump_data_q15(q15_t *src, int size, char *disp_string)
{
    static int group_id = 0;
    int i;

    DBGMSG("[%d] %s:\n", group_id++, disp_string);

    for (i = 0; i < size; i += 2)
    {
        DBGMSG("0x%08X [%5.10f] 0x%08X [%5.10f]\n", src[i], (float)src[i] / 0x8000L, src[i + 1], (float)src[i + 1] / 0x8000L);
    }
}

#endif //ENA_DBG_MSG



/**
 * @} end of dsp_isa
 */

