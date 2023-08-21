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
#include "internal_transform_math.h"

/* table of 1 / FFT_N or 2 / FFT_N where FFT_N is power of 2
 ** only the followings are supported */
const float riscv_dsp_recip_table_f32[RECIP_TABLE_SIZE] =
{
    ONE_BY_2_F32,
    ONE_BY_4_F32,     /* FFT_N = 4     FFT_LOGN = 2 (radix-4/radix-2) */
    ONE_BY_8_F32,     /* FFT_N = 8     FFT_LOGN = 3 (radix-2) */
    ONE_BY_16_F32,    /* FFT_N = 16    FFT_LOGN = 4 (radix-4/radix-2) */
    ONE_BY_32_F32,    /* FFT_N = 32    FFT_LOGN = 5 (radix-2) */
    ONE_BY_64_F32,    /* FFT_N = 64    FFT_LOGN = 6 (radix-4/radix-2) */
    ONE_BY_128_F32,   /* FFT_N = 128   FFT_LOGN = 7 (radix-2) */
    ONE_BY_256_F32,   /* FFT_N = 256   FFT_LOGN = 8 (radix-4/radix-2) */
    ONE_BY_512_F32,   /* FFT_N = 512   FFT_LOGN = 9 (radix-2) */
    ONE_BY_1024_F32,  /* FFT_N = 1024  FFT_LOGN = 10 (radix-4/radix-2) */
    ONE_BY_2048_F32,  /* FFT_N = 2048  FFT_LOGN = 11 (radix-2) */
    ONE_BY_4096_F32,  /* FFT_N = 4096  FFT_LOGN = 12 (radix-4/radix-2) */
    ONE_BY_8192_F32,  /* FFT_N = 8192  FFT_LOGN = 13 (radix-2) */
    ONE_BY_16384_F32, /* FFT_N = 16384 FFT_LOGN = 14 (radix-4/radix-2) */
    ONE_BY_32768_F32, /* FFT_N = 32768 FFT_LOGN = 15 (radix-2) */
    ONE_BY_65536_F32  /* FFT_N = 65536 FFT_LOGN = 16 (radix-4/radix-2) */
};

const double riscv_dsp_recip_table_f64[RECIP_TABLE_SIZE] =
{
    ONE_BY_2_F32,
    ONE_BY_4_F32,     /* FFT_N = 4     FFT_LOGN = 2 (radix-4/radix-2) */
    ONE_BY_8_F32,     /* FFT_N = 8     FFT_LOGN = 3 (radix-2) */
    ONE_BY_16_F32,    /* FFT_N = 16    FFT_LOGN = 4 (radix-4/radix-2) */
    ONE_BY_32_F32,    /* FFT_N = 32    FFT_LOGN = 5 (radix-2) */
    ONE_BY_64_F32,    /* FFT_N = 64    FFT_LOGN = 6 (radix-4/radix-2) */
    ONE_BY_128_F32,   /* FFT_N = 128   FFT_LOGN = 7 (radix-2) */
    ONE_BY_256_F32,   /* FFT_N = 256   FFT_LOGN = 8 (radix-4/radix-2) */
    ONE_BY_512_F32,   /* FFT_N = 512   FFT_LOGN = 9 (radix-2) */
    ONE_BY_1024_F32,  /* FFT_N = 1024  FFT_LOGN = 10 (radix-4/radix-2) */
    ONE_BY_2048_F32,  /* FFT_N = 2048  FFT_LOGN = 11 (radix-2) */
    ONE_BY_4096_F32,  /* FFT_N = 4096  FFT_LOGN = 12 (radix-4/radix-2) */
    ONE_BY_8192_F32,  /* FFT_N = 8192  FFT_LOGN = 13 (radix-2) */
    ONE_BY_16384_F32, /* FFT_N = 16384 FFT_LOGN = 14 (radix-4/radix-2) */
    ONE_BY_32768_F32, /* FFT_N = 32768 FFT_LOGN = 15 (radix-2) */
    ONE_BY_65536_F32  /* FFT_N = 65536 FFT_LOGN = 16 (radix-4/radix-2) */
};

