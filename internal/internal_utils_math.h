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
#ifndef __INTERNAL_UTILS_MATH_H__
#define __INTERNAL_UTILS_MATH_H__

/**
 * @defgroup utils Utils Functions
 * This set of functions implements sine, cosine, arctanm, and square root.
 * There are separate functions for Q15, Q31, and floating-point data.
 */

#include "internal_math_types.h"
#include "internal_isa.h"

// Cosine and Tangent Look-up Table and
/**
 * RES_N defines the resolution desired:
 * In riscv_dsp_cos and riscv_dsp_sin :
 * If application references higher resolution, call to riscv_dsp_cos and
 * riscv_dsp_sin to get the desired value using interpolation. otherwise, there
 * will be exact match to the table entry and thus these is no need to call
 * riscv_dsp_cos and riscv_dsp_sin.
 *
 * In riscv_dsp_atan :
 * Higher resolution is not supported. The same or lower resolution is
 * simply retrieved from table.
 */

#define STEP_UNIT_Q31_BASE      (0x80000000U)
#define STEP_UNIT_Q15_BASE      (0x8000U)

//Now STEP_UNIT_XX are only for transform functions (i.e. ignore 3600 for cos/sin functions)
#define STEP_UNIT_Q31           (STEP_UNIT_Q31_BASE >> (RES_LOGN - 1))
#define STEP_UNIT_Q15           (STEP_UNIT_Q15_BASE >> (RES_LOGN - 1))

#define STEP_UNIT_R_Q15 (0x7fff / STEP_UNIT_Q15)

// RES_N = 64
#if RES_N == 64

#define riscv_dsp_cos_table_f32 riscv_dsp_cos_table_f32_64
#define riscv_dsp_cos_table_q31 riscv_dsp_cos_table_q31_64
#define riscv_dsp_cos_table_q15 riscv_dsp_cos_table_q15_64

#define riscv_dsp_cos_table_f32_linear riscv_dsp_cos_table_f32_linear_64
#define riscv_dsp_cos_table_q15_linear riscv_dsp_cos_table_q15_linear_64
#define riscv_dsp_cos_table_q31_linear riscv_dsp_cos_table_q31_linear_64

#define riscv_dsp_tan_table_f32 riscv_dsp_tan_table_f32_64
#define riscv_dsp_tan_table_q31 riscv_dsp_tan_table_q31_64
#define riscv_dsp_tan_table_q15 riscv_dsp_tan_table_q15_64

#define riscv_dsp_cos_recip_table_f32   riscv_dsp_cos_recip_table_f32_64
#define riscv_dsp_cos_recip_table_q31   riscv_dsp_cos_recip_table_q31_64
#define riscv_dsp_cos_recip_table_q15   riscv_dsp_cos_recip_table_q15_64
// end of RES_N = 64

// RES_N = 128
#elif RES_N == 128


#define riscv_dsp_cos_table_f32 riscv_dsp_cos_table_f32_128
#define riscv_dsp_cos_table_q31 riscv_dsp_cos_table_q31_128
#define riscv_dsp_cos_table_q15 riscv_dsp_cos_table_q15_128

#define riscv_dsp_cos_table_f32_linear riscv_dsp_cos_table_f32_linear_128
#define riscv_dsp_cos_table_q15_linear riscv_dsp_cos_table_q15_linear_128
#define riscv_dsp_cos_table_q31_linear riscv_dsp_cos_table_q31_linear_128

#define riscv_dsp_tan_table_f32 riscv_dsp_tan_table_f32_128
#define riscv_dsp_tan_table_q31 riscv_dsp_tan_table_q31_128
#define riscv_dsp_tan_table_q15 riscv_dsp_tan_table_q15_128

#define riscv_dsp_cos_recip_table_f32   riscv_dsp_cos_recip_table_f32_128
#define riscv_dsp_cos_recip_table_q31   riscv_dsp_cos_recip_table_q31_128
#define riscv_dsp_cos_recip_table_q15   riscv_dsp_cos_recip_table_q15_128
// end of RES_N = 128

// RES_N = 256
#elif RES_N == 256

#define riscv_dsp_cos_table_f32 riscv_dsp_cos_table_f32_256
#define riscv_dsp_cos_table_q31 riscv_dsp_cos_table_q31_256
#define riscv_dsp_cos_table_q15 riscv_dsp_cos_table_q15_256

#define riscv_dsp_cos_table_f32_linear riscv_dsp_cos_table_f32_linear_256
#define riscv_dsp_cos_table_q15_linear riscv_dsp_cos_table_q15_linear_256
#define riscv_dsp_cos_table_q31_linear riscv_dsp_cos_table_q31_linear_256

#define riscv_dsp_tan_table_f32 riscv_dsp_tan_table_f32_256
#define riscv_dsp_tan_table_q31 riscv_dsp_tan_table_q31_256
#define riscv_dsp_tan_table_q15 riscv_dsp_tan_table_q15_256

#define riscv_dsp_sqrt_table_q15 riscv_dsp_sqrt_table_q15_256

#define riscv_dsp_cos_recip_table_f32   riscv_dsp_cos_recip_table_f32_256
#define riscv_dsp_cos_recip_table_q31   riscv_dsp_cos_recip_table_q31_256
#define riscv_dsp_cos_recip_table_q15   riscv_dsp_cos_recip_table_q15_256
// end of RES_N = 256

// RES_N = 512
#elif RES_N == 512

#define riscv_dsp_cos_table_f32 riscv_dsp_cos_table_f32_512
#define riscv_dsp_cos_table_q31 riscv_dsp_cos_table_q31_512
#define riscv_dsp_cos_table_q15 riscv_dsp_cos_table_q15_512

#define riscv_dsp_cos_table_f32_linear riscv_dsp_cos_table_f32_linear_512
#define riscv_dsp_cos_table_q15_linear riscv_dsp_cos_table_q15_linear_512
#define riscv_dsp_cos_table_q31_linear riscv_dsp_cos_table_q31_linear_512

#define riscv_dsp_tan_table_f32 riscv_dsp_tan_table_f32_512
#define riscv_dsp_tan_table_q31 riscv_dsp_tan_table_q31_512
#define riscv_dsp_tan_table_q15 riscv_dsp_tan_table_q15_512

#define riscv_dsp_sqrt_table_q15 riscv_dsp_sqrt_table_q15_512

#define riscv_dsp_cos_recip_table_f32   riscv_dsp_cos_recip_table_f32_512
#define riscv_dsp_cos_recip_table_q31   riscv_dsp_cos_recip_table_q31_512
#define riscv_dsp_cos_recip_table_q15   riscv_dsp_cos_recip_table_q15_512
// end of RES_N = 512

