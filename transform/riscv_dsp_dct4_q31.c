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
#include "internal_isa.h"

extern void riscv_dsp_dct_q31(q31_t *src, uint32_t m);
extern void riscv_dsp_idct_o_q31(q31_t *src, uint32_t m);

/**
 * @ingroup transform
 */

/**
 * @addtogroup dct4
 * @{
 */

/**
 * @brief Function to implement the q31 DCT type IV function.
 * @param[in]       *src     points to the input block data.
 * @param[in]       m        If not defined STATIC_VERSION, the data buffer
 *                           is of size 2 * n while n = 2 ** m.
 * @return none.
 */

/* function description */
/**
 * @brief riscv_dsp_dct4_q31
 *
 *            N-1
 *     y[k] = sum x[n] * cos(PI * (2 * k + 1) * (2 * n + 1) / (4 * N))
 *            n=0
 *
 * Implementation of N such that
 *   1] N = 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, and 16384.
 *   2] m, defined as log2(N), is less than RES_LOGN.
 */

void riscv_dsp_dct4_q31(q31_t *src, uint32_t m)
{
    /* N > 4: (N / 2) * log2(N) + N / 4 multiplications/divisions */
    /*        (2 * N) * log2(N) - 9 * N / 4 + 1
     **                        additions/subtractions */
    /*  (only (3 * N / 2) * log2(N) - 5 * N / 4 + 1 if maintaining
     **      table of 1 / (2 * cosine)) */
    uint32_t n = (1 << m);
    uint32_t factor = 1 << (RES_LOGN - 2 - m);
    const q31_t *scale = &riscv_dsp_cos_table_q31_linear[(factor >> 1)];

    /* pre-processing */
    /* example of n = 8:
     * input:  [a, b, c, d, e, f, g, h]
     * output: [a*cos(1/32), b*cos(3/32),  c*cos(5/32),  d*cos(7/32),
     *          e*cos(9/32), f*cos(11/32), g*cos(13/32), h*cos(15/32)]
     */
    uint32_t k = 0;
    for (k = 0; k < n; k++, scale += factor)
    {
    src[k] = ((q63_t)src[k] * (*scale)) >> 31;
    }

    /* do dct */
    riscv_dsp_dct_q31(src, m);

    /* post-processing */
    /* example of n = 8:
     * input:  [a, b, c, d, e, f, g, h]
     * output: [A=a>>1, B=b-A, C=c-B, D=d-C, E=e-D, F=f-E, G=g-F, H=h-G]
     */
    src[0] >>= 1;
    for (k = 1; k < n; k++)
    {
        src[k] = src[k] - src[k - 1];
    }
}


/**
 * @brief riscv_dsp_idct4_q31
 *
 *            2   N-1
 *     y[k] = - * sum x[n] * cos(PI * (2 * k + 1) * (2 * n + 1) / (4 * N))
 *            N   n=0
 *
 * Functions include:
 *  1. riscv_dsp_idct4_o_q31 : Internal work function for DCT-IV, used by
 *                         riscv_dsp_idct4_e_q31.
 *  2. riscv_dsp_idct4_e_q31 : idct main function.
 */

/**
 * @brief riscv_dsp_idct4_o_q31
 */

static void riscv_dsp_idct4_o_q31(q31_t *src, uint32_t m)
{
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

        /* pre-processing */
        /* example of n = 8:
         * input:  [a, b, c, d, e, f, g, h]
         * output: [a, a+b, b+c, c+d, d+e, e+f, f+g, g+h]
         */
        uint32_t k;
        k = n - 1;
        while (k != 0)
        {
        src[k] = src[k] + src[k - 1];
            k--;
        }

        /* do idct */
        riscv_dsp_idct_o_q31(src, m);

        /* post-processing */
        /* example of n = 8:
         * input:  [a, b, c, d, e, f, g, h]
         * output: [a/cos(1/32), b/cos(3/32),  c/cos(5/32),  d/cos(7/32),
         *          e/cos(9/32), f/cos(11/32), g/cos(13/32), h/cos(15/32)]
         */
        factor = 1 << (RES_LOGN - 2 - m);
        scale = &riscv_dsp_cos_recip_table_q31[(factor >> 1)];
        while (k != n)
        {
            src[k] = ((q63_t)(src[k]) * (*scale)) >> (35 - RES_LOGN);
            k++;
            scale += factor;
        }
    }
    else
    {
        /* N = 4: 8 multiplications/divisions
         **       12 additions/subtractions */
        q63_t w0, w1, w2, w3, tmp;  /* Temporary variable */

        /* pre-processing */
        w0 = src[0];    /* w0 = x0 */
        w1 = src[1];
        w2 = src[2];
        w3 = src[3];
        w3 += w2;       /* w3 = x2 + x3 */
        w2 += w1;       /* w2 = x1 + x2 */
        w1 += w0;       /* w1 = x0 + x1 */

        /* even portion */
        tmp = (w2 * DCT_COEF4_Q31) >> 31;
        w2 = w0 - tmp;    /* z1 = w0 - w2 * cos(PI / 4) */
        w0 = w0 + tmp;    /* z0 = w0 + w2 * cos(PI / 4) */

        /* odd portion */
        tmp = ((w1 + w3) * DCT_COEF4_Q31) >> 31;
        w3 = ((w1 + tmp) * DCT_COEF2_R_Q31) >> 30;
        w1 = ((w1 - tmp) * DCT_COEF6_R_Q31) >> 30;

        /* modified butterfly */
        src[0] = ((w0 + w3) * DCT_COEF1_R_Q31) >> 30;
        src[3] = ((w0 - w3) * DCT_COEF7_R_Q31) >> 30;
        src[1] = ((w2 + w1) * DCT_COEF3_R_Q31) >> 30;
        src[2] = ((w2 - w1) * DCT_COEF5_R_Q31) >> 30;
    }
}
/**
 * @brief riscv_dsp_idct4_e_q31
 */

