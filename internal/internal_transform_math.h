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
#ifndef __INTERNAL_TRANSFORM_MATH_H__
#define __INTERNAL_TRANSFORM_MATH_H__

/**
 * @defgroup transform Transform Functions
 */

#include "internal_math_types.h"
#include "internal_utils_math.h"        // for cos/sin tables
#include "riscv_dsp_utils_math.h"
#include "internal_isa.h"

// DCT Coefficient
#define DCT_COEF0_F32   0.707106781186548f  // 1.0f / sqrt(2)
#define DCT_COEF1_F32   0.980785280403230f  // cos(1 * PI / 16)
#define DCT_COEF2_F32   0.923879532511287f  // cos(2 * PI / 16)
#define DCT_COEF3_F32   0.831469612302545f  // cos(3 * PI / 16)
#define DCT_COEF4_F32   0.707106781186548f  // cos(4 * PI / 16)
#define DCT_COEF5_F32   0.555570233019602f  // cos(5 * PI / 16)
#define DCT_COEF6_F32   0.382683432365090f  // cos(6 * PI / 16)
#define DCT_COEF7_F32   0.195090322016128f  // cos(7 * PI / 16)

#define COEF_2_6 0x764230fc

#define DCT_COEF0_Q15   0x5a82  // 1.0f / sqrt(2)
#define DCT_COEF1_Q15   0x7d8b  // cos(1 * PI / 16)
#define DCT_COEF1_R_Q15 0x10504 // 1.0f / cos(1 * PI / 16)
#define DCT_COEF2_Q15   0x7642  // cos(2 * PI / 16)
#define DCT_COEF2_R_Q15 0x11518 // 1.0f / cos(2 * PI / 16)
#define DCT_COEF3_Q15   0x6a6e  // cos(3 * PI / 16)
#define DCT_COEF3_R_Q15 0x133e3 // 1.0f / cos(3 * PI / 16)
#define DCT_COEF4_Q15   0x5a82  // cos(4 * PI / 16)
#define DCT_COEF4_2_Q15   0xb504  // cos(4 * PI / 16)
#define DCT_COEF4_R_Q15 0x16a0a // 1.0f / cos(4 * PI / 16)
#define DCT_COEF5_Q15   0x471d  // cos(5 * PI / 16)
#define DCT_COEF5_R_Q15 0x1ccca // 1.0f / cos(5 * PI / 16)
#define DCT_COEF6_Q15   0x30fc  // cos(6 * PI / 16)
#define DCT_COEF6_R_Q15 0x29cf6 // 1.0f / cos(6 * PI / 16)
#define DCT_COEF7_Q15   0x18f9  // cos(7 * PI / 16)
#define DCT_COEF7_R_Q15 0x52036 // 1.0f / cos(7 * PI / 16)

#define DCT_COEF0_Q31   0x5A827999   // 1.0f / sqrt(2)
#define DCT_COEF1_Q31   0x7D8A5F3F   // cos(1 * PI / 16)
#define DCT_COEF1_R_Q31 0x20a07da3LL // 1.0f / cos(1 * PI / 16)
#define DCT_COEF2_Q31   0x7641AF3C   // cos(2 * PI / 16)
#define DCT_COEF2_R_Q31 0x22a2f4f7LL // 1.0f / cos(2 * PI / 16)
#define DCT_COEF3_Q31   0x6A6D98A4   // cos(3 * PI / 16)
#define DCT_COEF3_R_Q31 0x267c6f43LL // 1.0f / cos(3 * PI / 16)
#define DCT_COEF4_Q31   0x5a827999   // cos(4 * PI / 16)
#define DCT_COEF4_R_Q31 0x2d413ccdLL // 1.0f / cos(4 * PI / 16)
#define DCT_COEF5_Q31   0x471CECE6   // cos(5 * PI / 16)
#define DCT_COEF5_R_Q31 0x399935dfLL // 1.0f / cos(5 * PI / 16)
#define DCT_COEF6_Q31   0x30FBC54D   // cos(6 * PI / 16)
#define DCT_COEF6_R_Q31 0x539eba45LL // 1.0f / cos(6 * PI / 16)
#define DCT_COEF7_Q31   0x18F8B83C   // cos(7 * PI / 16)
#define DCT_COEF7_R_Q31 0xa406ce83LL // 1.0f / cos(7 * PI / 16)