// RES_N = 1024
#elif RES_N == 1024

#define riscv_dsp_cos_table_f32 riscv_dsp_cos_table_f32_1024
#define riscv_dsp_cos_table_f64 riscv_dsp_cos_table_f64_1024
#if defined (__riscv_zfh)
#define riscv_dsp_cos_table_f16 riscv_dsp_cos_table_f16_1024
#endif
#define riscv_dsp_cos_table_q31 riscv_dsp_cos_table_q31_1024
#define riscv_dsp_cos_table_q15 riscv_dsp_cos_table_q15_1024

#define riscv_dsp_cos_table_f32_linear riscv_dsp_cos_table_f32_linear_1024
#define riscv_dsp_cos_table_q15_linear riscv_dsp_cos_table_q15_linear_1024
#define riscv_dsp_cos_table_q31_linear riscv_dsp_cos_table_q31_linear_1024

#define riscv_dsp_tan_table_f32 riscv_dsp_tan_table_f32_1024
#define riscv_dsp_tan_table_q31 riscv_dsp_tan_table_q31_1024
#define riscv_dsp_tan_table_q15 riscv_dsp_tan_table_q15_1024

#define riscv_dsp_sqrt_table_q15 riscv_dsp_sqrt_table_q15_1024

#define riscv_dsp_cos_recip_table_f32   riscv_dsp_cos_recip_table_f32_1024
#define riscv_dsp_cos_recip_table_q31   riscv_dsp_cos_recip_table_q31_1024
#define riscv_dsp_cos_recip_table_q15   riscv_dsp_cos_recip_table_q15_1024
// end of RES_N = 1024

// RES_N = 2048
#elif RES_N == 2048

#define riscv_dsp_cos_table_f32 riscv_dsp_cos_table_f32_2048
#define riscv_dsp_cos_table_q31 riscv_dsp_cos_table_q31_2048
#define riscv_dsp_cos_table_q15 riscv_dsp_cos_table_q15_2048

#define riscv_dsp_cos_table_f32_linear riscv_dsp_cos_table_f32_linear_2048
#define riscv_dsp_cos_table_q15_linear riscv_dsp_cos_table_q15_linear_2048
#define riscv_dsp_cos_table_q31_linear riscv_dsp_cos_table_q31_linear_2048

#define riscv_dsp_tan_table_f32 riscv_dsp_tan_table_f32_2048
#define riscv_dsp_tan_table_q31 riscv_dsp_tan_table_q31_2048
#define riscv_dsp_tan_table_q15 riscv_dsp_tan_table_q15_2048

#define riscv_dsp_sqrt_table_q15 riscv_dsp_sqrt_table_q15_2048

#define riscv_dsp_cos_recip_table_f32   riscv_dsp_cos_recip_table_f32_2048
#define riscv_dsp_cos_recip_table_q31   riscv_dsp_cos_recip_table_q31_2048
#define riscv_dsp_cos_recip_table_q15   riscv_dsp_cos_recip_table_q15_2048
// end of RES_N = 2048

// RES_N = 3600
//#elif RES_N == 3600
//
//#define riscv_dsp_cos_table_f32 riscv_dsp_cos_table_f32_3600
//#define riscv_dsp_cos_table_q31 riscv_dsp_cos_table_q31_3600
//#define riscv_dsp_cos_table_q15 riscv_dsp_cos_table_q15_3600
//
//#define riscv_dsp_tan_table_f32 riscv_dsp_tan_table_f32_3600
//#define riscv_dsp_tan_table_q31 riscv_dsp_tan_table_q31_3600
//#define riscv_dsp_tan_table_q15 riscv_dsp_tan_table_q15_3600
//
//#define riscv_dsp_cos_recip_table_f32   riscv_dsp_cos_recip_table_f32_3600
//#define riscv_dsp_cos_recip_table_q31   riscv_dsp_cos_recip_table_q31_3600
//#define riscv_dsp_cos_recip_table_q15   riscv_dsp_cos_recip_table_q15_3600
// end of RES_N = 3600

// RES_N = 4096
#elif RES_N == 4096

#define riscv_dsp_cos_table_f32 riscv_dsp_cos_table_f32_4096
#define riscv_dsp_cos_table_f64 riscv_dsp_cos_table_f64_4096
#define riscv_dsp_cos_table_q31 riscv_dsp_cos_table_q31_4096
#define riscv_dsp_cos_table_q15 riscv_dsp_cos_table_q15_4096

#define riscv_dsp_cos_table_f32_linear riscv_dsp_cos_table_f32_linear_4096
#define riscv_dsp_cos_table_q15_linear riscv_dsp_cos_table_q15_linear_4096
#define riscv_dsp_cos_table_q31_linear riscv_dsp_cos_table_q31_linear_4096

#define riscv_dsp_tan_table_f32 riscv_dsp_tan_table_f32_4096
#define riscv_dsp_tan_table_q31 riscv_dsp_tan_table_q31_4096
#define riscv_dsp_tan_table_q15 riscv_dsp_tan_table_q15_4096

#define riscv_dsp_sqrt_table_q15 riscv_dsp_sqrt_table_q15_4096

#define riscv_dsp_cos_recip_table_f32   riscv_dsp_cos_recip_table_f32_4096
#define riscv_dsp_cos_recip_table_q31   riscv_dsp_cos_recip_table_q31_4096
#define riscv_dsp_cos_recip_table_q15   riscv_dsp_cos_recip_table_q15_4096
// end of RES_N = 4096

// RES_N = 8192
#elif RES_N == 8192

#define riscv_dsp_cos_table_f32 riscv_dsp_cos_table_f32_8192
#define riscv_dsp_cos_table_f64 riscv_dsp_cos_table_f64_8192
#define riscv_dsp_cos_table_q31 riscv_dsp_cos_table_q31_8192
#define riscv_dsp_cos_table_q15 riscv_dsp_cos_table_q15_8192

#define riscv_dsp_cos_table_f32_linear riscv_dsp_cos_table_f32_linear_8192
#define riscv_dsp_cos_table_q15_linear riscv_dsp_cos_table_q15_linear_8192
#define riscv_dsp_cos_table_q31_linear riscv_dsp_cos_table_q31_linear_8192

#define riscv_dsp_tan_table_f32 riscv_dsp_tan_table_f32_8192
#define riscv_dsp_tan_table_q31 riscv_dsp_tan_table_q31_8192
#define riscv_dsp_tan_table_q15 riscv_dsp_tan_table_q15_8192

#define riscv_dsp_sqrt_table_q15 riscv_dsp_sqrt_table_q15_8192

