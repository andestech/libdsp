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

/**
 * @defgroup isa header file
 */

/**
 * This header file create a layer for Andes's instructions
 * excepts DSP ISA. In the other word, this file includes
 * basline, perf ext, perf ext 2, sat ext instructions. The DSP
 * ISA is located at internal_dsp_isa.h.
 */

#ifndef __INTERNAL_ISA_H__
#define __INTERNAL_ISA_H__

#ifdef  __cplusplus
extern "C"
{
#endif

#include "riscv_dsp_math_types.h"
/**
 * Just for testing or debugging Saturation Extension
 * instructions.
 *
 * Define this flag can force to use C functions to simulate
 * Saturation Extension instructions, no matter if it is
 * presented.
 *
 * Note:
 * The performance or code size will be affected if this flag is
 * defined. If SIM_DSP_ISA_WITH_C is defined, the
 * SIM_SAT_ISA_WITH_C will be defined too.
 */
#ifdef SIM_DSP_ISA_WITH_C
#define SIM_SAT_ISA_WITH_C
#else
//#define SIM_SAT_ISA_WITH_C
#endif

/**
 * Just for testing or debugging Performance Extension
 * instructions.
 *
 * Define this flag can force to use C functions to simulate
 * Performance Extension instructions, no matter if it is
 * presented.
 *
 * Note:
 * The performance or code size will be affected if this flag is
 * defined. If SIM_DSP_ISA_WITH_C is defined, the
 * SIM_SAT_ISA_WITH_C will be defined too.
 */
#ifdef SIM_DSP_ISA_WITH_C
#define SIM_PE_ISA_WITH_C
#else
//#define SIM_PE_ISA_WITH_C
#endif

#define SIM_V3_BASELINE_ISA_WITH_C

#define ISA_FUNC_PRFX           static inline

//IMM is just for labeling the operand/value is immediate.
#define IMM

//#if !defined(__NDS32_EXT_PERF__) || !defined(ENA_EXT_SAT_INST) || defined(SIM_SAT_ISA_WITH_C) || defined(SIM_PE_ISA_WITH_C)
#define SIM_SAT_OV_FLAG

//Un-defined this flag for performance consideration since it is not necessary to check it in libdsp now.
#undef SIM_SAT_OV_FLAG

#ifdef SIM_SAT_OV_FLAG
static int nds_sat_ov_flag;
#endif

//Baseline V1
#ifndef SIM_V3_BASELINE_ISA_WITH_C
#define NDS_ISA_SVA             __nds32__sva
#define NDS_ISA_SVS             __nds32__svs
#else
ISA_FUNC_PRFX uint32_t NDS_ISA_SVA(q31_t x, q31_t y)
{
    uint32_t ret = 0;

    const q31_t max_val = (q31_t)0x7FFFFFFF;
    const q31_t min_val = (q31_t)0x80000000;

    if(((y > 0) && (x > max_val - y)) || ((y < 0) && (x < min_val - y)))
        ret = 1;
    return ret;
}

ISA_FUNC_PRFX uint32_t NDS_ISA_SVS(q31_t x, q31_t y)
{
    uint32_t ret = 0;
    const q31_t max_val = (q31_t)0x7FFFFFFF;
    const q31_t min_val = (q31_t)0x80000000;

    if(((y > 0) && (x < min_val + y)) || ((y < 0) && (x > max_val + y)))
        ret = 1;

    return ret;
}
#endif

//Baseline V2
#define NDS_ISA_MULR64(X, Y)    ((uint64_t)X * Y)
#define NDS_ISA_MULSR64(X, Y)   ((q63_t)X * Y)


ISA_FUNC_PRFX uint32_t NDS_ISA_ABS(q31_t data)
{
    uint32_t ret;

    ret = (data >= 0) ? data : ((data == INT32_MIN) ? INT32_MAX : -data);
    return ret;
}

//ISA_FUNC_PRFX q31_t clip_q31_to_any(q31_t x, uint32_t y)
//{
//    int32_t posMax, negMin;
//    uint32_t i;
//
//    posMax = 1;
//    for (i = 0; i < (y - 1); i++)
//    {
//        posMax = posMax * 2;
//    }
//
//    if (x > 0)
//    {
//        posMax = (posMax - 1);
//
//        if (x > posMax)
//        {
//#ifdef SIM_SAT_OV_FLAG
//            nds_sat_ov_flag = 1;
//#endif
//
//            x = posMax;
//        }
//    }
//    else
//    {
//        negMin = -posMax;
//
//        if (x < negMin)
//        {
//#ifdef SIM_SAT_OV_FLAG
//            nds_sat_ov_flag = 1;
//#endif
//            x = negMin;
//        }
//    }
//    return (x);
//}

//ISA_FUNC_PRFX q31_t clip_u32_to_any(uint32_t x, uint32_t y)
//{
//    uint32_t posMax;
//    uint32_t ret;
//
//    posMax = (uint32_t)(((uint32_t)1 << (y - 1)) - 1);
//
//    if (x > posMax)
//    {
//#ifdef SIM_SAT_OV_FLAG
//        nds_sat_ov_flag = 1;
//#endif
//        ret = posMax;
//    }
//    else if ((int32_t)x < 0)
//    {
//#ifdef SIM_SAT_OV_FLAG
//        nds_sat_ov_flag = 1;
//#endif
//        ret = 0;
//    }
//    else
//    {
//        ret = x;
//    }
//    return ret;
//}

//bit_size: can be 1 to 32
static inline uint32_t sat_u32_to_any(q31_t src, const uint32_t bit_size)
{
    uint32_t ret;
    const q31_t max_val = (uint32_t)0xFFFFFFFF >> (32 - bit_size);

    ret = (src < 0) ? 0UL : (src > max_val) ? (uint32_t) max_val : (uint32_t) src;

#ifdef SIM_SAT_OV_FLAG
    if (ret != (uint32_t)src)
    {
        nds_sat_ov_flag = 1;
    }
#endif

    return ret;
}


//bit_size: can be 1 to 32
static inline q31_t sat_q31_to_any(q31_t src, const uint32_t bit_size)
{
    q31_t ret;
    const q31_t max_val = (q31_t)0x7FFFFFFF >> (32 - bit_size);
    const q31_t min_val = (q31_t)0x80000000 >> (32 - bit_size);

    ret = (src > max_val) ? max_val : (src < min_val) ? min_val : src;

#ifdef SIM_SAT_OV_FLAG
    if (ret != src)
    {
        nds_sat_ov_flag = 1;
    }
#endif

    return ret;
}


//the value of type should be 16 or 8.
//static inline q31_t sat_q31_to_q15_q8(q31_t src, const uint32_t type)
//{
//    q31_t type_max, type_min, ret;
//
//    if (type == 16U)
//    {
//        type_max = INT16_MAX;
//        type_min = INT16_MIN;
//    }
//    else
//    {
//        type_max = INT8_MAX;
//        type_min = INT8_MIN;
//    }
//
//    ret = (src > type_max) ? type_max : (src < type_min) ? type_min : src;
//
//#ifdef SIM_SAT_OV_FLAG
//    if (ret != src)
//    {
//        nds_sat_ov_flag = 1;
//    }
//#endif
//
//    return ret;
//}

//Calculate the value of log2(v)
static inline int32_t get_log2_32b(uint32_t v)
{
    uint32_t r, shift;

    r = (v > 0xFFFF) << 4;
    v >>= r;
    shift = (v > 0xFF) << 3;
    v >>= shift;
    r |= shift;
    shift = (v > 0xF) << 2;
    v >>= shift;
    r |= shift;
    shift = (v > 0x3) << 1;
    v >>= shift;
    r |= shift;
    r |= (v >> 1);

    return (int32_t)r;
}

//Get the index of Highest Bit for 32-bit value
static inline uint32_t get_highest_bit_32b(q31_t data)
{
    uint32_t idx = (uint32_t)data;
    if (idx != 0)
    {
        idx = get_log2_32b(idx) + 1;
    }
    return idx;
}

ISA_FUNC_PRFX uint32_t NDS_ISA_CLZ(q31_t data)
{
    return (uint32_t)32 - get_highest_bit_32b(data);
}

ISA_FUNC_PRFX uint32_t NDS_ISA_CLO(q31_t data)
{
    return (uint32_t)32 - get_highest_bit_32b(~data);
}

//Unsigned clipping for one 32-bit word at bit_idx, if ov set ov_flag.
//ISA_FUNC_PRFX uint32_t NDS_ISA_CLIP(uint32_t x, IMM uint8_t bit_idx)
//{
//    return sat_u32_to_any(x, bit_idx + 1);
//}
#define NDS_ISA_CLIP(X, Y)          sat_u32_to_any(X, (Y + 1))

//Signed clipping for one 32-bit word at bit_idx, if ov set ov_flag.
//ISA_FUNC_PRFX q31_t NDS_ISA_CLIPS(q31_t x, IMM uint8_t bit_idx)
//{
//    return sat_q31_to_any(x, bit_idx + 1);
//}
#define NDS_ISA_CLIPS(X, Y)         sat_q31_to_any(X, (Y + 1))

//Y: from 1 bit to 32 bits.
#define NDS_ISA_SATS(X, Y)          sat_q31_to_any(X, Y)
#define NDS_ISA_SAT(X, Y)           sat_u32_to_any(X, Y)

ISA_FUNC_PRFX q31_t sat_double_mul(q15_t x, q15_t y)
{
    q31_t ret;
    q31_t x_by_y = (q31_t)x * y;
    ret = x_by_y << 1;
    if (x_by_y != (ret >> 1))
    {
#ifdef SIM_SAT_OV_FLAG
        nds_sat_ov_flag = 1;
#endif
        ret = (q31_t)0x7FFFFFFF;
    }
    return ret;

    //if (((q15_t)0x8000 != x) || ((q15_t)0x8000 != y))
    //{
    //    ret = ((q31_t)x * y) << 1;
    //}
    //else
    //{
    //    ret = (q31_t)0x7FFFFFFF;
    //}
    //return ret;
}

ISA_FUNC_PRFX q15_t sat_half_mul(q15_t aop, q15_t bop)
{
    q31_t ret = ((q31_t)aop * bop) >> 15;
    return NDS_ISA_SATS(ret, 16);

    //q15_t Rt;
    //if (((q15_t)0x8000 != aop) || ((q15_t)0x8000 != bop))
    //{
    //    Rt = (q15_t)(((q31_t)aop * bop) >> 15);
    //}
    //else
    //{
    //    Rt = (q15_t)0x7FFF;
    //}
    //return Rt;
}

//Instructions simulation
ISA_FUNC_PRFX int32_t NDS_ISA_RDOV(void)
{
#ifdef SIM_SAT_OV_FLAG
    return nds_sat_ov_flag;
#else
    return 0;
#endif
}

ISA_FUNC_PRFX void NDS_ISA_CLOV(void)
{
#ifdef SIM_SAT_OV_FLAG
    nds_sat_ov_flag = 0;
#endif
}

ISA_FUNC_PRFX q31_t NDS_ISA_KADDW(q31_t x, q31_t y)
{
    //return clip_q63_to_q31((q63_t)x + y);   //very slow

    q31_t ret = x + y;
    if ((NDS_ISA_SVA(x, y) != 0) )
    {
#ifdef SIM_SAT_OV_FLAG
        nds_sat_ov_flag = 1;
#endif
        ret = (x >> 31) ^ 0x7FFFFFFF;
    }
    return ret;
}

ISA_FUNC_PRFX q31_t NDS_ISA_KSUBW(q31_t x, q31_t y)
{
    //return clip_q63_to_q31((q63_t)x - y);

    q31_t ret = x - y;
    if ((NDS_ISA_SVS(x, y) != 0) )
    {
#ifdef SIM_SAT_OV_FLAG
        nds_sat_ov_flag = 1;
#endif
        ret = (x >> 31) ^ 0x7FFFFFFF;
    }
    return ret;
}

ISA_FUNC_PRFX q15_t NDS_ISA_KADDH(q15_t x, q15_t y)
{
    q31_t ret;

    ret = (q31_t)x + y;
    ret = NDS_ISA_SATS(ret, 16);

    return (q15_t)ret;
}

ISA_FUNC_PRFX q15_t NDS_ISA_KSUBH(q15_t x, q15_t y)
{
    q31_t ret;

    ret = (q31_t)x - y;
    ret = NDS_ISA_SATS(ret, 16);

    return (q15_t)ret;
}


ISA_FUNC_PRFX q31_t NDS_ISA_KDMBB(q31_t x, q31_t y)
{
    return sat_double_mul((q15_t)x, (q15_t)y);
}

ISA_FUNC_PRFX q31_t NDS_ISA_KDMBT(q31_t x, q31_t y)
{
    return sat_double_mul((q15_t)x, (q15_t)(y >> 16));
}

ISA_FUNC_PRFX q31_t NDS_ISA_KDMTB(q31_t x, q31_t y)
{
    return sat_double_mul((q15_t)(x >> 16), (q15_t)y);
}

ISA_FUNC_PRFX q31_t NDS_ISA_KDMTT(q31_t x, q31_t y)
{
    return sat_double_mul((q15_t)(x >> 16), (q15_t)(y >> 16));
}

ISA_FUNC_PRFX q15_t NDS_ISA_KHMBB(q31_t x, q31_t y)
{
    return sat_half_mul((q15_t)x, (q15_t)y);
}
ISA_FUNC_PRFX q15_t NDS_ISA_KHMBT(q31_t x, q31_t y)
{
    return sat_half_mul((q15_t)x, (q15_t)(y >> 16));
}
ISA_FUNC_PRFX q15_t NDS_ISA_KHMTB(q31_t x, q31_t y)
{
    return sat_half_mul((q15_t)(x >> 16), (q15_t)y);
}
ISA_FUNC_PRFX q15_t NDS_ISA_KHMTT(q31_t x, q31_t y)
{
    return sat_half_mul((q15_t)(x >> 16), (q15_t)(y >> 16));
}

ISA_FUNC_PRFX q31_t NDS_ISA_KSLRAW(q31_t x, int sh)
{
    if (sh < 0 )
    {
        return (x >> (-sh));
    }
    else
    {
        //q63_t ret = (q63_t)x << sh;   //64-bit is very slow
        //return clip_q63_to_q31(ret);

        q31_t ret = x << sh;
        q31_t rev_sh_val = ret >> sh;

        if (x != rev_sh_val)
        {
#ifdef SIM_SAT_OV_FLAG
            nds_sat_ov_flag = 1;
#endif
            ret = (x >> 31) ^ 0x7FFFFFFF;
        }

        return ret;
    }
}





#ifdef  __cplusplus
}
#endif


#endif      //__INTERNAL_ISA_H__