/**
 * GET_COS_SIN_VALUES
 * This is the macro to retrieve cosine and sine values of a given radian.
 */

/**
 * @param[in]   *in     index for a value from [0, 2 * PI)
 * @param[out]  c       cosine value
 * @param[out]  s       sine value
 * @param[in]   m / p   if FFT_N > RES_N, it is p - constant value of
 *                      (2 * PI) / RES_N othersie, it is m or FFT_LOGN for
 *                      dynamic or static version
 * @param[in]   dt      data type - f32, q31, or q15
 */

#if FFT_LOGN > RES_LOGN

// FFT block entries is higher than the resolution of cosine look-up table.
// Need to call interpolation logic to get better cosine and sine values.
// p [input] : constant value of (2 * PI) / RES_N
#define GET_COS_SIN_VALUES(ia, c, s, p, dt)                \
    do {                                                   \
        c = riscv_dsp_cos_##dt(ia * p);                        \
        s = riscv_dsp_sin_##dt(ia * p);                        \
    } while (0)

#else

#define TRANSFORM_MATH_RAD90                ((int32_t)RES_N >> 2)
#define TRANSFORM_MATH_GET_CS_VAL                       \
    int32_t k = ia << (RES_LOGN - m);                   \
    if (TRANSFORM_MATH_RAD90 - k > 0)                   \
    {                                                   \
        *c = ptr[k];                                    \
        *s = ptr[TRANSFORM_MATH_RAD90 - k];             \
    }                                                   \
    else                                                \
    {                                                   \
        k -= TRANSFORM_MATH_RAD90;                      \
        if (TRANSFORM_MATH_RAD90 - k > 0)               \
        {                                               \
            *c = -ptr[TRANSFORM_MATH_RAD90 - k];        \
            *s = ptr[k];                                \
        }                                               \
        else                                            \
        {                                               \
            k -= TRANSFORM_MATH_RAD90;                  \
            if (TRANSFORM_MATH_RAD90 - k > 0)           \
            {                                           \
                *c = -ptr[k];                           \
                *s = -ptr[TRANSFORM_MATH_RAD90 - k];    \
            }                                           \
            else                                        \
            {                                           \
                *c = ptr[TRANSFORM_MATH_RAD90 - k];     \
                *s = -ptr[k];                           \
            }                                           \
        }                                               \
    }

static inline void get_cos_sin_values_f64 (int32_t ia, uint32_t m,
        float64_t *c, float64_t *s)
{
    const float64_t *ptr = &riscv_dsp_cos_table_f64[NDS32_COS_TABLE_OFFSET];
    TRANSFORM_MATH_GET_CS_VAL;
}

static inline void get_cos_sin_values_f32 (int32_t ia, uint32_t m,
        float32_t *c, float32_t *s)
{
    const float32_t *ptr = &riscv_dsp_cos_table_f32[NDS32_COS_TABLE_OFFSET];
    TRANSFORM_MATH_GET_CS_VAL;
}

static inline void get_cos_sin_values_q31 (int32_t ia, uint32_t m,
        q31_t *c, q31_t *s)
{
    const q31_t *ptr = &riscv_dsp_cos_table_q31[NDS32_COS_TABLE_OFFSET];
    TRANSFORM_MATH_GET_CS_VAL;
}

static inline void get_cos_sin_values_q15 (int32_t ia, uint32_t m,
        q15_t *c, q15_t *s)
{
    const q15_t *ptr = &riscv_dsp_cos_table_q15[NDS32_COS_TABLE_OFFSET];
    TRANSFORM_MATH_GET_CS_VAL;
}