#define riscv_dsp_cos_recip_table_f32   riscv_dsp_cos_recip_table_f32_8192
#define riscv_dsp_cos_recip_table_q31   riscv_dsp_cos_recip_table_q31_8192
#define riscv_dsp_cos_recip_table_q15   riscv_dsp_cos_recip_table_q15_8192
// end of RES_N = 8192

// RES_N = 16384
#elif RES_N == 16384

#define riscv_dsp_cos_table_f32 riscv_dsp_cos_table_f32_16384
#define riscv_dsp_cos_table_f64 riscv_dsp_cos_table_f64_16384
#define riscv_dsp_cos_table_q31 riscv_dsp_cos_table_q31_16384
#define riscv_dsp_cos_table_q15 riscv_dsp_cos_table_q15_16384

#define riscv_dsp_cos_table_f32_linear riscv_dsp_cos_table_f32_linear_16384
#define riscv_dsp_cos_table_q15_linear riscv_dsp_cos_table_q15_linear_16384
#define riscv_dsp_cos_table_q31_linear riscv_dsp_cos_table_q31_linear_16384

#define riscv_dsp_tan_table_f32 riscv_dsp_tan_table_f32_16384
#define riscv_dsp_tan_table_q31 riscv_dsp_tan_table_q31_16384
#define riscv_dsp_tan_table_q15 riscv_dsp_tan_table_q15_16384

#define riscv_dsp_cos_recip_table_f32   riscv_dsp_cos_recip_table_f32_16384
#define riscv_dsp_cos_recip_table_q31   riscv_dsp_cos_recip_table_q31_16384
#define riscv_dsp_cos_recip_table_q15   riscv_dsp_cos_recip_table_q15_16384
// end of RES_N = 16384

// RES_N = 32768
#elif RES_N == 32768

#define riscv_dsp_cos_table_f32 riscv_dsp_cos_table_f32_32768
#define riscv_dsp_cos_table_f64 riscv_dsp_cos_table_f64_32768
#define riscv_dsp_cos_table_q31 riscv_dsp_cos_table_q31_32768
#define riscv_dsp_cos_table_q15 riscv_dsp_cos_table_q15_32768

#define riscv_dsp_cos_table_f32_linear riscv_dsp_cos_table_f32_linear_32768
#define riscv_dsp_cos_table_q15_linear riscv_dsp_cos_table_q15_linear_32768
#define riscv_dsp_cos_table_q31_linear riscv_dsp_cos_table_q31_linear_32768

#define riscv_dsp_tan_table_f32 riscv_dsp_tan_table_f32_32768
#define riscv_dsp_tan_table_q31 riscv_dsp_tan_table_q31_32768
#define riscv_dsp_tan_table_q15 riscv_dsp_tan_table_q15_32768

#define riscv_dsp_cos_recip_table_f32   riscv_dsp_cos_recip_table_f32_32768
#define riscv_dsp_cos_recip_table_q31   riscv_dsp_cos_recip_table_q31_32768
#define riscv_dsp_cos_recip_table_q15   riscv_dsp_cos_recip_table_q15_32768
// end of RES_N = 32768

// RES_N = 65536
#elif RES_N == 65536

#define riscv_dsp_cos_table_f32 riscv_dsp_cos_table_f32_65536
#define riscv_dsp_cos_table_f64 riscv_dsp_cos_table_f64_65536
#define riscv_dsp_cos_table_q31 riscv_dsp_cos_table_q31_65536
#define riscv_dsp_cos_table_q15 riscv_dsp_cos_table_q15_65536

#define riscv_dsp_cos_table_f32_linear riscv_dsp_cos_table_f32_linear_65536
#define riscv_dsp_cos_table_q15_linear riscv_dsp_cos_table_q15_linear_65536
#define riscv_dsp_cos_table_q31_linear riscv_dsp_cos_table_q31_linear_65536

#define riscv_dsp_tan_table_f32 riscv_dsp_tan_table_f32_65536
#define riscv_dsp_tan_table_q31 riscv_dsp_tan_table_q31_65536
#define riscv_dsp_tan_table_q15 riscv_dsp_tan_table_q15_65536

#define riscv_dsp_cos_recip_table_f32   riscv_dsp_cos_recip_table_f32_65536
#define riscv_dsp_cos_recip_table_q31   riscv_dsp_cos_recip_table_q31_65536
#define riscv_dsp_cos_recip_table_q15   riscv_dsp_cos_recip_table_q15_65536
// end of RES_N = 65536

#endif

// Hermit Interpolation Table Offset
#ifdef HERMITE_INTERPOLATION
#define NDS32_COS_TABLE_SIZE    (RES_N >> 2) + 5
#define NDS32_COS_TABLE_LINEAR_SIZE (RES_N >> 2) + 1
#define NDS32_COS_TABLE_OFFSET  2
#else
#define NDS32_COS_TABLE_SIZE    (RES_N >> 2) + 1
#define NDS32_COS_TABLE_OFFSET  0
#endif

#define NDS32_TAN_TABLE_SIZE    (RES_N >> 2) + 1
#define NDS32_TAN_TABLE_OFFSET  0

#define NDS32_SQRT_TABLE_SIZE    (RES_N + 1)
#define NDS32_SQRT_TABLE_OFFSET  0

extern const float32_t riscv_dsp_cos_table_f32_linear[]; //NDS32_COS_TABLE_LINEAR_SIZE
extern const q15_t riscv_dsp_cos_table_q15_linear[]; //NDS32_COS_TABLE_LINEAR_SIZE
extern const q31_t riscv_dsp_cos_table_q31_linear[]; //NDS32_COS_TABLE_LINEAR_SIZE

extern const float32_t riscv_dsp_cos_table_f32[NDS32_COS_TABLE_SIZE];
extern const float64_t riscv_dsp_cos_table_f64[NDS32_COS_TABLE_SIZE];
#if defined (__riscv_zfh)
extern const float16_t riscv_dsp_cos_table_f16[NDS32_COS_TABLE_SIZE];
#endif
extern const q31_t riscv_dsp_cos_table_q31[NDS32_COS_TABLE_SIZE];
extern const q15_t riscv_dsp_cos_table_q15[NDS32_COS_TABLE_SIZE];

extern const float32_t riscv_dsp_tan_table_f32[NDS32_TAN_TABLE_SIZE];
extern const q31_t riscv_dsp_tan_table_q31[NDS32_TAN_TABLE_SIZE];
extern const q15_t riscv_dsp_tan_table_q15[NDS32_TAN_TABLE_SIZE];

extern const q15_t riscv_dsp_sqrt_table_q15[NDS32_SQRT_TABLE_SIZE];

extern const float32_t riscv_dsp_cos_recip_table_f32[RES_N >> 2];
extern const unsigned riscv_dsp_cos_recip_table_q31[RES_N >> 2];
extern const unsigned short riscv_dsp_cos_recip_table_q15[RES_N >> 2];

