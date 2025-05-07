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
#ifndef __INTERNAL_MATH_TYPES_H__
#define __INTERNAL_MATH_TYPES_H__

//#include "nds32_intrinsic.h"
#include "riscv_dsp_math_types.h"       //for data types (q31_t, q15_t, ...)
#include "internal_nds_types.h"     //for union data types.

//RES_LOGN controls the resolution of cosine tables in transform functions.
//It also decide the value of RES_N is (1 << RES_LOGN)
#ifndef RES_LOGN
#define RES_LOGN    (10)            //default is 10
#endif

//RES_N is decided by RES_LOGN and it should not be defined manully.
#ifdef RES_N
#undef RES_N
#endif
#define RES_N       (1 << RES_LOGN)


//FFT_LOGN is idnetical with RES_LOGN in default configuration.
//
//If user wish to use lower resolution for larger samples transform,
//i.e. the samples in a frame is lager then the coeffients in cosine table.
//then FFT_LOGN should be set to RES_LOGN + 1
//(any value greater than RES_LOGN is OK, since we only check if FFT_LOGN > RES_LOGN).
//This leads lower precision but use smaller cosine table.
#ifndef FFT_LOGN
#define FFT_LOGN    RES_LOGN
#endif

//Only used by STATIC_VERSION
#define FFT_N (1 << FFT_LOGN)

//COS_RES_N controls the resolution of cosine tables in nds_cos_xx and nds_sin_xx functions.
//The value of COS_RES_N is identical with RES_N in default configuration.
//
//There is an exception case when user needs to calculate nds_cos_xx and
//nds32_sin_xx by input degree instead of redian. In this case, COS_RES_N should be set to 3600 manually.
//So both value between RES_N and COS_RES_N are different.
#ifndef COS_RES_N
#define COS_RES_N   RES_N
#endif

// Common Mathematical Symbols
#define TWO_PI      6.28318530717959f
#define PI          3.14159265358979f
#define PI_F64      3.1415926535897932384626433832795
#define HALF_PI     1.57079632679490f

// Radians Mapping
/**
 * @brief Radian mapping
 * radian [-2 * PI, 2 * PI) is mapped to [-1, 1)
 * degree [-180, 180) is mapped to [-1, 1)
 */

// radian [-PI, PI) is mapped to [-1, 1)
// degree [-180, 180) is mapped to [-1, 1)
#define UNIT_Q31    (1.0f/2147483648.0f)
#define PI_Q31      INT32_MIN   // 0x80000000
#define HALF_PI_Q31 1073741824  // 0x40000000

#define UNIT_Q15    (1.0f/32768.0f)
#define PI_Q15      INT16_MIN   // 0x8000
#define HALF_PI_Q15 16384       // 0x4000

#define UNIT_Q7     (1.0f/128.0f)
#define PI_Q7       INT8_MIN    // 0x80
#define HALF_PI_Q7  64          // 0x40

// Type Conversion
/**
 * dsp_convert_(source type)_to_(destination type)
 *      Comput the element of the source type to destination type.
 */

/**
 * @brief convert_src_to_dst.
 * @param[in]   *src point to the input vector.
 * @param[out]  *dst point to the output vector.
 * @param[in]   size size of the vectors.
 * @return none.
 */
// floating-point to Q(destination type)
static inline q15_t dsp_convert_float_to_q15(float x)
{
    q31_t q31;
    q15_t q15;
    x *= 32768.0f;
    x += (x < 0.0f ? -0.5f : 0.5f);
    q31 = (q31_t)x;
    if (q31 >= 32768)
    {
        q15 = INT16_MAX;
    }
    else if (q31 <= -32768)
    {
        q15 = INT16_MIN;
    }
    else
    {
        q15 = (q15_t)q31;
    }
    return q15;
}

static inline q31_t dsp_convert_float_to_q31(float x)
{
    q63_t q63;
    q31_t q31;
    x *= 2147483648.0f;
    x += (x < 0.0f ? -0.5f : 0.5f);
    q63 = (q63_t)x;
    if (q63 >= 2147483648LL)
    {
        q31 = INT32_MAX;
    }
    else if (q63 <= -2147483648LL)
    {
        q31 = INT32_MIN;
    }
    else
    {
        q31 = (q31_t)q63;
    }

    return q31;
}

static inline q7_t dsp_convert_float_to_q7(float x)
{
    q31_t q31;
    q7_t q7;

    x *= 128.0f;
    x += (x < 0.0f ? -0.5f : 0.5f);
    q31 = (q31_t)x;
    if (q31 >= 128)
    {
        q7 = INT8_MAX;
    }
    else if (q31 <= -128)
    {
        q7 = INT8_MIN;
    }
    else
    {
        q7 = (q7_t)q31;
    }

    return q7;
}

// Q31 to destination type
static inline float dsp_convert_q31_to_float(q31_t x)
{
    return x * UNIT_Q31;
}

static inline q15_t dsp_convert_q31_to_q15(q31_t x)
{
    return (q15_t)(x >> 16);
}

static inline q7_t dsp_convert_q31_to_q7(q31_t x)
{
    return (q7_t)(x >> 24);
}

// Q15 to destination type
static inline float dsp_convert_q15_to_float(q15_t x)
{
    return x * UNIT_Q15;
}

static inline q31_t dsp_convert_q15_to_q31(q15_t x)
{
    return ((q31_t)x) << 16;
}

static inline q7_t dsp_convert_q15_to_q7(q15_t x)
{
    return (q7_t)(x >> 8);
}

// Q7 to destination type
static inline float dsp_convert_q7_to_float(q7_t x)
{
    return x * UNIT_Q7;
}

static inline q31_t dsp_convert_q7_to_q31(q7_t x)
{
    return ((q31_t)x) << 24;
}

static inline q15_t dsp_convert_q7_to_q15(q7_t x)
{
    return ((q15_t)x) << 8;
}

/**
 * dsp_truncate_(source type)_to_(destination type);
 *      Truncates the source type values to destination type.
 */

/**
 * @brief truncate_src_to_dst
 * @param[in]   src input value.
 * @return output value after truncate.
 */

// Q63 to Q(destination type)
static inline q31_t dsp_truncate_q63_to_q31(q63_t src)
{
#ifdef NDS_CPU_64
    return src != (q31_t)src ?
           ((INT32_MAX ^ ((q31_t) (src >> 63)))) : (q31_t) src;
#else
    return ((q31_t) (src >> 32) != ((q31_t) src >> 31)) ?
           ((INT32_MAX ^ ((q31_t) (src >> 63)))) : (q31_t) src;
#endif
}

/**
 * nds_saturate(q31_t src, uin32_t (destination_type))
 *      Comput the saturation value to the input data.
 */

/**
 * @brief saturate to dst_type.
 * @param[in]   src input value.
 * @param[in]   type destination type.
 * @return saturate value of the destination type.
 */

//only support 16u and 8u types.
//32u does need to saturate since the incoming type is q31_t.

static inline q31_t dsp_saturate(q31_t src, const uint32_t type)
{
    q31_t max, min;

    if (type == 16u)
    {
        max = INT16_MAX;
        min = INT16_MIN;
    }
    else
    {
        max = INT8_MAX;
        min = INT8_MIN;
    }

    return (src > max) ? max : (src < min) ? min : src;
}

//Follows are patterns for helping compiler to generate correspoding instructions

//Bit-wise rotate right
#define NDS_PTN_ROTR(val,  rsh) \
    ((((uint32_t)(val)) >> (rsh)) | ((uint32_t)(val) << (rsh)))


#endif // __INTERNAL_MATH_TYPES_H__
