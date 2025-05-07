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
#include "internal_transform_math.h"

/**
 * @ingroup transform
 */

/**
 * @defgroup dct DCT Type II Functions
 *
 * A discrete cosine transform (DCT) expresses a finite sequence of data
 * points in terms of a sum of cosine functions oscillating at different
 * frequencies. In particular, a DCT is a Fourier-related transform similar
 * to the discrete Fourier transform (DFT), but using only real numbers.
 *
 * <pre>
 * DCT functions:
 * This function implements DCT Type II Function:
 *        N-1
 * y[k] = sum x[n] * cos(PI * (2 * n + 1) * k / (2 * N))
 *        n=0
 * </pre>
 *
 * <pre>
 * IDCT functions:
 * This function implements DCT Type III(IDCT for type II) Function:
 *                    N-1
 * x[k] = (2.0 / N) * sum c[n] * y[n] * cos(PI * (2 * k + 1) * n / (2 * N))
 *                    n=0
 *      where c[0] = 1 / 2.0 and c[n] = 1 for n != 0.
 * </pre>
 *
 * Implementation of N such that
 *
 *   1] N = 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192,
 *      and 16384.
 *
 *   2] m, defined as log2(N), is less than RES_LOGN.
 *
 * Functions include:
 *
 *  1. swap function : Internal work function for DCT/IDCT-II, used by odd and
 * even functions
 *
 *  2. odd function : Internal work function for DCT/IDCT-II, used by even
 * functions
 *
 *  3. even function : DCT/IDCT main function.
 */

/**
 * @addtogroup dct
 * @{
 */

/* function description */
static void riscv_dsp_idct_swap_f32(float32_t *src, uint32_t n, uint32_t level)
{
    float32_t riscv_dsp_dct_swap_buffer[n >> 1];
    unsigned k = n >> 1;
    float32_t *ptr1 = &src[k * level], *ptr2 = &src[n * level];
    float32_t *ptrs, *ptrd;

    ptrs = ptr2;
    ptrd = &riscv_dsp_dct_swap_buffer[0];

    while (ptrs != &src[0])
    {
        ptrs -= level;
        *ptrd++ = *ptrs;
        ptrs -= level;
    }

    ptrd = ptrs + level;
    while (ptrd != ptr1)
    {
        ptrs += 2 * level;
        *ptrd = *ptrs;
        ptrd += level;
    }

    ptrs = &riscv_dsp_dct_swap_buffer[0];
    ptrd = ptr1;
    while (ptrd != ptr2)
    {
        *ptrd = *ptrs++;
        ptrd += level;
    }
}

static void riscv_dsp_idct_o_f32(float32_t *src, uint32_t m, uint32_t level)
{
    float32_t even0, even1;  /* Temporary variable */
    float32_t odd0, odd1, tmp;
    uint32_t k;

    /* induction functions from 2, 3, 4, ..., DCT_LOGN */
    if (2 != m)
    {
        uint32_t n = 1 << m;
        uint32_t factor = 1 << (RES_LOGN - 1 - m);
        const float32_t *scale = &riscv_dsp_cos_recip_table_f32[factor >> 1];
        float32_t *ptre, *ptro;

        /* N > 4: (N / 2) * log2(N) + N / 4 (multiplications/divisions)
         **        (2 * N) * log2(N) - 9 * N / 4 + 1
         **                              (additions/subtractions) */
        /* (only (3 * N / 2) * log2(N) - 5 * N / 4 + 1 if maintaining
         ** table of 1 / (2 * cosine)) */

        /* even portion */
        riscv_dsp_idct_o_f32(src, m - 1, level << 1);
        /* preprocessing */
        ptre = &src[level];
        ptro = &src[(n - 1) * level];
        while (ptre != ptro)
        {
            float32_t *ptro2 = ptro - 2 * level;
            *ptro += *ptro2;
            ptro = ptro2;
        }

        /* odd portion */
        riscv_dsp_idct_o_f32(src + level, m - 1, level << 1);

        /* preprocessing */
        riscv_dsp_idct_swap_f32(src, n, level);

        /* weighted butterfly */
        ptre = &src[0];
        ptro = &src[n * level];
        while (ptre != ptro)
        {
            even0 = *ptre;
            ptro -= level;
            odd0 = *ptro;
            odd0 = odd0 * (*scale);
            *ptro = even0 - odd0;
            *ptre = even0 + odd0;
            ptre += level;
            scale += factor;
        }
    }
    else
    {
        /* N = 4: 5 multiplications/divisions */
        /*        8 additions/subtractions */
        k = level << 1;

        /* even portion */
        even0 = src[0];
        tmp = src[k] * DCT_COEF4_F32;
        even1 = even0 - tmp;    /* z1 = x0 - x2 * cos(PI / 4) */
        even0 += tmp;           /* z0 = x0 + x2 * cos(PI / 4) */

        /* odd portion */
        odd0 = src[k + level];
        tmp = src[level];
        /* z3 = x1 * cos(3 * PI / 8) - x3 * cos(PI / 8) */
        odd1 = tmp * DCT_COEF6_F32 - odd0 * DCT_COEF2_F32;
        /* z2 = x1 * cos(PI / 8) + x3 * cos(3 * PI / 8) */
        odd0 = tmp * DCT_COEF2_F32 + odd0 * DCT_COEF6_F32;

        /* butterfly */
        src[0] = even0 + odd0;            /* y0 = z0 + z2 */
        src[level] = even1 + odd1;        /* y1 = z1 + z3 */
        src[k] = even1 - odd1;            /* y2 = z1 - z3 */
        src[k + level] = even0 - odd0;    /* y3 = z0 - z2 */
    }
}