// Count the log2 of 16-bit value
static inline int32_t dsp_log2_16b(uint16_t v)
{
    uint32_t r, shift;

    r = (v > 0xFF) << 3;
    v >>= r;
    shift = (v > 0xF) << 2;
    v >>= shift;
    r |= shift;
    shift = (v > 0x3) << 1;
    v >>= shift;
    r |= shift;
    r |= (v >> 1);

    return (int32_t)r;
}

// Count the log2 of 32-bit value
static inline int32_t dsp_log2_32b(uint32_t v)
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


// Get the index of Highest Bit for 16-bit value
static inline uint32_t dsp_ghb_16b(q15_t data)
{
    uint32_t count;

    count = data;
    if (data != 0)
    {
        count = dsp_log2_16b(data) + 1;
    }
    return count;
}

// Get the index of Highest Bit for 32-bit value
static inline uint32_t dsp_ghb_32b(q31_t data)
{
    uint32_t count;

    count = data;
    if (data != 0)
    {
        count = dsp_log2_32b(data) + 1;
    }
    return count;
}

// Count Leading Zero
static inline uint32_t dsp_clz(q31_t data)
{
    uint32_t count;
    count = 32 - dsp_ghb_32b(data);
    return count;
}

// Absolute q31 value
//Use definition from internal header files rather than re-defined ABS.
#define dsp_abs(X)    NDS_ISA_ABS(X)


//#define FUNC_ATTR __attribute__ ((noinline))
#define F32_HMT_FUNC_ATTR   inline

typedef struct
{
    uint32_t e;
    int32_t m;
} st754;

//This fucntion extracts the elements of IEEE754 format
//and save elements into st754 structure.
//Note:
//  The sign bit will be applied on mantissa to do signed-extension.
F32_HMT_FUNC_ATTR
static st754
Xtr754(float32_t a)
{
    union32_t uni_a;
    st754 ret;

    uni_a.f0 = a;
    ret.e = (uni_a.u0 << 1) >> (32 - 8);
    ret.m = 0;
    if (uni_a.u0 != 0)
        ret.m = ((uni_a.u0 | ((uint32_t)1 << 23)) << 8) >> 8;
    ret.m = (uni_a.w0 < 0 ) ? -ret.m : ret.m;

#ifdef ENA_DBG_MSG
    DBGMSG("[Xtr754] a=%f;e=0x%08X;m=0x%08X\n", a, (unsigned int)ret.e, (unsigned int)ret.m);
#endif

    return ret;
}

//This function recovers back to IEEE754 format from st754 structure.
F32_HMT_FUNC_ATTR
static float32_t
To754(st754 a)
{
    union32_t ret;
    int32_t adj;
    uint32_t zcount, e, s, m;

    ret.f0 = 0.0F;
    if (a.m != 0)
    {
        s = a.m & 0x80000000UL;
        m = s ? -a.m : a.m;
        zcount = NDS_ISA_CLZ(m);
        adj = 9 - zcount - 1;
        e = a.e + adj;

        if (adj < 0)
        {
            m <<= (-adj);
        }
        else if (adj > 0)
        {
            m >>= adj;
        }

        m &= ~(1UL << 23);
        ret.u0 = s | m | (e << 23);
    }

    return ret.f0;
}

F32_HMT_FUNC_ATTR
static st754
Mul754(st754 a, st754 b)
{
    st754 ret;

    //1.23*1.23 >> 23 = 2.23
    ret.m = ((int64_t)a.m * b.m) >> 23;
    ret.e = 0;

    if (ret.m != 0)
    {
        ret.e = 127 + a.e - 127 + b.e - 127;
    }

    return ret;
}

//This function does addition between two st754 structures.
//Before adds two mantissa, the bigger exponent will be
//normalized to small one and the adjustment will be applied to its mantissa.
//This method also avoids overflow problem.
F32_HMT_FUNC_ATTR
static st754
Add754(st754 a, st754 b)
{
    st754 ret;
    int32_t m, adj;

    adj = (int32_t)a.e - (int32_t)b.e;
    if (adj < 0)
    {
        ret.e = b.e;
        m = a.m >> (-adj);
        ret.m = m + b.m;
    }
    else
    {
        ret.e = a.e;
        m = b.m >> adj;
        ret.m = a.m + m;
    }

    return ret;
}

//This function does subtraction between two st754 structures.
//Before subtracts two mantissa, it needs to do nomalization like Add754 function.
F32_HMT_FUNC_ATTR
static st754
Sub754(st754 a, st754 b)
{
    st754 ret;
    int32_t m, adj;

    adj = (int32_t)a.e - (int32_t)b.e;
    if (adj < 0)
    {
        ret.e = b.e;
        m = a.m >> (-adj);
        ret.m = m - b.m;
    }
    else
    {
        ret.e = a.e;
        m = b.m >> adj;
        ret.m = a.m - m;
    }
    return ret;
}