// FFT block entries is less than or equal to the resolution of cosine
// look-up table. Thus direct table look-up is possible.
// m [input] : FFT_LOGN for dynamic version
#define GET_COS_SIN_VALUES(ia, c, s, m, dt) get_cos_sin_values_##dt(ia,m,&c,&s)
#endif // FFT_N > RES_N

// For the cfft_radix2
#define RECIP_TABLE_SIZE    16
#define RESOLUTION_TABLE_SIZE   15
#define NORM_TABLE_SIZE     15

// The following defines are used in the following tables.
// For static resolution, the global symbol can be used directly.
// For dynamic resolution, the look-up table can be used.
#define ONE_BY_2_F32        0.5000000000000000f
#define ONE_BY_4_F32        0.2500000000000000f // FFT_LOGN = 2
#define ONE_BY_8_F32        0.1250000000000000f // FFT_LOGN = 3
#define ONE_BY_16_F32       0.0625000000000000f // FFT_LOGN = 4
#define ONE_BY_32_F32       0.0312500000000000f // FFT_LOGN = 5
#define ONE_BY_64_F32       0.0156250000000000f // FFT_LOGN = 6
#define ONE_BY_128_F32      0.0078125000000000f // FFT_LOGN = 7
#define ONE_BY_256_F32      0.0039062500000000f // FFT_LOGN = 8
#define ONE_BY_512_F32      0.0019531250000000f // FFT_LOGN = 9
#define ONE_BY_1024_F32     0.0009765625000000f // FFT_LOGN = 10
#define ONE_BY_2048_F32     0.0004882812500000f // FFT_LOGN = 11
#define ONE_BY_4096_F32     0.0002441406250000f // FFT_LOGN = 12
#define ONE_BY_8192_F32     0.0001220703125000f // FFT_LOGN = 13
#define ONE_BY_16384_F32    0.0000610351562500f // FFT_LOGN = 14
#define ONE_BY_32768_F32    0.0000305175781250f // FFT_LOGN = 15
#define ONE_BY_65536_F32    0.0000152587890625f // FFT_LOGN = 16

#define ONE_BY_2_Q31        0x40000000
#define ONE_BY_4_Q31        0x20000000 // FFT_LOGN = 2
#define ONE_BY_8_Q31        0x10000000 // FFT_LOGN = 3
#define ONE_BY_16_Q31       0x08000000 // FFT_LOGN = 4
#define ONE_BY_32_Q31       0x04000000 // FFT_LOGN = 5
#define ONE_BY_64_Q31       0x02000000 // FFT_LOGN = 6
#define ONE_BY_128_Q31      0x01000000 // FFT_LOGN = 7
#define ONE_BY_256_Q31      0x00800000 // FFT_LOGN = 8
#define ONE_BY_512_Q31      0x00400000 // FFT_LOGN = 9
#define ONE_BY_1024_Q31     0x00200000 // FFT_LOGN = 10
#define ONE_BY_2048_Q31     0x00100000 // FFT_LOGN = 11
#define ONE_BY_4096_Q31     0x00080000 // FFT_LOGN = 12
#define ONE_BY_8192_Q31     0x00040000 // FFT_LOGN = 13
#define ONE_BY_16384_Q31    0x00020000 // FFT_LOGN = 14
#define ONE_BY_32768_Q31    0x00010000 // FFT_LOGN = 15
#define ONE_BY_65536_Q31    0x00008000 // FFT_LOGN = 16

