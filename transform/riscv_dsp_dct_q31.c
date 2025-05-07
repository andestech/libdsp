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
#include "internal_transform_math.h"

/**
 * @ingroup transform
 */

/**
 * @addtogroup dct
 * @{
 */

void riscv_dsp_idct_o_q31(q31_t *src, uint32_t m)
{
    q63_t even0, even1;  /* Temporary variable */
    q63_t odd0, odd1, tmp;

    /* induction functions from 2, 3, 4, ..., FFT_LOGN */
    if (2 != m)
    {
        uint32_t n = 1 << m;
        uint32_t k = n >> 1;
        q31_t swap_buf[k];
        uint32_t factor;
        const unsigned *scale;
        q31_t *ptre, *ptro, *ptrz, *ptrx, data;

        /* N > 4: (N / 2) * log2(N) + N / 4 (multiplications/divisions)
         **        (2 * N) * log2(N) - 9 * N / 4 + 1
         **                              (additions/subtractions) */
        /* (only (3 * N / 2) * log2(N) - 5 * N / 4 + 1 if maintaining
         ** table of 1 / (2 * cosine)) */

        /* pre-processing */
        /* example of n = 8:
         * input:  [a, b, c, d, e, f, g, h]
         * output: [a, c, e, g, -, -, -, -] and [b, b+d, d+f, f+h]
         */
        ptrz = ptre = &src[1];
        ptro = &src[n];
        ptrx = &swap_buf[0];
        data = *ptre++;
        *ptrx++ = data;
        while (ptre != ptro)
        {
            *ptrz++ = *ptre++;
            *ptrx++ = data + *ptre;
            data = *ptre++;
        }

        /* even portion */
        riscv_dsp_idct_o_q31(src, m - 1);

        /* odd portion */
        riscv_dsp_idct_o_q31(swap_buf, m - 1);

        /* post-processing */
        /* example of n = 8:
         * output: [a, b, c, d, -, -, -, -] and [e, f, g, h]
         * input:  [a, b, c, d, h, g, f, e]
         */
        ptrx--;
        while (ptrz != ptro)
        {
            *ptrz++ = *ptrx--;
        }

        /* weighted butterfly */
        factor = 1 << (RES_LOGN - 1 - m);
        scale = &riscv_dsp_cos_recip_table_q31[(factor >> 1)];
        uint32_t i = 0;
        while (i != k)
        {
            even0 = src[i];
            odd0 = ((q63_t)src[(n - 1) - i] * (*scale)) >> (35 - RES_LOGN);
            src[i] = even0 + odd0;
            src[(n - 1) - i] = even0 - odd0;
            i++;
            scale += factor;
        }

    }
    else
    {
        /* N = 4: 5 multiplications/divisions */
        /*        8 additions/subtractions */

        /* even portion */
        even0 = src[0];
        tmp = ((q63_t)src[2] * DCT_COEF4_Q31) >> 31;
        even1 = even0 - tmp;    /* z1 = x0 - x2 * cos(PI / 4) */
        even0 = even0 + tmp;    /* z0 = x0 + x2 * cos(PI / 4) */

        /* odd portion */
        odd0 = src[3];
        tmp = src[1];
        /* z3 = x1 * cos(3 * PI / 8) - x3 * cos(PI / 8) */
        odd1 = (tmp * DCT_COEF6_Q31 - odd0 * DCT_COEF2_Q31) >> 31;
        /* z2 = x1 * cos(PI / 8) + x3 * cos(3 * PI / 8) */
        odd0 = (tmp * DCT_COEF2_Q31 + odd0 * DCT_COEF6_Q31) >> 31;

        /* butterfly */
        src[0] = even0 + odd0;    /* y0 = z0 + z2 */
        src[1] = even1 + odd1;    /* y1 = z1 + z3 */
        src[2] = even1 - odd1;    /* y2 = z1 - z3 */
        src[3] = even0 - odd0;    /* y3 = z0 - z2 */
    }
}