//This function is for F32 of Hermite Interpolation.
//This function extracts the elements of IEE754 format and saves them into a customized structure.
//Then processing calculations use with integer ALU rather than floating point calculation.
//And at the end, the separated elements are combinated back to IEE754 format as a return value.
//This method provides better performance compared with floating point calculation
//in spite of lossing precision but is acceptable.
//
//As the experiments of cos_f32 showed:
//The orignal code and results are:
//  The inst count is 1133
//  The cycle count is 1233
//  MAE is 0.00000011, RMSD is 0.00000015, NRMSD is 0.00000007, MAXDIFF is 0.00000037, SNR is 133.74754333
//Using this fucntion instead and results are:
//  The inst count is 344
//  The cycle count is 385
//  MAE is 0.00000010, RMSD is 0.00000013, NRMSD is 0.00000006, MAXDIFF is 0.00000036, SNR is 134.96951294
//
//Note:
//This fucntion is only designed and tested for cos_f32 and sin_f32. It works well for almost all IEEE754
//calculations (addition, subtraction and multiply) even through the input value is zero.
//However, the special cases of IEE754 are not considered totally (likes NAN, INF,...).
//
//The formula of Hermite Interpolation is:
//ret = y1 + 0.5f * (y2 - y0 + (y0 - y1 - tmp2 + tmp + tmp2 * fract) * fract) * fract;
F32_HMT_FUNC_ATTR
static float32_t
dsp_hmt_intp_ieee754(
    float32_t y0, float32_t y1, float32_t y2, float32_t y3, st754 st_fract)
{
    const float32_t cst_0_5 = 0.5F;
    const float32_t cst_3_0 = 3.0F;
    st754 st_ret;
    float32_t ret;
    st754 st_y0, st_y1, st_y2, st_y3, st_tmp, st_tmp2;
    st754 st_0_5, st_3_0;

    st_y0 = Xtr754(y0);                 DBGMSG("y0 = %2.24f\n", To754(st_y0));
    st_y1 = Xtr754(y1);                 DBGMSG("y1 = %2.24f\n", To754(st_y1));
    st_y2 = Xtr754(y2);                 DBGMSG("y2 = %2.24f\n", To754(st_y2));
    st_y3 = Xtr754(y3);                 DBGMSG("y3 = %2.24f\n", To754(st_y3));
    st_0_5 = Xtr754(cst_0_5);
    st_3_0 = Xtr754(cst_3_0);

    //y2 - y1;
    st_tmp = Sub754(st_y2, st_y1);      DBGMSG("tmp = %2.24f\n", To754(st_tmp));
    //y3 - y0 + (3.0f) * tmp;
    st_ret = Mul754(st_3_0, st_tmp);    DBGMSG("ret = %2.24f\n", To754(st_ret));

    //Recover to IEE754 then back will increase accuracy.
    ret = To754(st_ret);
    st_ret = Xtr754(ret);               DBGMSG("Normalized ret = %2.24f\n", To754(st_ret));

    st_tmp2 = Sub754(st_y3, st_y0);
    st_tmp2 = Sub754(st_tmp2, st_ret);  DBGMSG("tmp2 = %2.24f\n", To754(st_tmp2));

    //y0 - y1
    st_ret = Sub754(st_y0, st_y1);
    //c - tmp2
    st_ret = Sub754(st_ret, st_tmp2);
    st_ret = Add754(st_ret, st_tmp);

    //tmp2 * fract
    st_tmp = Mul754(st_tmp2, st_fract); DBGMSG("tmp2 * fract = %2.24f\n", To754(st_tmp));

    st_ret = Add754(st_ret, st_tmp);
    st_ret = Mul754(st_ret, st_fract);  DBGMSG("ret = %2.24f\n", To754(st_ret));

    st_tmp = Sub754(st_y2, st_y0);
    st_ret = Add754(st_tmp, st_ret);

    st_ret = Mul754(st_0_5, st_ret);
    st_ret = Mul754(st_ret, st_fract);
    st_ret = Add754(st_y1, st_ret);

    ret = To754(st_ret);                DBGMSG("HMT returns %2.24f\n", ret);

    return ret;
}


//This function compares the values between two st754 structures.
//if a > b, then return 1
//if a = b, then return 0
//if a < b, then return -1
F32_HMT_FUNC_ATTR
static int
Cmp754(st754 a, st754 b, st754 *st_result)
{
    int ret = 1;
    *st_result = Sub754(a, b);
    if (st_result->m < 0)
        ret = -1;
    else if (st_result->m == 0)
        ret = 0;
    return ret;
}

//Do some calculations and return inter and fraction part separately.
F32_HMT_FUNC_ATTR
static void
Get754I_F(st754 st_src, int32_t *index, st754 *st_fract)
//static void Get754I_F(float32_t src, int32_t *index, st754 *st_fract)
{
    st754 st_one = Xtr754((float32_t)((RES_N >> 1) / PI));
    //st754 st_src = Xtr754(src);
    st754 st_in = Mul754(st_src, st_one);
    int adj = st_in.e - 127;

    //normalize
    if (adj > 0)
    {
        st_in.m <<= adj;
    }
    else
    {
        st_in.m >>= (-adj);
    }

    *index = st_in.m >> 23;

    st_in.m = ((uint32_t)st_in.m << 9) >> 9;
    adj = NDS_ISA_CLZ(st_in.m);
    adj -= (9 - 1);
    st_in.e = 127 - adj;
    st_in.m <<= adj;
    //*fract = To754(st_in);
    st_fract->e = st_in.e;
    st_fract->m = st_in.m;
}

#define __ll_B ((uint32_t) 1 << 16)
#define __ll_lowpart(t) ((uint32_t) (t) & (__ll_B - 1))
#define __ll_highpart(t) ((uint32_t) (t) >> 16)

#ifndef ENA_DIV_REMAINDER
static inline q31_t dsp_div_u64_u32(uint64_t src1, uint32_t src2)
#else
static inline q31_t dsp_div_u64_u32(uint64_t src1, uint32_t src2, uint32_t * r)
#endif
{
    uint32_t n1, n0, cnt;
    uint32_t __d1, __d0, __q1, __q0;
    uint32_t __r1, __r0, __m;

    cnt = NDS_ISA_CLZ(src2);

    src1 <<= cnt;
    src2 <<= cnt;
    n1 = src1 >> 32;
    n0 = (uint32_t)src1;

    __d1 = __ll_highpart (src2);
    __d0 = __ll_lowpart (src2);
    __r1 = (n1) % __d1;
    __q1 = (n1) / __d1;
    __m = (uint32_t) __q1 * __d0;
    __r1 = __r1 * __ll_B | __ll_highpart (n0);
    if (__r1 < __m)
    {
        __q1--, __r1 += (src2);
        if (__r1 >= (src2))
            if (__r1 < __m)
                __q1--, __r1 += (src2);
    }
     __r1 -= __m;
     __r0 = __r1 % __d1;
     __q0 = __r1 / __d1;
     __m = (uint32_t) __q0 * __d0;
     __r0 = __r0 * __ll_B | __ll_lowpart (n0);

     if (__r0 < __m)
     {
         __q0--, __r0 += (src2);
         if (__r0 >= (src2))
             if (__r0 < __m)
                 __q0--, __r0 += (src2);
     }
#ifdef ENA_CALC_REMAINDER
     __r0 -= __m;
     *r = __r0;
#endif
     return ((uint32_t) __q1 * __ll_B | __q0);
}

#ifndef ENA_DIV_REMAINDER
static inline q31_t dsp_div_s64_u32(int64_t src1, uint32_t src2)
#else
static inline q31_t dsp_div_s64_u32(int64_t src1, uint32_t src2, int32_t * r)
#endif
{
    int32_t sgn = 0, ret;

    if (src1 < 0)
    {
        src1 = -src1;
        sgn = 1;
    }
#ifndef ENA_CALC_REMAINDER
    ret = dsp_div_u64_u32(src1, src2);
#else
    ret = dsp_div_u64_u32(src1, src2, r);
#endif
    if (sgn)
    {
        ret = -ret;
#ifdef ENA_CALC_REMAINDER
        *r = -(*r);
#endif
    }
    return ret;
}

// ------------- Compute library ------------
#include "math.h"
typedef union
{
    float32_t f32;
    int32_t   i32;
    uint32_t  u32;
} dsp_union32_t;

static const float CONST_LN2          = 0.6931471805f;     // ln(2)