#define ONE_BY_2_Q15        0x4000
#define ONE_BY_4_Q15        0x2000 // FFT_LOGN = 2
#define ONE_BY_8_Q15        0x1000 // FFT_LOGN = 3
#define ONE_BY_16_Q15       0x0800 // FFT_LOGN = 4
#define ONE_BY_32_Q15       0x0400 // FFT_LOGN = 5
#define ONE_BY_64_Q15       0x0200 // FFT_LOGN = 6
#define ONE_BY_128_Q15      0x0100 // FFT_LOGN = 7
#define ONE_BY_256_Q15      0x0080 // FFT_LOGN = 8
#define ONE_BY_512_Q15      0x0040 // FFT_LOGN = 9
#define ONE_BY_1024_Q15     0x0020 // FFT_LOGN = 10
#define ONE_BY_2048_Q15     0x0010 // FFT_LOGN = 11
#define ONE_BY_4096_Q15     0x0008 // FFT_LOGN = 12
#define ONE_BY_8192_Q15     0x0004 // FFT_LOGN = 13
#define ONE_BY_16384_Q15    0x0002 // FFT_LOGN = 14
#define ONE_BY_32768_Q15    0x0001 // FFT_LOGN = 15

#define TWO_PI_BY_4_F32     1.570796326794897f // FFT_LOGN = 4
#define TWO_PI_BY_8_F32     0.785398163397448f // FFT_LOGN = 8
#define TWO_PI_BY_16_F32    0.392699081698724f // FFT_LOGN = 16
#define TWO_PI_BY_32_F32    0.196349540849362f // FFT_LOGN = 32
#define TWO_PI_BY_64_F32    0.098174770424681f // FFT_LOGN = 64
#define TWO_PI_BY_128_F32   0.049087385212340f // FFT_LOGN = 128
#define TWO_PI_BY_256_F32   0.024543692606170f // FFT_LOGN = 256
#define TWO_PI_BY_512_F32   0.012271846303085f // FFT_LOGN = 512
#define TWO_PI_BY_1024_F32  0.006135923151542f // FFT_LOGN = 1024
#define TWO_PI_BY_2048_F32  0.003067961575771f // FFT_LOGN = 2048
#define TWO_PI_BY_4096_F32  0.001533980787886f // FFT_LOGN = 4096
#define TWO_PI_BY_8192_F32  0.000766990393943f // FFT_LOGN = 8192
#define TWO_PI_BY_16384_F32 0.000383495196971f // FFT_LOGN = 16384
#define TWO_PI_BY_32768_F32 0.000191747598486f // FFT_LOGN = 32768
#define TWO_PI_BY_65536_F32 0.000095873799243f // FFT_LOGN = 65536

#define TWO_PI_BY_4_Q31     0xc90fdaa2 // FFT_LOGN = 4
#define TWO_PI_BY_8_Q31     0x6487ed51 // FFT_LOGN = 8
#define TWO_PI_BY_16_Q31    0x3243f6a8 // FFT_LOGN = 16
#define TWO_PI_BY_32_Q31    0x1921fb54 // FFT_LOGN = 32
#define TWO_PI_BY_64_Q31    0x0c90fdaa // FFT_LOGN = 64
#define TWO_PI_BY_128_Q31   0x06487ed5 // FFT_LOGN = 128
#define TWO_PI_BY_256_Q31   0x03243f6a // FFT_LOGN = 256
#define TWO_PI_BY_512_Q31   0x01921fb5 // FFT_LOGN = 512
#define TWO_PI_BY_1024_Q31  0x00c90fda // FFT_LOGN = 1024
#define TWO_PI_BY_2048_Q31  0x006487ed // FFT_LOGN = 2048
#define TWO_PI_BY_4096_Q31  0x003243f6 // FFT_LOGN = 4096
#define TWO_PI_BY_8192_Q31  0x001921fb // FFT_LOGN = 8192
#define TWO_PI_BY_16384_Q31 0x000c90fd // FFT_LOGN = 16384
#define TWO_PI_BY_32768_Q31 0x0006487e // FFT_LOGN = 32768
#define TWO_PI_BY_65536_Q31 0x0003243f // FFT_LOGN = 65536