static void riscv_dsp_idct_e_q31(q31_t *src, uint32_t m)
{
    uint32_t n = (1 << m);
    q31_t *ptre, *ptro;

    /* pre-processing - scale the first term */
    src[0] >>= 1;

    /* call the recursive work function */
    riscv_dsp_idct_o_q31(src, m);

    /* post-processing - scale by 2 / N */
    ptre = &src[0];
    ptro = &src[n];
    while (ptre != ptro)
    {
        *ptre++ <<= 1;
    }
}

static void riscv_dsp_dct_2o_q31(q31_t *src, uint32_t m)
{
    q63_t even0, even1;  /* Temporary variable */
    q63_t odd0, odd1, tmp;

    /* induction functions from 2, 3, 4, ..., FFT_LOGN */
    if (2 != m)
    {
        uint32_t n = 1 << m;
        uint32_t k = n >> 1;
        q31_t swap_buf[k];
        uint32_t factor;
        const unsigned *scale;
        q31_t *ptre = &src[0], *ptro = &src[n], *ptrz = &src[0];
        q31_t *ptrx = &swap_buf[0];

        /* N > 4: (N / 2) * log2(N) + N / 4 (multiplications/divisions)
         **        (2 * N) * log2(N) - 9 * N / 4 + 1
         **                              (additions/subtractions) */
        /* (only (3 * N / 2) * log2(N) - 5 * N / 4 + 1 if maintaining
         ** table of 1 / (2 * cosine)) */

        /* pre-processing - half the values */
        /* example of n = 8:
         * input:  [a, b, c, d, e, f, g, h]
         * output: [a>>1, c>>1, e>>1, g>>1, -, -, -, -] and
         *         [b>>1, (b>>1)+(d>>1), (d>>1)+(f>>1), (f>>1)+(h>>1)]
         */
        while (ptre != ptro)
        {
            *ptrz++ = *ptre++ >> 1;
            *ptrx++ = *ptre++ >> 1;
        }
        /* odd portion processing */
        ptro = &swap_buf[0];
        ptrx--;
        while (ptrx != ptro)
        {
            q31_t *ptrx2 = ptrx - 1;
            q31_t data = *ptrx;
            *ptrx-- = *ptrx2 + data;
        }

        /* even portion */
        riscv_dsp_dct_2o_q31(src, m - 1);

        /* odd portion */
        riscv_dsp_dct_2o_q31(swap_buf, m - 1);

        /* post-processing - shuffle */
        /* example of n = 8:
         * input:  [a, b, c, d, -, -, -, -] and [e, f, g, h]
         * output: [a, b, c, d, e, f, g, h]
         */
        ptro = &swap_buf[k];
        while (ptrx != ptro)
        {
            *ptrz++ = *--ptro;
        }

        /* weighted butterfly */
        factor = 1 << (RES_LOGN - 1 - m);
        scale = &riscv_dsp_cos_recip_table_q31[(factor >> 1)];

        ptre = &src[0];
        ptro = &src[n];
        while (ptre != ptro)
        {
            even0 = *ptre;
            odd0 = *--ptro;
            odd0 = (odd0 * (*scale)) >> (35 - RES_LOGN);
            *ptro = even0 - odd0;
            *ptre++ = even0 + odd0;
            scale += factor;
        }
    }
    else
    {
        /* N = 4: 5 multiplications/divisions */
        /*        8 additions/subtractions */

        /* even portion */
        even0 = src[0];
        tmp = ((q63_t)src[2] * DCT_COEF4_Q31) >> 31;
        even1 = even0 - tmp;    /* z1 = x0 - x2 * cos(PI / 4) */
        even0 = even0 + tmp;    /* z0 = x0 + x2 * cos(PI / 4) */

        /* odd portion */
        odd0 = src[3];
        tmp = src[1];
        /* z3 = x1 * cos(3 * PI / 8) - x3 * cos(PI / 8) */
        odd1 = (tmp * DCT_COEF6_Q31 - odd0 * DCT_COEF2_Q31) >> 31;
        /* z2 = x1 * cos(PI / 8) + x3 * cos(3 * PI / 8) */
        odd0 = (tmp * DCT_COEF2_Q31 + odd0 * DCT_COEF6_Q31) >> 31;

        /* butterfly */
        src[0] = (even0 + odd0) >> 1;    /* y0 = z0 + z2 */
        src[1] = (even1 + odd1) >> 1;    /* y1 = z1 + z3 */
        src[2] = (even1 - odd1) >> 1;    /* y2 = z1 - z3 */
        src[3] = (even0 - odd0) >> 1;    /* y3 = z0 - z2 */
    }
}