// Compute library of exp_f32
//--- const values for taylor_poly_f32 ---
// Exponent polynomial coefficients
static const float EXP_CONST_COE0 = 1.f;
static const float EXP_CONST_COE1 = 0.0416598916054f;
static const float EXP_CONST_COE2 = 0.500000596046f;
static const float EXP_CONST_COE3 = 0.0014122662833f;
static const float EXP_CONST_COE4 = 1.00000011921f;
static const float EXP_CONST_COE5 = 0.00833693705499f;
static const float EXP_CONST_COE6 = 0.166665703058f;
static const float EXP_CONST_COE7 = 0.000195780929062f;

//--- const values for exp_f32 ---
static const float EXP_CONST_INV_LN2      = 1.4426950408f;     // 1/ln(2)
static const float EXP_CONST_INF          = INFINITY;
static const float EXP_CONST_MAX_INPUT    = 88.7f;
static const float CONST_0            = 0.f;
static const int   EXP_CONST_NEGATIVE_126 = -126;

//--- const values for taylor_poly_f64 ---
// Exponent polynomial coefficients
static const float64_t EXP_CONST_F64_COE0 = 1.0;
static const float64_t EXP_CONST_F64_COE1 = 0.0416598916054;
static const float64_t EXP_CONST_F64_COE2 = 0.500000596046;
static const float64_t EXP_CONST_F64_COE3 = 0.0014122662833;
static const float64_t EXP_CONST_F64_COE4 = 1.00000011921;
static const float64_t EXP_CONST_F64_COE5 = 0.00833693705499;
static const float64_t EXP_CONST_F64_COE6 = 0.166665703058;
static const float64_t EXP_CONST_F64_COE7 = 0.000195780929062;

//--- const values for exp_f64 ---
static const float64_t EXP_CONST_F64_INV_LN2      = 1.4426950408;     // 1/ln(2)
static const float64_t EXP_CONST_F64_INF          = INFINITY;
static const float64_t EXP_CONST_F64_MAX_INPUT    = 88.7;
static const float64_t F64_CONST_0            = 0.0;
static const int   EXP_CONST_NEGATIVE_1022 = -1022;
static const float64_t CONST_F64_LN2          = 0.6931471805;     // ln(2)

#if defined (__riscv_zfh)
static const float16_t F16_CONST_LN2          = 0.6931471805f;     // ln(2)
static const float16_t EXP_F16_CONST_COE0 = 1.f;
static const float16_t EXP_F16_CONST_COE1 = 0.0416598916054f;
static const float16_t EXP_F16_CONST_COE2 = 0.500000596046f;
static const float16_t EXP_F16_CONST_COE3 = 0.0014122662833f;
static const float16_t EXP_F16_CONST_COE4 = 1.00000011921f;
static const float16_t EXP_F16_CONST_COE5 = 0.00833693705499f;
static const float16_t EXP_F16_CONST_COE6 = 0.166665703058f;
static const float16_t EXP_F16_CONST_COE7 = 0.000195780929062f;

//--- const values for exp_f32 ---
static const float16_t EXP_F16_CONST_INV_LN2      = 1.4426950408f;     // 1/ln(2)
static const float16_t EXP_F16_CONST_INF          = INFINITY;
static const float16_t EXP_F16_CONST_MAX_INPUT    = 88.7f;
static const float16_t F16_CONST_0            = 0.f;
static const int   EXP_F16_CONST_NEGATIVE_15 = -15;
#endif

FUNC_ATTR_FFAST_MATH
static inline float dsp_taylor_poly_exp_f32(float x)
{
    float A = EXP_CONST_COE0 + EXP_CONST_COE4 * x;
    float B = EXP_CONST_COE2 + EXP_CONST_COE6 * x;
    float C = EXP_CONST_COE1 + EXP_CONST_COE5 * x;
    float D = EXP_CONST_COE3 + EXP_CONST_COE7 * x;
    float x2 = x * x;
    float x4 = x2 * x2;
    float res = (A + B * x2) + (C + D * x2) * x4;
    return res;
}

FUNC_ATTR_FFAST_MATH
static inline float32_t dsp_exp_f32(float32_t x)
{
#ifdef ENA_HIGHER_PERFORMANCE_VEXP_F32
    // Perform range reduction [-log(2),log(2)]
    int m = x * EXP_CONST_INV_LN2;
    float val = x - (float)m * CONST_LN2;

    // Polynomial Approximation
    dsp_union32_t poly;
    poly.f32 = dsp_taylor_poly_exp_f32(val);

    // Reconstruct
    int m2 = NDS_ISA_SATS((m << 23), 32);
    poly.i32 = NDS_ISA_KADDW(poly.i32, m2);

    // Handle overflow
    poly.f32 = (m < EXP_CONST_NEGATIVE_126) ? CONST_0 : poly.f32;
    poly.f32 = (x > EXP_CONST_MAX_INPUT) ? EXP_CONST_INF : poly.f32;

    return poly.f32;
#else
    return expf(x);
#endif
}

typedef union
{
    float64_t f64;
    int64_t   i64;
    uint64_t  u64;
} dsp_union64_t;

static inline float64_t dsp_taylor_poly_exp_f64(float64_t x)
{
    float64_t A = EXP_CONST_F64_COE0 + EXP_CONST_F64_COE4 * x;
    float64_t B = EXP_CONST_F64_COE2 + EXP_CONST_F64_COE6 * x;
    float64_t C = EXP_CONST_F64_COE1 + EXP_CONST_F64_COE5 * x;
    float64_t D = EXP_CONST_F64_COE3 + EXP_CONST_F64_COE7 * x;
    float64_t x2 = x * x;
    float64_t x4 = x2 * x2;
    float64_t res = (A + B * x2) + (C + D * x2) * x4;
    return res;
}


static inline float64_t dsp_exp_f64(float64_t x)
{
#ifdef ENA_HIGHER_PERFORMANCE_VEXP_F64
    // Perform range reduction [-log(2),log(2)]
    int64_t m = x * EXP_CONST_F64_INV_LN2;
    float64_t val = x - (float64_t)m * CONST_F64_LN2;

    // Polynomial Approximation
    dsp_union64_t poly;
    poly.f64 = dsp_taylor_poly_exp_f64(val);

    // Reconstruct
    int64_t m2 = m << 52;
    poly.i64 = poly.i64 + m2;

    // Handle overflow
    poly.f64 = (m < EXP_CONST_NEGATIVE_1022) ? F64_CONST_0 : poly.f64;
    poly.f64 = (x > EXP_CONST_F64_MAX_INPUT) ? EXP_CONST_F64_INF : poly.f64;

    return poly.f64;
#else
    return exp(x);
#endif
}