#define TWO_PI_BY_4_Q15     0xc910 // FFT_LOGN = 4
#define TWO_PI_BY_8_Q15     0x6488 // FFT_LOGN = 8
#define TWO_PI_BY_16_Q15    0x3244 // FFT_LOGN = 16
#define TWO_PI_BY_32_Q15    0x1922 // FFT_LOGN = 32
#define TWO_PI_BY_64_Q15    0x0c91 // FFT_LOGN = 64
#define TWO_PI_BY_128_Q15   0x0648 // FFT_LOGN = 128
#define TWO_PI_BY_256_Q15   0x0324 // FFT_LOGN = 256
#define TWO_PI_BY_512_Q15   0x0192 // FFT_LOGN = 512
#define TWO_PI_BY_1024_Q15  0x00c9 // FFT_LOGN = 1024
#define TWO_PI_BY_2048_Q15  0x0064 // FFT_LOGN = 2048
#define TWO_PI_BY_4096_Q15  0x0032 // FFT_LOGN = 4096
#define TWO_PI_BY_8192_Q15  0x0019 // FFT_LOGN = 8192
#define TWO_PI_BY_16384_Q15 0x000d // FFT_LOGN = 16384
#define TWO_PI_BY_32768_Q15 0x0006 // FFT_LOGN = 32768
#define TWO_PI_BY_65536_Q15 0x0003 // FFT_LOGN = 65536

#define SQRT_TWO_BY_4_F32       0.707106781186548f // FFT_N = 4
#define SQRT_TWO_BY_8_F32       0.500000000000000f // FFT_N = 8
#define SQRT_TWO_BY_16_F32      0.353553390593274f // FFT_N = 16
#define SQRT_TWO_BY_32_F32      0.250000000000000f // FFT_N = 32
#define SQRT_TWO_BY_64_F32      0.176776695296637f // FFT_N = 64
#define SQRT_TWO_BY_128_F32     0.125000000000000f // FFT_N = 128
#define SQRT_TWO_BY_256_F32     0.088388347648318f // FFT_N = 256
#define SQRT_TWO_BY_512_F32     0.062500000000000f // FFT_N = 512
#define SQRT_TWO_BY_1024_F32    0.044194173824159f // FFT_N = 1024
#define SQRT_TWO_BY_2048_F32    0.031250000000000f // FFT_N = 2048
#define SQRT_TWO_BY_4096_F32    0.022097086912080f // FFT_N = 4096
#define SQRT_TWO_BY_8192_F32    0.015625000000000f // FFT_N = 8192
#define SQRT_TWO_BY_16384_F32   0.011048543456040f // FFT_N = 16384
#define SQRT_TWO_BY_32768_F32   0.007812500000000f // FFT_N = 32768
#define SQRT_TWO_BY_65536_F32   0.005524271728020f // FFT_N = 65536

#define SQRT_TWO_BY_4_Q31       0x5a82799a // FFT_N = 4
#define SQRT_TWO_BY_8_Q31       0x40000000 // FFT_N = 8
#define SQRT_TWO_BY_16_Q31      0x2d413ccd // FFT_N = 16
#define SQRT_TWO_BY_32_Q31      0x20000000 // FFT_N = 32
#define SQRT_TWO_BY_64_Q31      0x16a09e66 // FFT_N = 64
#define SQRT_TWO_BY_128_Q31     0x10000000 // FFT_N = 128
#define SQRT_TWO_BY_256_Q31     0x0b504f33 // FFT_N = 256
#define SQRT_TWO_BY_512_Q31     0x08000000 // FFT_N = 512
#define SQRT_TWO_BY_1024_Q31    0x05a8279a // FFT_N = 1024
#define SQRT_TWO_BY_2048_Q31    0x04000000 // FFT_N = 2048
#define SQRT_TWO_BY_4096_Q31    0x02d413cd // FFT_N = 4096
#define SQRT_TWO_BY_8192_Q31    0x02000000 // FFT_N = 8192
#define SQRT_TWO_BY_16384_Q31   0x016a09e6 // FFT_N = 16384
#define SQRT_TWO_BY_32768_Q31   0x01000000 // FFT_N = 32768
#define SQRT_TWO_BY_65536_Q31   0x00b504f3 // FFT_N = 65536