static void riscv_dsp_idct_e_f32(float32_t *src, uint32_t m)
{
    uint32_t n = (1 << m);
    float32_t *ptre, *ptro;

    float32_t r;

    /* scale the first term */
    src[0] *= 0.5f;

    /* call the recursive work function */
    riscv_dsp_idct_o_f32(src, m, 1);

    /* scale by 2 / N */
    r = riscv_dsp_recip_table_f32[m - 2];
    ptre = &src[0];
    ptro = &src[n];
    while (ptre != ptro)
    {
        *ptre++ *= r;
    }
}

static void riscv_dsp_dct_swap_f32(float32_t *src, uint32_t n, uint32_t level)
{
    float32_t riscv_dsp_dct_swap_buffer[n >> 1];
    unsigned k = n >> 1;
    float32_t *ptr1 = &src[k * level], *ptr2 = &src[n * level], *ptrs, *ptrd;

    ptrs = ptr2;
    ptrd = &riscv_dsp_dct_swap_buffer[0];
    while (ptrs != ptr1)
    {
        ptrs -= level;
        *ptrd++ = *ptrs;
    }

    ptrd = ptr2;
    while (ptrs != &src[0])
    {
        ptrs -= level;
        ptrd -= 2 * level;
        *ptrd = *ptrs;
    }

    ptrs = &riscv_dsp_dct_swap_buffer[0];
    ptrd = &src[0];
    while (ptrd != ptr2)
    {
        ptrd += level;
        *ptrd = *ptrs++;
        ptrd += level;
    }
}

static void riscv_dsp_dct_o_f32(float32_t *src, uint32_t m, uint32_t level)
{
    float32_t even0, even1;  /* Temporary variable */
    float32_t odd0, odd1, tmp;
    uint32_t k;

    /* induction functions from 2, 3, 4, ..., DCT_LOGN */
    if (2 != m)
    {
        /* N > 4: (N / 2) * log2(N) + N / 4 (multiplications/divisions)
         **       (2 * N) * log2(N) - 9 * N / 4 + 1
         **                    (additions/subtractions) */
        /*  (only (3 * N / 2) * log2(N) - 5 * N / 4 + 1 if maintaining
         ** table of 1 / (2 * cosine)) */
        uint32_t n = 1 << m;
        uint32_t factor = 1 << (RES_LOGN - 2 - m);
        const float32_t *scale = &riscv_dsp_cos_recip_table_f32[factor >> 1];
        float32_t *ptre = &src[0], *ptro = &src[(n - 1) * level];

        /* preprocessing */
        while (ptre != ptro)
        {
            float32_t *ptro2 = ptro - level;
            even0 = *ptro;
            *ptro = (even0 + *ptro2);
            ptro = ptro2;
        }

        /* do idct */
        riscv_dsp_idct_o_f32(src, m, level);

        /* postprocessing */
        ptro = &src[n * level];
        while (ptre != ptro)
        {
            even0 = *ptre;
            *ptre = (even0 * (*scale));
            ptre += level;
            scale += factor;
        }
    }
    else
    {
        /* N = 4: 5 multiplications/divisions
         **       8 additions/subtractions */

        /* preprocessing */
        k = level << 1;
        even0 = src[0];                   /* w0 = x0 */
        odd0 = src[level];
        even1 = src[k];
        odd1 = src[k + level] + even1;    /* w3 = x2 + x3 */
        even1 += odd0;                    /* w2 = x1 + x2 */
        odd0 += even0;                    /* w1 = x0 + x1 */

        /* even portion */
        tmp = even1 * DCT_COEF4_F32;
        even1 = even0 - tmp;    /* z1 = w0 - w2 * cos(PI / 4) */
        even0 += tmp;           /* z0 = w0 + w2 * cos(PI / 4) */

        /* odd portion */
        tmp = (odd0 + odd1) * DCT_COEF4_F32;
        /* z2 = x1 * cos(3 * PI / 8) - x3 * cos(PI / 8) */
        odd1 = (odd0 + tmp) * (1.0F / (2.0F * DCT_COEF2_F32));
        /* z3 = x1 * cos(PI / 8) + x3 * cos(3 * PI / 8) */
        odd0 = (odd0 - tmp) * (1.0F / (2.0f * DCT_COEF6_F32));

        /* modified butterfly */
        /* y0 = z0 + z2 */
        src[0] = (even0 + odd1) * (1.0F / (2.0F * DCT_COEF1_F32));
        /* y1 = z1 + z3 */
        src[k + level] = (even0 - odd1) * (1.0F / (2.0F * DCT_COEF7_F32));
        /* y3 = z1 - z3 */
        src[level] = (even1 + odd0) * (1.0F / (2.0F * DCT_COEF3_F32));
        /* y2 = z0 - z2 */
        src[k] = (even1 - odd0) * (1.0F / (2.0F * DCT_COEF5_F32));
    }
}