#if defined (__riscv_zfh)
static inline float16_t dsp_taylor_poly_exp_f16(float16_t x)
{
    float16_t A = EXP_F16_CONST_COE0 + EXP_F16_CONST_COE4 * x;
    float16_t B = EXP_F16_CONST_COE2 + EXP_F16_CONST_COE6 * x;
    float16_t C = EXP_F16_CONST_COE1 + EXP_F16_CONST_COE5 * x;
    float16_t D = EXP_F16_CONST_COE3 + EXP_F16_CONST_COE7 * x;
    float16_t x2 = x * x;
    float16_t x4 = x2 * x2;
    float16_t res = (A + B * x2) + (C + D * x2) * x4;
    return res;
}

typedef union
{
    float16_t f16;
    int16_t   i16;
    uint16_t  u16;
} dsp_union16_t;

static inline float16_t dsp_exp_f16(float16_t x)
{
    // Perform range reduction [-log(2),log(2)]
    int m = x * EXP_F16_CONST_INV_LN2;
    float16_t val = x - (float16_t)m * F16_CONST_LN2;

    // Polynomial Approximation
    dsp_union16_t poly;
    poly.f16 = dsp_taylor_poly_exp_f16(val);

    // Reconstruct
    int m2 = NDS_ISA_SATS((m << 10), 16);
    poly.i16 = NDS_ISA_KADDH(poly.i16, m2);

    // Handle overflow
    poly.f16 = (m < EXP_F16_CONST_NEGATIVE_15) ? F16_CONST_0 : poly.f16;
    poly.f16 = (x > EXP_F16_CONST_MAX_INPUT) ? EXP_F16_CONST_INF : poly.f16;

    return poly.f16;
}
// end of exp_f16
#endif

// Compute library of log_f32
static const float LOG_CONST_COE0 = -2.29561495781f;
static const float LOG_CONST_COE1 = -2.47071170807f;
static const float LOG_CONST_COE2 = -5.68692588806f;
static const float LOG_CONST_COE3 = -0.165253549814f;
static const float LOG_CONST_COE4 = 5.17591238022f;
static const float LOG_CONST_COE5 = 0.844007015228f;
static const float LOG_CONST_COE6 = 4.58445882797f;
static const float LOG_CONST_COE7 = 0.0141278216615f;
static const int   CONST_127  = 127;

FUNC_ATTR_FFAST_MATH
static inline float dsp_taylor_poly_log_f32(float x)
{
    float A = LOG_CONST_COE0 + LOG_CONST_COE4 * x;
    float B = LOG_CONST_COE2 + LOG_CONST_COE6 * x;
    float C = LOG_CONST_COE1 + LOG_CONST_COE5 * x;
    float D = LOG_CONST_COE3 + LOG_CONST_COE7 * x;
    float x2 = x * x;
    float x4 = x2 * x2;
    float res = (A + B * x2) + (C + D * x2) * x4;
    return res;
}

FUNC_ATTR_FFAST_MATH
static inline float dsp_log_f32(float x)
{
#ifdef ENA_HIGHER_PERFORMANCE_VLOG_F32
    // Extract exponent
    dsp_union32_t val, tmp;
    val.f32 = x;
    tmp.u32 = val.u32 >> 23;
    int m = tmp.i32 - CONST_127;
    int m2 = m << 23;
    val.i32 = val.i32 - m2;

    // Polynomial Approximation
    float out = dsp_taylor_poly_log_f32(val.f32);

    // Reconstruct
    out = out + (float)m * CONST_LN2;

    return out;
#else
    return logf(x);
#endif
}
// end of log_f32

#if defined (__riscv_zfh)
// Compute library of log_f16
static const float16_t LOG_F16_CONST_COE0 = -2.29561495781f;
static const float16_t LOG_F16_CONST_COE1 = -2.47071170807f;
static const float16_t LOG_F16_CONST_COE2 = -5.68692588806f;
static const float16_t LOG_F16_CONST_COE3 = -0.165253549814f;
static const float16_t LOG_F16_CONST_COE4 = 5.17591238022f;
static const float16_t LOG_F16_CONST_COE5 = 0.844007015228f;
static const float16_t LOG_F16_CONST_COE6 = 4.58445882797f;
static const float16_t LOG_F16_CONST_COE7 = 0.0141278216615f;
static const int   CONST_15  = 15;

static inline float16_t dsp_taylor_poly_log_f16(float16_t x)
{
    float16_t A = LOG_F16_CONST_COE0 + LOG_F16_CONST_COE4 * x;
    float16_t B = LOG_F16_CONST_COE2 + LOG_F16_CONST_COE6 * x;
    float16_t C = LOG_F16_CONST_COE1 + LOG_F16_CONST_COE5 * x;
    float16_t D = LOG_F16_CONST_COE3 + LOG_F16_CONST_COE7 * x;
    float16_t x2 = x * x;
    float16_t x4 = x2 * x2;
    float16_t res = (A + B * x2) + (C + D * x2) * x4;
    return res;
}

static inline float16_t dsp_log_f16(float16_t x)
{
    // Extract exponent
    dsp_union16_t val, tmp;
    val.f16 = x;
    tmp.u16 = val.u16 >> 10;
    int m = tmp.i16 - CONST_15;
    int m2 = m << 10;
    val.i16 = val.i16 - m2;

    // Polynomial Approximation
    float16_t out = dsp_taylor_poly_log_f16(val.f16);

    // Reconstruct
    out = out + (float16_t)m * F16_CONST_LN2;

    return out;
}
// end of log_f16
#endif

// Compute library of sigmoid_f32
static const float32_t CONST_1 = 1.f;
static const float32_t CONST_MIN_SIGMOID = -10.f;
static const float32_t CONST_MAX_SIGMOID = 10.f;

FUNC_ATTR_FFAST_MATH
static inline float32_t dsp_sigmoid_f32(float32_t x)
{
    x = (x < CONST_MIN_SIGMOID) ? CONST_MIN_SIGMOID : x;
    x = (x > CONST_MAX_SIGMOID) ? CONST_MAX_SIGMOID : x;
    float32_t num = dsp_exp_f32(x);
    float32_t den = num + CONST_1;
    float32_t out = num / den;
    return out;
}
// end of sigmoid_f32

#if defined (__riscv_zfh)
// Compute library of sigmoid_f16
static const float16_t F16_CONST_1 = 1.f;
static const float16_t F16_CONST_MIN_SIGMOID = -10.f;
static const float16_t F16_CONST_MAX_SIGMOID = 10.f;

