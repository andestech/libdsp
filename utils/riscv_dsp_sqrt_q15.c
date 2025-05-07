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
#include <math.h>
#include "internal_utils_math.h"

/**
 * @ingroup utils
 */

/**
 * @defgroup utsqrt Sqrt
 * In mathematics, a square root of a number x is a number y such that
 * y ^ 2 = x, or, in other words, a number y whose square (the result of
 * multiplying the number by itself, or y × y) is x. In q15 and q31 format,
 * the square root function is computed using the Newton-Raphson algorithm.
 * This is an iterative algorithm of the form:
 *             x1 = x0 - f(x0) / f'(x0);
 * Where x1 is the current estimate, x0 is the previous estimate, and f'(x0)
 * is the derivative of f(.) evaluated at x0. The algorithm is reduces to:
 *               x0 = in / 2    [initial guess]
 *               x1 = 0.5 * ( x0 + in / x0)     [each iteration].
 * In the q15 format there is another algorithm can use by defined the
 * configure SQRT_TABLE.
 *
 * 1) SQRT_TABLE : the alogorithn divide the input X = A * B where A = 2 ^ N
 * and 1 <= B < 2. Then we have a lookuptable for sqrt(2 ^ N), and two
 * lookuptables for sqrt(1 <= B < 2).
 */

/**
 * @addtogroup utsqrt
 * @{
 */

/**
 * @brief Square root of the q15 input.
 * @param[in]       src the input value.
 * @return the suqare root of input.
 */

/* function description */
#ifdef TWO_TABLE
#define SQRT_Q15_TBL2_NUM 128
#define SQRT_Q15_TBL2_LOG 7

#define SQRT_Q15_TBL2_FLOAT_NUM SQRT_Q15_TBL2_NUM##.0

/**
 * @brief Lookuptable for sqrt(2 ^ N)
 * int i;
 * sqrt_q15_tbl1[0] = 0;
 * for (i = 0; i < 15; i++) {
 *     sqrt_q15_tbl1[i + 1]= (int)(sqrt(pow(2.0, i)) * sqrt(32768.0));
 * }
 */

static q15_t sqrt_q15_tbl1[16] =
{
    0x0000, 0x00B5, 0x0100, 0x016A, 0x0200, 0x02D4, 0x0400, 0x05A8,
    0x0800, 0x0B50, 0x1000, 0x16A0, 0x2000, 0x2D41, 0x4000, 0x5A82,
};

/**
 * @brief Lookuptable for sqrt(1 <= B < 2)
 * for (i = 0; i < SQRT_Q15_TBL2_NUM; i++) {
 *     fin = sqrt(1.0 + i / SQRT_Q15_TBL2_FLOAT_NUM);
 *      sqrt_q15_tbl2[i] =  fin * 0x4000;
 * }
 */

static q15_t sqrt_q15_tbl2[128] =
{
    0x4000, 0x403F, 0x407F, 0x40BE, 0x40FE, 0x413C, 0x417B, 0x41BA,
    0x41F8, 0x4236, 0x4273, 0x42B1, 0x42EE, 0x432B, 0x4368, 0x43A5,
    0x43E1, 0x441E, 0x445A, 0x4495, 0x44D1, 0x450C, 0x4548, 0x4583,
    0x45BE, 0x45F8, 0x4633, 0x466D, 0x46A7, 0x46E1, 0x471B, 0x4754,
    0x478D, 0x47C7, 0x4800, 0x4838, 0x4871, 0x48A9, 0x48E2, 0x491A,
    0x4952, 0x498A, 0x49C1, 0x49F9, 0x4A30, 0x4A67, 0x4A9E, 0x4AD5,
    0x4B0B, 0x4B42, 0x4B78, 0x4BAE, 0x4BE5, 0x4C1A, 0x4C50, 0x4C86,
    0x4CBB, 0x4CF1, 0x4D26, 0x4D5B, 0x4D90, 0x4DC4, 0x4DF9, 0x4E2D,
    0x4E62, 0x4E96, 0x4ECA, 0x4EFE, 0x4F32, 0x4F65, 0x4F99, 0x4FCC,
    0x5000, 0x5033, 0x5066, 0x5099, 0x50CB, 0x50FE, 0x5130, 0x5163,
    0x5195, 0x51C7, 0x51F9, 0x522B, 0x525D, 0x528F, 0x52C0, 0x52F2,
    0x5323, 0x5354, 0x5385, 0x53B6, 0x53E7, 0x5418, 0x5449, 0x5479,
    0x54A9, 0x54DA, 0x550A, 0x553A, 0x556A, 0x559A, 0x55CA, 0x55FA,
    0x5629, 0x5659, 0x5688, 0x56B7, 0x56E6, 0x5716, 0x5745, 0x5773,
    0x57A2, 0x57D1, 0x5800, 0x582E, 0x585C, 0x588B, 0x58B9, 0x58E7,
    0x5915, 0x5943, 0x5971, 0x599F, 0x59CC, 0x59FA, 0x5A27, 0x5A55,
};