static void riscv_dsp_idct4_e_q31(q31_t *src, uint32_t m)
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
        q31_t *ptre = &src[0], *ptro = &src[n], *ptrz;
        q31_t *ptrx = &swap_buf[0];

        /* pre-processing */
        /* example of n = 8:
         * input:  [a, b, c, d, e, f, g, h]
         * output: [a+h, b+g, c+f, d+e, -, -, -, -] and [a-h, b-g, c-f, d-e]
         */
        do
        {
            q63_t data = *ptre;

            ptro--;
            tmp = *ptro;
            *ptrx++ = data - tmp;
            *ptre++ = data + tmp;
        }
        while (ptre != ptro);

        /* even portion */
        riscv_dsp_idct4_e_q31(src, m - 1);

        /* odd portion */
        riscv_dsp_idct4_o_q31(swap_buf, m - 1);

        /* post-processing - shuffle */
        /* example of n = 8:
         * input:  [a, b, c, d, -, -, -, -] and [e, f, g, h]
         * output: [a, e, b, f, c, g, d, h]
         */
        ptrx--;
        ptre--;
        ptro = &src[0];
        ptrz = &src[n - 1];
        do
        {
            *ptrz-- = *ptrx--;
            *ptrz-- = *ptre--;
        }
        while (ptre != ptro);
        *ptrz = *ptrx;
    }
    else
    {
        /* N = 4: 5 (multiplications/divisions) */
        /*        8 (additions/subtractions) */
        q63_t w0, w1, w2, w3;

        /* butterfly */
        w0 = src[0];
        tmp = src[3];
        w2 = w0 - tmp;     /* w2 = x0 - x3 */
        w0 = w0 + tmp;     /* w0 = x0 + x3 */
        w1 = src[2];
        tmp = src[1];
        w3 = tmp - w1;     /* w3 = x1 - x2 */
        w1 = tmp + w1;     /* w1 = x1 + x2 */

        /* even portion */
        src[0] = w0 + w1;                /* y0 = w0 + w1 */
        /* y2 = (w0 - w1) * cos(PI / 4) */
        src[2] = ((w0 - w1) * DCT_COEF4_Q31) >> 31;

        /* odd portion */
        /* y1 = w2 * cos(PI / 8) + w3 * cos(3 * PI / 8) */
        src[1] = (w2 * DCT_COEF2_Q31 + w3 * DCT_COEF6_Q31) >> 31;
        /* y3 = w2 * cos(3 * PI / 8) - w3 * cos(PI / 8) */
        src[3] = (w2 * DCT_COEF6_Q31 - w3 * DCT_COEF2_Q31) >> 31;
    }
}

/**
 * @brief riscv_dsp_idct4_q31
 */

void riscv_dsp_idct4_q31(q31_t *src, uint32_t m)
{
    /* DCT_IV */
    uint32_t n = (1 << m);
    uint32_t factor = 1 << (RES_LOGN - 2 - m);
    const q31_t *scale = &riscv_dsp_cos_table_q31_linear[(factor >> 1)];

    /* pre-processing */
    /* example of n = 8:
     * input:  [a, b, c, d, e, f, g, h]
     * output: [a*cos(1/32), b*cos(3/32),  c*cos(5/32),  d*cos(7/32),
     *          e*cos(9/32), f*cos(11/32), g*cos(13/32), h*cos(15/32)]
     */
    uint32_t k;
    for (k = 0; k < n; k++, scale += factor)
    {
    src[k] = ((q63_t)src[k] * (*scale)) >> 31;
    }

    riscv_dsp_idct4_e_q31(src, m);

    /* post-processing */
    /* example of n = 8:
     * input:  [a, b, c, d, e, f, g, h]
     * output: [A=a>>1, B=b-A, C=c-B, D=d-C, E=e-D, F=f-E, G=g-F, H=h-G]
     */
    q31_t data = src[0] << 2;
    for (k = 1; k < n; k++, scale += factor)
    {
        data = (src[k] << 3) - data;
        src[k] = data;
    }

}

/**
 * @} end of dct4
 */