static void riscv_dsp_dct_o_q31(q31_t *src, uint32_t m)
{
    q63_t tmp;  /* Temporary variable */

    /* induction functions from 2, 3, 4, ..., FFT_LOGN */
    if (2 != m)
    {
        /* N > 4: (N / 2) * log2(N) + N / 4 (multiplications/divisions)
         **       (2 * N) * log2(N) - 9 * N / 4 + 1
         **                    (additions/subtractions) */
        /*  (only (3 * N / 2) * log2(N) - 5 * N / 4 + 1 if maintaining
         ** table of 1 / (2 * cosine)) */
        uint32_t n = 1 << m;
        uint32_t factor;
        const unsigned *scale;
        uint32_t k;
        k = n - 1;
        while (k != 0)
        {
            src[k] = (src[k] + src[k - 1]) >> 1;
            k--;
        }
        src[0] >>= 1;

        /* do idct */
        riscv_dsp_dct_2o_q31(src, m);

        /* post-processing */
        factor = 1 << (RES_LOGN - 2 - m);
        scale = &riscv_dsp_cos_recip_table_q31[(factor >> 1)];
        while (k != n)
        {
            src[k] = ((q63_t)src[k] * (*scale)) >> (35 - RES_LOGN);
            k++;
            scale += factor;
        }
    }
    else
    {
        /* N = 4: 8 multiplications/divisions
         **       12 additions/subtractions */
        q63_t w0, w1, w2, w3;  /* Temporary variable */

        /* pre-processing */
        w0 = src[0] >> 1;    /* w0 = x0 */
        w1 = src[1] >> 1;
        w2 = src[2] >> 1;
        w3 = src[3] >> 1;
        w3 += w2;            /* w3 = x2 + x3 */
        w2 += w1;            /* w2 = x1 + x2 */
        w1 += w0;            /* w1 = x0 + x1 */

        /* even portion */
        tmp = (w2 * DCT_COEF4_Q31) >> 31;
        w2 = (w0 - tmp) >> 1;    /* z1 = w0 - w2 * cos(PI / 4) */
        w0 = (w0 + tmp) >> 1;    /* z0 = w0 + w2 * cos(PI / 4) */

        /* odd portion */
        tmp = ((w1 + w3) * DCT_COEF4_Q31) >> 31;
        /* z2 = x1 * cos(3 * PI / 8) - x3 * cos(PI / 8) */
        w3 = ((w1 + tmp) * DCT_COEF2_R_Q31) >> 31;
        /* z3 = x1 * cos(PI / 8) + x3 * cos(3 * PI / 8) */
        w1 = ((w1 - tmp) * DCT_COEF6_R_Q31) >> 31;

        /* modified butterfly */
        /* y0 = z0 + z2 */
        src[0] = ((w0 + w3) * DCT_COEF1_R_Q31) >> 30;
        /* y3 = z0 - z2 */
        src[3] = ((w0 - w3) * DCT_COEF7_R_Q31) >> 30;
        /* y1 = z1 + z3 */
        src[1] = ((w2 + w1) * DCT_COEF3_R_Q31) >> 30;
        /* y2 = z1 - z3 */
        src[2] = ((w2 - w1) * DCT_COEF5_R_Q31) >> 30;
    }
}