#define SQRT_TWO_BY_4_Q15       0x5a82 // FFT_N = 4
#define SQRT_TWO_BY_8_Q15       0x4000 // FFT_N = 8
#define SQRT_TWO_BY_16_Q15      0x2d41 // FFT_N = 16
#define SQRT_TWO_BY_32_Q15      0x2000 // FFT_N = 32
#define SQRT_TWO_BY_64_Q15      0x16a1 // FFT_N = 64
#define SQRT_TWO_BY_128_Q15     0x1000 // FFT_N = 128
#define SQRT_TWO_BY_256_Q15     0x0b50 // FFT_N = 256
#define SQRT_TWO_BY_512_Q15     0x0800 // FFT_N = 512
#define SQRT_TWO_BY_1024_Q15    0x05a8 // FFT_N = 1024
#define SQRT_TWO_BY_2048_Q15    0x0400 // FFT_N = 2048
#define SQRT_TWO_BY_4096_Q15    0x02d4 // FFT_N = 4096
#define SQRT_TWO_BY_8192_Q15    0x0200 // FFT_N = 8192
#define SQRT_TWO_BY_16384_Q15   0x016a // FFT_N = 16384
#define SQRT_TWO_BY_32768_Q15   0x0100 // FFT_N = 32768
#define SQRT_TWO_BY_65536_Q15   0x00b5 // not supported

// table of 1 / FFT_N or 2 / FFT_N where FFT_N is power of 2
// only the followings are supported
extern const float32_t riscv_dsp_recip_table_f32[RECIP_TABLE_SIZE];
extern const float64_t riscv_dsp_recip_table_f64[RECIP_TABLE_SIZE];
extern const q31_t riscv_dsp_recip_table_q31[RECIP_TABLE_SIZE];
extern const q15_t riscv_dsp_recip_table_q15[RECIP_TABLE_SIZE];

// table of PI / FFT_N or 2 * PI / FFT_N where FFT_N is power of 2
// only the followings are supported
extern const float32_t riscv_dsp_resolution_table_f32[RESOLUTION_TABLE_SIZE];
extern const q31_t riscv_dsp_resolution_table_q31[RESOLUTION_TABLE_SIZE];
extern const q15_t riscv_dsp_resolution_table_q15[RESOLUTION_TABLE_SIZE];

// table of sqrt(2 / FFT_N) where FFT_N is power of 2
// only the followings are supported
extern const float32_t riscv_dsp_norm_table_f32[NORM_TABLE_SIZE];
extern const q31_t riscv_dsp_norm_table_q31[NORM_TABLE_SIZE];
extern const q15_t riscv_dsp_norm_table_q15[NORM_TABLE_SIZE];


#define TRANSFORM_MATH_SWAP(X, Y)                                       \
    temp=src[2*(X)]; src[2*(X)]=src[2*(Y)]; src[2*(Y)]=temp;            \
    temp=src[2*(X)+1]; src[2*(X)+1]=src[2*(Y)+1]; src[2*(Y)+1]=temp;

#define TRANSFORM_MATH_SWAPF1(X, Y)                                     \
    temp=f1[2*(int32_t)(X)]; f1[2*(int32_t)(X)]=f1[2*(int32_t)(Y)]; f1[2*(int32_t)(Y)]=temp;                \
    temp=f1[2*(int32_t)(X)-1]; f1[2*(int32_t)(X)-1]=f1[2*(int32_t)(Y)-1]; f1[2*(int32_t)(Y)-1]=temp;

#define BITREV_SWAP(r, k, m) for (i = (nh >> 2); !((r ^= i) & i); i >>= 1){}