/**
 * @brief Lookuptable for sqrt(1 <= B < 2)
 * sqrt_q15_tbl3[0] = 0;
 * for (i = 0; i < 127; i++) {
 *     sqrt_q15_tbl3[i + 1] = (int)(sqrt((float)i + 1)
 *         * sqrt(32768.0)) >> 5;
 * }
 */

static q15_t sqrt_q15_tbl3[128] =
{
    0x0000, 0x0005, 0x0007, 0x0009, 0x000b, 0x000c, 0x000d, 0x000e,
    0x000f, 0x0010, 0x0011, 0x0012, 0x0013, 0x0014, 0x0015, 0x0015,
    0x0016, 0x0017, 0x0017, 0x0018, 0x0019, 0x0019, 0x001a, 0x001b,
    0x001b, 0x001c, 0x001c, 0x001d, 0x001d, 0x001e, 0x001e, 0x001f,
    0x001f, 0x0020, 0x0020, 0x0021, 0x0021, 0x0022, 0x0022, 0x0023,
    0x0023, 0x0024, 0x0024, 0x0025, 0x0025, 0x0025, 0x0026, 0x0026,
    0x0027, 0x0027, 0x0027, 0x0028, 0x0028, 0x0029, 0x0029, 0x0029,
    0x002a, 0x002a, 0x002b, 0x002b, 0x002b, 0x002c, 0x002c, 0x002c,
    0x002d, 0x002d, 0x002d, 0x002e, 0x002e, 0x002e, 0x002f, 0x002f,
    0x002f, 0x0030, 0x0030, 0x0030, 0x0031, 0x0031, 0x0031, 0x0032,
    0x0032, 0x0032, 0x0033, 0x0033, 0x0033, 0x0034, 0x0034, 0x0034,
    0x0035, 0x0035, 0x0035, 0x0035, 0x0036, 0x0036, 0x0036, 0x0037,
    0x0037, 0x0037, 0x0038, 0x0038, 0x0038, 0x0038, 0x0039, 0x0039,
    0x0039, 0x0039, 0x003a, 0x003a, 0x003a, 0x003b, 0x003b, 0x003b,
    0x003b, 0x003c, 0x003c, 0x003c, 0x003c, 0x003d, 0x003d, 0x003d,
    0x003d, 0x003e, 0x003e, 0x003e, 0x003e, 0x003f, 0x003f, 0x003f
};

q15_t riscv_dsp_sqrt_q15(q15_t src)
{
    int bits, idx;
    int retval;

    bits = dsp_ghb_16b(src);

    if ((bits - (SQRT_Q15_TBL2_LOG + 1)) > 0)
    {
        idx = src >> (bits - (SQRT_Q15_TBL2_LOG + 1));

        retval = ((q31_t)sqrt_q15_tbl1[bits] * sqrt_q15_tbl2[idx
                  & ((1 << SQRT_Q15_TBL2_LOG) - 1)]) >> 14;

        bits -= (SQRT_Q15_TBL2_LOG + 1);
        idx = (uint32_t)(src << (32 - bits)) >> (32 - bits);
        retval += sqrt_q15_tbl3[idx];
    }
    else
    {
        idx = src  << ((SQRT_Q15_TBL2_LOG + 1) - bits);
        retval = ((q31_t)sqrt_q15_tbl1[bits] * sqrt_q15_tbl2[idx
                  & ((1 << SQRT_Q15_TBL2_LOG) - 1)]) >> 14;
    }
    return (q15_t)retval;
}

#else

#define F2Q(f) ((int)((f) * 0x8000))
#define Q2F(q) ((float)(q) * (1.0f / 0x8000))
#define MULQ(a, b) (((a) * (b)) >> 15)

static inline int rsqrt(int x)
{
    int y;
    if (x & 0x8000)
    {
        y = MULQ(F2Q(3.81) - x, F2Q(0.37));
    }
    else
        y = MULQ(F2Q(3.81) - (x << 1), F2Q(0.37 * sqrt(2)));

    int x2 = x >> 1;
    y = MULQ(y, (F2Q(1.5) - MULQ(MULQ(x2, y), y)));
    y = MULQ(y, (F2Q(1.5) - MULQ(MULQ(x2, y), y)));

    return y;
}

q15_t riscv_dsp_sqrt_q15(q15_t val_in)
{
    int val = val_in;
    if (val > 0)
    {
        unsigned signbits = NDS_ISA_CLZ(val) - 17 + 1;
        signbits &= ~0x1;
        val = val << signbits;
        val = MULQ(val, rsqrt(val));
        val = val >> (signbits >> 1);
        return val;
    }
    else
    {
        return (val != 0) ? -1 : 0;
    }
}

#endif

/**
 * @} end of utsqrt
 */