const q31_t riscv_dsp_recip_table_q31[RECIP_TABLE_SIZE] =
{
    ONE_BY_2_Q31,
    ONE_BY_4_Q31,     /* FFT_N = 4     FFT_LOGN = 2 (radix-4/radix-2) */
    ONE_BY_8_Q31,     /* FFT_N = 8     FFT_LOGN = 3 (radix-2) */
    ONE_BY_16_Q31,    /* FFT_N = 16    FFT_LOGN = 4 (radix-4/radix-2) */
    ONE_BY_32_Q31,    /* FFT_N = 32    FFT_LOGN = 5 (radix-2) */
    ONE_BY_64_Q31,    /* FFT_N = 64    FFT_LOGN = 6 (radix-4/radix-2) */
    ONE_BY_128_Q31,   /* FFT_N = 128   FFT_LOGN = 7 (radix-2) */
    ONE_BY_256_Q31,   /* FFT_N = 256   FFT_LOGN = 8 (radix-4/radix-2) */
    ONE_BY_512_Q31,   /* FFT_N = 512   FFT_LOGN = 9 (radix-2) */
    ONE_BY_1024_Q31,  /* FFT_N = 1024  FFT_LOGN = 10 (radix-4/radix-2) */
    ONE_BY_2048_Q31,  /* FFT_N = 2048  FFT_LOGN = 11 (radix-2) */
    ONE_BY_4096_Q31,  /* FFT_N = 4096  FFT_LOGN = 12 (radix-4/radix-2) */
    ONE_BY_8192_Q31,  /* FFT_N = 8192  FFT_LOGN = 13 (radix-2) */
    ONE_BY_16384_Q31, /* FFT_N = 16384 FFT_LOGN = 14 (radix-4/radix-2) */
    ONE_BY_32768_Q31, /* FFT_N = 32768 FFT_LOGN = 15(radix-2) */
    ONE_BY_65536_Q31  /* FFT_N = 65536 FFT_LOGN = 16(radix4/radix-2) */
};

const q15_t riscv_dsp_recip_table_q15[RECIP_TABLE_SIZE] =
{
    ONE_BY_2_Q15,
    ONE_BY_4_Q15,     /* FFT_N = 4     FFT_LOGN = 2 (radix-4/radix-2) */
    ONE_BY_8_Q15,     /* FFT_N = 8     FFT_LOGN = 3 (radix-2) */
    ONE_BY_16_Q15,    /* FFT_N = 16    FFT_LOGN = 4 (radix-4/radix-2) */
    ONE_BY_32_Q15,    /* FFT_N = 32    FFT_LOGN = 5 (radix-2) */
    ONE_BY_64_Q15,    /* FFT_N = 64    FFT_LOGN = 6 (radix-4/radix-2) */
    ONE_BY_128_Q15,   /* FFT_N = 128   FFT_LOGN = 7 (radix-2) */
    ONE_BY_256_Q15,   /* FFT_N = 256   FFT_LOGN = 8 (radix-4/radix-2) */
    ONE_BY_512_Q15,   /* FFT_N = 512   FFT_LOGN = 9 (radix-2) */
    ONE_BY_1024_Q15,  /* FFT_N = 1024  FFT_LOGN = 10 (radix-4/radix-2) */
    ONE_BY_2048_Q15,  /* FFT_N = 2048  FFT_LOGN = 11 (radix-2) */
    ONE_BY_4096_Q15,  /* FFT_N = 4096  FFT_LOGN = 12 (radix-4/radix-2) */
    ONE_BY_8192_Q15,  /* FFT_N = 8192  FFT_LOGN = 13 (radix-2) */
    ONE_BY_16384_Q15, /* FFT_N = 16384 FFT_LOGN = 14 (radix-4/radix-2) */
    ONE_BY_32768_Q15, /* FFT_N = 32768 FFT_LOGN = 15(radix-2) */
    0x0000            /* cannot be supported */
};

/* table of PI / FFT_N or 2 * PI / FFT_N where FFT_N is power of 2
 ** only the followings are supported */