#define TRANSFORM_MATH_BIT_REV                                          \
    const uint32_t nh = (n >> 1);                                       \
    uint32_t i; \
    uint32_t k = 2, r = nh;                                             \
    TRANSFORM_MATH_SWAP(1, nh);                                         \
    while (1)                                                           \
    {                                                                   \
        /* k even: */                                                   \
        r ^= nh;                                                        \
        for (i = (nh >> 1); !((r ^= i) & i); i >>= 1)                   \
        {                                                               \
            ;                                                           \
        }                                                               \
        \
        if (r > k)                                                      \
        {                                                               \
            TRANSFORM_MATH_SWAP(k, r);                                  \
            TRANSFORM_MATH_SWAPF1(-k, -r);                              \
        }                                                               \
        ++k;                                                            \
        \
        /* k odd: */                                                    \
        r += nh;                                                        \
        TRANSFORM_MATH_SWAP(k, r);                                      \
        \
        ++k;                                                            \
        \
        if (k >= nh)                                                    \
        {                                                               \
            /* break always happens here */                             \
            break;                                                      \
        }                                                               \
        \
        /* k even: */                                                   \
        r ^= nh;                                                        \
        r ^= (nh >> 1);                                                 \
        BITREV_SWAP(r, k, m);                                           \
        \
        if (r > k)                                                      \
        {                                                               \
            TRANSFORM_MATH_SWAP(k, r);                                  \
            TRANSFORM_MATH_SWAPF1(-k, -r);                              \
        }                                                               \
        ++k;                                                            \
        \
        /* k odd: */                                                    \
        r += nh;                                                        \
        TRANSFORM_MATH_SWAP(k, r);                                      \
        ++k;                                                            \
    }

/**
 * @brief  This function is the implementaion of bit reversal for f32.
 * @param[in]       src src[2 * k] and src[2 * k + 1] contain the real and
 *                      imaginary parts of the k-th data.
 * @param[in]       m   If not defined STATIC_VERSION, the data buffer
 *                      is of size 2 * n while n = 2 ** m.
 * @return valaue   0   success.
 *                  -1   invalid range.
 */

static inline int32_t riscv_dsp_bit_reversal_f32(float32_t *src, uint32_t m)
{
    float32_t temp;
    uint32_t n = (1 << m);
    float32_t *const f1 = src + (2 * n) - 1;
    TRANSFORM_MATH_BIT_REV;
    return 0;
}

static inline int32_t riscv_dsp_bit_reversal_f64(float64_t *src, uint32_t m)
{
    float64_t temp;
    uint32_t n = (1 << m);
    float64_t *const f1 = src + (2 * n) - 1;
    TRANSFORM_MATH_BIT_REV;
    return 0;
}

/**
 * @brief  This function is the implementaion of bit reversal for q31.
 * @param[in]       src src[2 * k] and src[2 * k + 1] contain the real and
 *                      imaginary parts of the k-th data.
 * @param[in]       m   If not defined STATIC_VERSION, the data buffer
 *                      is of size 2 * n while n = 2 ** m.
 * @return valaue   0   success.
 *                  -1   invalid range.
 */


static inline int32_t riscv_dsp_bit_reversal_q31(q31_t *src, uint32_t m)
{
    q31_t temp;
    uint32_t n = (1 << m);
    q31_t *const f1 = src + (2 * n) - 1;
    TRANSFORM_MATH_BIT_REV;
    return 0;
}

/**
 * @brief  This function is the implementaion of bit reversal for q15.
 * @param[in]       src src[2 * k] and src[2 * k + 1] contain the real and
 *                      imaginary parts of the k-th data.
 * @param[in]       m   If not defined STATIC_VERSION, the data buffer
 *                      is of size 2 * n while n = 2 ** m.
 * @return valaue   0   success.
 *                  -1   invalid range.
 */

static inline int32_t riscv_dsp_bit_reversal_q15(q15_t *src, uint32_t m)
{
    q15_t temp;
    uint32_t n = (1 << m);
    q15_t *const f1 = src + (2 * n) - 1;
    TRANSFORM_MATH_BIT_REV;
    return 0;
}

#endif // __INTERNAL_TRANSFORM_MATH_H__