static void riscv_dsp_dct_e_f32(float32_t *src, uint32_t m, uint32_t level)
{
    float32_t tmp;
    uint32_t k;

    /* induction functions from 2, 3, 4, ..., DCT_LOGN */
    if (2 != m)
    {
        /*  N > 4: (N / 2) * log2(N) + N / 4 (multiplications/divisions)
         **        (2 * N) * log2(N) - 9 * N / 4 + 1
         **                        (additions/subtractions) */
        /*   (only (3 * N / 2) * log2(N) - 5 * N / 4 + 1 if maintaining
         ** table of 1 / (2 * cosine)) */
        uint32_t n = (1 << m);
        float32_t *ptre = &src[0], *ptro = &src[n * level];

        /* butterfly */
        while (ptre != ptro)
        {
            float32_t data = *ptre;
            ptro -= level;
            tmp = *ptro;
            *ptro = (data - tmp);
            *ptre = (data + tmp);
            ptre += level;
        }

        /* preprocessing */
        riscv_dsp_dct_swap_f32(src, n, level);

        /* even portion */
        riscv_dsp_dct_e_f32(src, m - 1, level << 1);

        /* odd portion */
        riscv_dsp_dct_o_f32(src + level, m - 1, level << 1);
    }
    else
    {
        /* N = 4: 5 (multiplications/divisions) */
        /*        8 (additions/subtractions) */
        float32_t x0, x1, x2, x3;

        /* butterfly */
        k = level << 1;
        x0 = src[0];
        tmp = src[k + level];
        x1 = x0 - tmp;     /* w2 = x0 - x3 */
        x0 += tmp;         /* w0 = x0 + x3 */
        x2 = src[k];
        tmp = src[level];
        x3 = tmp - x2;     /* w3 = x1 - x2 */
        x2 += tmp;         /* w1 = x1 + x2 */

        /* even portion */
        src[0] = x0 + x2;                /* y0 = w0 + w1 */
        src[k] = (x0 - x2) * DCT_COEF4_F32;  /* y2 = (w0 - w1) * cos(PI / 4) */

        /* odd portion */
        /* y1 = w2 * cos(PI / 8) + w3 * cos(3 * PI / 8) */
        src[level] = x1 * DCT_COEF2_F32 + x3 * DCT_COEF6_F32;
        /* y3 = w2 * cos(3 * PI / 8) - w3 * cos(PI / 8) */
        src[k + level] = x1 * DCT_COEF6_F32 - x3 * DCT_COEF2_F32;
    }
}

/**
 * @brief Function to implement the floating-point DCT Functions
 * @param[in]       *src     points to the input block data.
 * @param[in]       m        If not defined STATIC_VERSION, the data buffer
 *                           is of size 2 * n while n = 2 ** m.
 * @return none.
 */

void FUNC_ATTR_NO_HWLOOP
riscv_dsp_dct_f32(float32_t *src, uint32_t m)
{
    riscv_dsp_dct_e_f32(src, m, 1);
}

/**
 * @brief Function to implement the floating-point IDCT Functions
 * @param[in]       *src     points to the input block data.
 * @param[in]       m        If not defined STATIC_VERSION, the data buffer
 *                           is of size 2 * n while n = 2 ** m.
 * @return none.
 */

void FUNC_ATTR_NO_HWLOOP
riscv_dsp_idct_f32(float32_t *src, uint32_t m)
{
    riscv_dsp_idct_e_f32(src, m);
}

/**
 * @} end of dct
 */