static void riscv_dsp_dct_e_q31(q31_t *src, uint32_t m)
{
    q63_t tmp;

    /* induction functions from 2, 3, 4, ..., FFT_LOGN */
    if (2 != m)
    {
        /*  N > 4: (N / 2) * log2(N) + N / 4 (multiplications/divisions)
         **        (2 * N) * log2(N) - 9 * N / 4 + 1
         **                        (additions/subtractions) */
        /*   (only (3 * N / 2) * log2(N) - 5 * N / 4 + 1 if maintaining
         ** table of 1 / (2 * cosine)) */
        uint32_t n = (1 << m);
        uint32_t k = (n >> 1);
        q31_t swap_buf[k];
        q31_t *ptre = &src[0], *ptro = &src[k], *ptrz = &src[n - 1];
        q31_t *ptrx = &swap_buf[0];

        /* butterfly */
        /* example of n = 8:
         * input:  [a, b, c, d, e, f, g, h]
         * output: [(a+h)>>1, (b+g)>>1, (c+f)>>1, (d+e)>>1, -, -, -, -] and
         *         [(a-h)>>1, (b-g)>>1, (c-f)>>1, (d-e)>>1]
         */
        while (ptre != ptro)
        {
            q63_t data = *ptre;
            tmp = *ptrz--;
            *ptrx++ = (data - tmp) >> 1;
            *ptre++ = (data + tmp) >> 1;
        }

        /* even portion */
        riscv_dsp_dct_e_q31(&src[0], m - 1);

        /* odd portion */
        riscv_dsp_dct_o_q31(&swap_buf[0], m - 1);

        /* post-processing - shuffle the even/odd entries */
        /* example of n = 8:
         * input:  [a, b, c, d, -, -, -, -] and [e, f, g, h]
         * output: [a, e, b, f, c, g, d, h]
         */
        ptre = &src[n];
        ptro = &src[0];
        while (ptrz != ptro)
        {
            ptre -= 2;
            *ptre = *ptrz--;
        }
        ptre = &src[n - 1];
        ptro = &swap_buf[0];
        while (ptrx != ptro)
        {
            ptrx--;
            *ptre = *ptrx;
            ptre -= 2;
        }
    }
    else
    {
        /* N = 4: 5 (multiplications/divisions) */
        /*        8 (additions/subtractions) */
        q63_t w0, w1, w2, w3;

        /* butterfly */
        w0 = src[0];
        tmp = src[3];
        w2 = (w0 - tmp) >> 1;     /* w2 = x0 - x3 */
        w0 = (w0 + tmp) >> 1;     /* w0 = x0 + x3 */
        w1 = src[2];
        tmp = src[1];
        w3 = (tmp - w1) >> 1;     /* w3 = x1 - x2 */
        w1 = (tmp + w1) >> 1;     /* w1 = x1 + x2 */

        src[0] = (w0 + w1) >> 1;
        src[2] = ((w0 - w1) * DCT_COEF4_Q31) >> 32;

        /* odd portion */
        /* y1 = w2 * cos(PI / 8) + w3 * cos(3 * PI / 8) */
        src[1] = (w2 * DCT_COEF2_Q31 + w3 * DCT_COEF6_Q31) >> 32;
        /* y3 = w2 * cos(3 * PI / 8) - w3 * cos(PI / 8) */
        src[3] = (w2 * DCT_COEF6_Q31 - w3 * DCT_COEF2_Q31) >> 32;
    }
}

/**
 * @brief Function to implement the q31 DCT Functions
 * @param[in]       *src     points to the input block data.
 * @param[in]       m        If not defined STATIC_VERSION, the data buffer
 *                           is of size 2 * n while n = 2 ** m.
 * @return none.
 */

void riscv_dsp_dct_q31(q31_t *src, uint32_t m)
{
    riscv_dsp_dct_e_q31(src, m);
}

/**
 * @brief Function to implement the q31 IDCT Functions
 * @param[in]       *src     points to the input block data.
 * @param[in]       m        If not defined STATIC_VERSION, the data buffer
 *                           is of size 2 * n while n = 2 ** m.
 * @return none.
 */

void riscv_dsp_idct_q31(q31_t *src, uint32_t m)
{
    riscv_dsp_idct_e_q31(src, m);
}

/**
 * @} end of dct
 */