const float riscv_dsp_resolution_table_f32[RESOLUTION_TABLE_SIZE] =
{
    TWO_PI_BY_4_F32,     /* FFT_N = 4 */
    TWO_PI_BY_8_F32,     /* FFT_N = 8 */
    TWO_PI_BY_16_F32,    /* FFT_N = 16 */
    TWO_PI_BY_32_F32,    /* FFT_N = 32 */
    TWO_PI_BY_64_F32,    /* FFT_N = 64 */
    TWO_PI_BY_128_F32,   /* FFT_N = 128 */
    TWO_PI_BY_256_F32,   /* FFT_N = 256 */
    TWO_PI_BY_512_F32,   /* FFT_N = 512 */
    TWO_PI_BY_1024_F32,  /* FFT_N = 1024 */
    TWO_PI_BY_2048_F32,  /* FFT_N = 2048 */
    TWO_PI_BY_4096_F32,  /* FFT_N = 4096 */
    TWO_PI_BY_8192_F32,  /* FFT_N = 8192 */
    TWO_PI_BY_16384_F32, /* FFT_N = 16384 */
    TWO_PI_BY_32768_F32, /* FFT_N = 32768 */
    TWO_PI_BY_65536_F32  /* FFT_N = 65536 */
};

const q31_t riscv_dsp_resolution_table_q31[RESOLUTION_TABLE_SIZE] =
{
    TWO_PI_BY_4_Q31,     /* FFT_N = 4 */
    TWO_PI_BY_8_Q31,     /* FFT_N = 8 */
    TWO_PI_BY_16_Q31,    /* FFT_N = 16 */
    TWO_PI_BY_32_Q31,    /* FFT_N = 32 */
    TWO_PI_BY_64_Q31,    /* FFT_N = 64 */
    TWO_PI_BY_128_Q31,   /* FFT_N = 128 */
    TWO_PI_BY_256_Q31,   /* FFT_N = 256 */
    TWO_PI_BY_512_Q31,   /* FFT_N = 512 */
    TWO_PI_BY_1024_Q31,  /* FFT_N = 1024 */
    TWO_PI_BY_2048_Q31,  /* FFT_N = 2048 */
    TWO_PI_BY_4096_Q31,  /* FFT_N = 4096 */
    TWO_PI_BY_8192_Q31,  /* FFT_N = 8192 */
    TWO_PI_BY_16384_Q31, /* FFT_N = 16384 */
    TWO_PI_BY_32768_Q31, /* FFT_N = 32768 */
    TWO_PI_BY_65536_Q31  /* FFT_N = 65536 */
};

const q15_t riscv_dsp_resolution_table_q15[RESOLUTION_TABLE_SIZE] =
{
    TWO_PI_BY_4_Q15,     /* FFT_N = 4 */
    TWO_PI_BY_8_Q15,     /* FFT_N = 8 */
    TWO_PI_BY_16_Q15,    /* FFT_N = 16 */
    TWO_PI_BY_32_Q15,    /* FFT_N = 32 */
    TWO_PI_BY_64_Q15,    /* FFT_N = 64 */
    TWO_PI_BY_128_Q15,   /* FFT_N = 128 */
    TWO_PI_BY_256_Q15,   /* FFT_N = 256 */
    TWO_PI_BY_512_Q15,   /* FFT_N = 512 */
    TWO_PI_BY_1024_Q15,  /* FFT_N = 1024 */
    TWO_PI_BY_2048_Q15,  /* FFT_N = 2048 */
    TWO_PI_BY_4096_Q15,  /* FFT_N = 4096 */
    TWO_PI_BY_8192_Q15,  /* FFT_N = 8192 */
    TWO_PI_BY_16384_Q15, /* FFT_N = 16384 */
    TWO_PI_BY_32768_Q15, /* FFT_N = 32768 */
    TWO_PI_BY_65536_Q15  /* FFT_N = 65536 */
};

/* table of sqrt(2 / FFT_N) where FFT_N is power of 2
 ** only the followings are supported */