static inline float16_t dsp_sigmoid_f16(float16_t x)
{
    x = (x < F16_CONST_MIN_SIGMOID) ? F16_CONST_MIN_SIGMOID : x;
    x = (x > F16_CONST_MAX_SIGMOID) ? F16_CONST_MAX_SIGMOID : x;
    float16_t num = dsp_exp_f16(x);
    float16_t den = num + F16_CONST_1;
    float16_t out = num / den;
    return out;
}
// end of sigmoid_f16
#endif

// Compute library of sin_f32
#define CONST_PI (3.14159265358979323846)
static const float32_t pi_v     = CONST_PI;
static const float32_t pio2_v   = (CONST_PI / 2);
static const float32_t ipi_v    = (1 / CONST_PI);
static const float32_t te_sin_coeff2 = 0.166666666666f; // 1/(2*3)
static const float32_t te_sin_coeff3 = 0.05f;           // 1/(4*5)
static const float32_t te_sin_coeff4 = 0.023809523810f; // 1/(6*7)
static const float32_t te_sin_coeff5 = 0.013888888889f; // 1/(8*9)
static const int INT_ZERO = 0;

static inline float dsp_sin_f32(float x)
{
#ifdef ENA_HIGHER_PERFORMANCE_VSIN_F32
    // algorithm from compute library
    dsp_union32_t c_v;

    //Find positive or negative
#ifdef ENA_XTR_754
    st754 st_x = Xtr754(x);
    st754 st_ipi_v = Xtr754(ipi_v);
    st754 st_out;
    st_out = Mul754(st_x, st_ipi_v);
    float32_t tmp = To754(st_out);
    c_v.i32 = NDS_ISA_ABS((int)tmp);
#else
    c_v.i32 = NDS_ISA_ABS((int)x * ipi_v);
#endif
#ifdef ENA_XTR_754
    st754 st_pre;
    st754 st_0f = Xtr754(CONST_0);
    uint32_t sign_v;
    if (Cmp754(st_x, st_0f, &st_pre) < 0)
        sign_v = 1;
    else
        sign_v = 0;
#else
    uint32_t sign_v = (x < 0) ? 1 : 0;
#endif
    uint32_t odd_v  = c_v.u32 & 1;
    uint32_t neg_v = odd_v ^ sign_v;

    // Modulus a - (n * int(a*(1/n)))
    float32_t absx = fabsf(x);
#ifdef ENA_XTR_754
    float tmpc_v = (float)c_v.i32;
    st754 st_tmpc_v = Xtr754(tmpc_v);
    st754 st_pi_v = Xtr754(pi_v);
    st754 st_absx = Xtr754(absx);
    st_out = Mul754(st_pi_v, st_tmpc_v);
    st754 st_ma;
    st_ma = Sub754(st_absx, st_out);
    float32_t ma = To754(st_ma);
#else
    float32_t ma = absx - pi_v * (float)c_v.i32;
#endif
    const uint32_t reb_v = (ma >= pio2_v) ? 1 : 0;

    //Rebase a between 0 and pi/2
    ma = (reb_v != 0) ? (pi_v - ma) : ma;

    // Taylor series
    const float32_t ma2 = ma * ma;

    //2nd elem: x^3 / 3!
    float32_t elem = (ma * ma2) * te_sin_coeff2;
    float32_t res = ma - elem;

    //3rd elem: x^5 / 5!
    elem = (elem * ma2) * te_sin_coeff3;
    res = res + elem;

    //4th elem: x^7 / 7!float32x2_t vsin_f32(float32x2_t val)
    elem = (elem * ma2) * te_sin_coeff4;
    res = res - elem;

    //5th elem: x^9 / 9!
    elem = (elem * ma2) * te_sin_coeff5;
    res = res + elem;

    //Change of sign
    neg_v = neg_v << 31;
    dsp_union32_t ures, out;
    ures.f32 = res;
    out.u32 = (ures.u32 ^ neg_v);
    return (out.f32);
#else
    return sinf(x);
#endif
}

#if defined (__riscv_zfh)
static const float16_t f16_pi_v     = CONST_PI;
static const float16_t f16_pio2_v   = (CONST_PI / 2);
static const float16_t f16_ipi_v    = (1 / CONST_PI);
static const float16_t f16_te_sin_coeff2 = 0.166666666666f; // 1/(2*3)
static const float16_t f16_te_sin_coeff3 = 0.05f;           // 1/(4*5)
static const float16_t f16_te_sin_coeff4 = 0.023809523810f; // 1/(6*7)
static const float16_t f16_te_sin_coeff5 = 0.013888888889f; // 1/(8*9)

static inline float16_t dsp_sin_f16(float16_t x)
{
    // algorithm from compute library
    dsp_union16_t c_v;

    //Find positive or negative
    c_v.i16 = NDS_ISA_ABS((int)x * f16_ipi_v);
    uint32_t sign_v = (x < 0) ? 1 : 0;
    uint32_t odd_v  = c_v.u16 & 1;
    uint32_t neg_v = odd_v ^ sign_v;

    // Modulus a - (n * int(a*(1/n)))
    float16_t absx = fabsf(x);
    float16_t ma = absx - f16_pi_v * (float16_t)c_v.i16;
    const uint32_t reb_v = (ma >= f16_pio2_v) ? 1 : 0;

    //Rebase a between 0 and pi/2
    ma = (reb_v != 0) ? (f16_pi_v - ma) : ma;

    // Taylor series
    const float16_t ma2 = ma * ma;

    //2nd elem: x^3 / 3!
    float16_t elem = (ma * ma2) * f16_te_sin_coeff2;
    float16_t res = ma - elem;

    //3rd elem: x^5 / 5!
    elem = (elem * ma2) * f16_te_sin_coeff3;
    res = res + elem;

    //4th elem: x^7 / 7!float32x2_t vsin_f32(float32x2_t val)
    elem = (elem * ma2) * f16_te_sin_coeff4;
    res = res - elem;

    //5th elem: x^9 / 9!
    elem = (elem * ma2) * f16_te_sin_coeff5;
    res = res + elem;

    //Change of sign
    neg_v = neg_v << 15;
    dsp_union16_t ures, out;
    ures.f16 = res;
    out.u16 = (ures.u16 ^ neg_v);
    return (out.f16);
}
#endif

static inline float dsp_cos_f32(float x)
{
#ifdef ENA_HIGHER_PERFORMANCE_VCOS_F32
    float32_t val = x + pio2_v;
    return (dsp_sin_f32(val));
#else
    return cosf(x);
#endif
}

#if defined (__riscv_zfh)
static inline float16_t dsp_cos_f16(float16_t x)
{
    float16_t val = x + f16_pio2_v;
    return (dsp_sin_f16(val));
}
#endif

// sqrt_q31 rv64 simd version (used in cmag_hp_q15 and cmag_q31)

#endif // __INTERNAL_UTILS_MATH_H__