const float riscv_dsp_norm_table_f32[NORM_TABLE_SIZE] =
{
    SQRT_TWO_BY_4_F32,     /* FFT_N = 4 */
    SQRT_TWO_BY_8_F32,     /* FFT_N = 8 */
    SQRT_TWO_BY_16_F32,    /* FFT_N = 16 */
    SQRT_TWO_BY_32_F32,    /* FFT_N = 32 */
    SQRT_TWO_BY_64_F32,    /* FFT_N = 64 */
    SQRT_TWO_BY_128_F32,   /* FFT_N = 128 */
    SQRT_TWO_BY_256_F32,   /* FFT_N = 256 */
    SQRT_TWO_BY_512_F32,   /* FFT_N = 512 */
    SQRT_TWO_BY_1024_F32,  /* FFT_N = 1024 */
    SQRT_TWO_BY_2048_F32,  /* FFT_N = 2048 */
    SQRT_TWO_BY_4096_F32,  /* FFT_N = 4096 */
    SQRT_TWO_BY_8192_F32,  /* FFT_N = 8192 */
    SQRT_TWO_BY_16384_F32, /* FFT_N = 16384 */
    SQRT_TWO_BY_32768_F32, /* FFT_N = 32768 */
    SQRT_TWO_BY_65536_F32  /* FFT_N = 65536 */
};

const q31_t riscv_dsp_norm_table_q31[NORM_TABLE_SIZE] =
{
    SQRT_TWO_BY_4_Q31,     /* FFT_N = 4 */
    SQRT_TWO_BY_8_Q31,     /* FFT_N = 8 */
    SQRT_TWO_BY_16_Q31,    /* FFT_N = 16 */
    SQRT_TWO_BY_32_Q31,    /* FFT_N = 32 */
    SQRT_TWO_BY_64_Q31,    /* FFT_N = 64 */
    SQRT_TWO_BY_128_Q31,   /* FFT_N = 128 */
    SQRT_TWO_BY_256_Q31,   /* FFT_N = 256 */
    SQRT_TWO_BY_512_Q31,   /* FFT_N = 512 */
    SQRT_TWO_BY_1024_Q31,  /* FFT_N = 1024 */
    SQRT_TWO_BY_2048_Q31,  /* FFT_N = 2048 */
    SQRT_TWO_BY_4096_Q31,  /* FFT_N = 4096 */
    SQRT_TWO_BY_8192_Q31,  /* FFT_N = 8192 */
    SQRT_TWO_BY_16384_Q31, /* FFT_N = 16384 */
    SQRT_TWO_BY_32768_Q31, /* FFT_N = 32768 */
    SQRT_TWO_BY_65536_Q31  /* FFT_N = 65536 */
};

const q15_t riscv_dsp_norm_table_q15[NORM_TABLE_SIZE] =
{
    SQRT_TWO_BY_4_Q15,     /* FFT_N = 4 */
    SQRT_TWO_BY_8_Q15,     /* FFT_N = 8 */
    SQRT_TWO_BY_16_Q15,    /* FFT_N = 16 */
    SQRT_TWO_BY_32_Q15,    /* FFT_N = 32 */
    SQRT_TWO_BY_64_Q15,    /* FFT_N = 64 */
    SQRT_TWO_BY_128_Q15,   /* FFT_N = 128 */
    SQRT_TWO_BY_256_Q15,   /* FFT_N = 256 */
    SQRT_TWO_BY_512_Q15,   /* FFT_N = 512 */
    SQRT_TWO_BY_1024_Q15,  /* FFT_N = 1024 */
    SQRT_TWO_BY_2048_Q15,  /* FFT_N = 2048 */
    SQRT_TWO_BY_4096_Q15,  /* FFT_N = 4096 */
    SQRT_TWO_BY_8192_Q15,  /* FFT_N = 8192 */
    SQRT_TWO_BY_16384_Q15, /* FFT_N = 16384 */
    SQRT_TWO_BY_32768_Q15, /* FFT_N = 32768 */
    SQRT_TWO_BY_65536_Q15  /* FFT_N = 65536 */
};
