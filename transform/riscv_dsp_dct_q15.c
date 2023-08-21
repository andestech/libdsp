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

/**
 * @ingroup transform
 */

/**
 * @addtogroup dct
 * @{
 */

void riscv_dsp_idct_o_q15(q15_t *src, uint32_t m)
{
    q31_t even0, even1;  /* Temporary variable */
    q31_t odd0, odd1, tmp;

    /* induction functions from 2, 3, 4, ..., FFT_LOGN */
    if (2 != m)
    {
        uint32_t n = 1 << m;
        uint32_t k = n >> 1;
        q15_t swap_buf[k];
        uint32_t factor;
        const unsigned short *scale;

        q15_t *ptre, *ptro, *ptrz, *ptrx, data;
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
        riscv_dsp_idct_o_q15(src, m - 1);

        /* odd portion */
        riscv_dsp_idct_o_q15(swap_buf, m - 1);

        ptrx--;
        while (ptrz != ptro)
        {
            *ptrz++ = *ptrx--;
        }

        /* weighted butterfly */
        factor = 1 << (RES_LOGN - 1 - m);
        scale = &riscv_dsp_cos_recip_table_q15[(factor >> 1)];
        uint32_t i = 0;
        while (i != k)
        {
            even0 = src[i];
            odd0 = (src[(n - 1) - i] * (*scale)) >> (19 - RES_LOGN);
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
        tmp = ((q31_t)src[2] * DCT_COEF4_Q15) >> 15;
        even1 = even0 - tmp;    /* z1 = x0 - x2 * cos(PI / 4) */
        even0 = even0 + tmp;    /* z0 = x0 + x2 * cos(PI / 4) */

        /* odd portion */
        odd0 = src[3];
        tmp = src[1];
        /* z3 = x1 * cos(3 * PI / 8) - x3 * cos(PI / 8) */
        odd1 = (tmp * DCT_COEF6_Q15 - odd0 * DCT_COEF2_Q15) >> 15;
        /* z2 = x1 * cos(PI / 8) + x3 * cos(3 * PI / 8) */
        odd0 = (tmp * DCT_COEF2_Q15 + odd0 * DCT_COEF6_Q15) >> 15;

        /* butterfly */
        src[0] = even0 + odd0;    /* y0 = z0 + z2 */
        src[1] = even1 + odd1;    /* y1 = z1 + z3 */
        src[2] = even1 - odd1;    /* y2 = z1 - z3 */
        src[3] = even0 - odd0;    /* y3 = z0 - z2 */
    }
}

static void riscv_dsp_idct_e_q15(q15_t *src, uint32_t m)
{
    uint32_t n = (1 << m), i = 0;

    /* scale the first term */
    src[0] >>= 1;

    /* call the recursive work function */
    riscv_dsp_idct_o_q15(src, m);

    /* scale by 2 / N */
    while (i != n)
    {
        src[i] = src[i] << 1;
        i++;
    }
}

static void riscv_dsp_dct_2o_q15(q15_t *src, uint32_t m)
{
    q31_t even0, even1;  /* Temporary variable */
    q31_t odd0, odd1, tmp;

    /* induction functions from 2, 3, 4, ..., FFT_LOGN */
    if (2 != m)
    {
        uint32_t n = 1 << m;
        uint32_t k = n >> 1;
        q15_t riscv_dsp_dct_swap_buffer[k];
        uint32_t factor;
        const unsigned short *scale;
        q15_t *ptre = &src[0], *ptro = &src[n], *ptrz = &src[0];
        q15_t *ptrx = &riscv_dsp_dct_swap_buffer[0];

        /* N > 4: (N / 2) * log2(N) + N / 4 (multiplications/divisions)
         **        (2 * N) * log2(N) - 9 * N / 4 + 1
         **                              (additions/subtractions) */
        /* (only (3 * N / 2) * log2(N) - 5 * N / 4 + 1 if maintaining
         ** table of 1 / (2 * cosine)) */

        /* pre-processing - half the values */
        while (ptre != ptro)
        {
            *ptrz++ = *ptre++ >> 1;
            *ptrx++ = *ptre++ >> 1;
        }
        /* odd portion processing */
        ptro = &riscv_dsp_dct_swap_buffer[0];
        ptrx--;
        while (ptrx != ptro)
        {
            q15_t *ptrx2 = ptrx - 1;
            q15_t data = *ptrx;
            *ptrx-- = *ptrx2 + data;
        }

        /* even portion */
        riscv_dsp_dct_2o_q15(src, m - 1);

        /* odd portion */
        riscv_dsp_dct_2o_q15(riscv_dsp_dct_swap_buffer, m - 1);

        /* post-processing - shuffle */
        ptro = &riscv_dsp_dct_swap_buffer[k];
        while (ptrx != ptro)
        {
            *ptrz++ = *--ptro;
        }

        /* weighted butterfly */
        factor = 1 << (RES_LOGN - 1 - m);
        scale = &riscv_dsp_cos_recip_table_q15[(factor >> 1)];
        uint32_t i = 0;
        while (i != k)
        {
            even0 = src[i];
            odd0 = (src[(n - 1) - i] * (*scale)) >> (19 - RES_LOGN);
            src[i] = even0 + odd0;
            src[(n - 1) - i] = even0 - odd0;
            scale += factor;
            i++;
        }
    }
    else
    {
        /* N = 4: 5 multiplications/divisions */
        /*        8 additions/subtractions */

        /* even portion */
        even0 = src[0];
        tmp = ((q31_t)src[2] * DCT_COEF4_Q15) >> 15;
        even1 = even0 - tmp;    /* z1 = x0 - x2 * cos(PI / 4) */
        even0 = even0 + tmp;    /* z0 = x0 + x2 * cos(PI / 4) */

        /* odd portion */
        odd0 = src[3];
        tmp = src[1];
        /* z3 = x1 * cos(3 * PI / 8) - x3 * cos(PI / 8) */
        odd1 = (tmp * DCT_COEF6_Q15 - odd0 * DCT_COEF2_Q15) >> 15;
        /* z2 = x1 * cos(PI / 8) + x3 * cos(3 * PI / 8) */
        odd0 = (tmp * DCT_COEF2_Q15 + odd0 * DCT_COEF6_Q15) >> 15;

        /* butterfly */
        src[0] = (even0 + odd0) >> 1;    /* y0 = z0 + z2 */
        src[1] = (even1 + odd1) >> 1;    /* y1 = z1 + z3 */
        src[2] = (even1 - odd1) >> 1;    /* y2 = z1 - z3 */
        src[3] = (even0 - odd0) >> 1;    /* y3 = z0 - z2 */
    }
}

static void riscv_dsp_dct_o_q15(q15_t *src, uint32_t m)
{
    q31_t tmp;  /* Temporary variable */

    /* induction functions from 2, 3, 4, ..., FFT_LOGN */
    if (2 != m)
    {
        /* N > 4: (N / 2) * log2(N) + N / 4 (multiplications/divisions)
         **       (2 * N) * log2(N) - 9 * N / 4 + 1
         **                    (additions/subtractions) */
        /*  (only (3 * N / 2) * log2(N) - 5 * N / 4 + 1 if maintaining
         ** table of 1 / (2 * cosine)) */
        uint32_t n = 1 << m;
        uint32_t factor, k;
        const unsigned short *scale;
        k = n - 1;
        while (k != 0)
        {
            src[k] = (src[k] + src[k - 1]) >> 1;
            k--;
        }
        src[0] >>= 1;

        /* do idct */
        riscv_dsp_dct_2o_q15(src, m);

        /* post-processing */
        factor = 1 << (RES_LOGN - 2 - m);
        scale = &riscv_dsp_cos_recip_table_q15[(factor >> 1)];
        while (k != n)
        {
            src[k] = ((src[k]) * (*scale)) >> (19 - RES_LOGN);
            k++;
            scale += factor;
        }
    }
    else
    {
        q31_t even0, even1;  /* Temporary variable */
        q31_t odd0, odd1;
        /* N = 4: 5 multiplications/divisions
         **       8 additions/subtractions */

        /* pre-processing */
        even0 = src[0] >> 1;             /* w0 = x0 */
        odd0 = src[1] >> 1;
        even1 = src[2] >> 1;
        odd1 = (src[3] >> 1) + even1;    /* w3 = x2 + x3 */
        even1 = even1 + odd0;            /* w2 = x1 + x2 */
        odd0 = odd0 + even0;             /* w1 = x0 + x1 */


        /* even portion */
        tmp = (even1 * DCT_COEF4_Q15) >> 15;
        even1 = (even0 - tmp) >> 1;    /* z1 = w0 - w2 * cos(PI / 4) */
        even0 = (even0 + tmp) >> 1;    /* z0 = w0 + w2 * cos(PI / 4) */

        /* odd portion */
        tmp = ((odd0 + odd1) * DCT_COEF4_Q15) >> 15;
        odd1 = ((odd0 + tmp) * DCT_COEF2_R_Q15) >> 18;
        odd0 = ((odd0 - tmp) * DCT_COEF6_R_Q15) >> 18;

        /* modified butterfly */
        /* y0 = z0 + z2 */
        src[0] = ((even0 + odd1) * DCT_COEF1_R_Q15) >> 17;
        src[3] = ((even0 - odd1) * DCT_COEF7_R_Q15) >> 17;
        src[1] = ((even1 + odd0) * DCT_COEF3_R_Q15) >> 17;
        src[2] = ((even1 - odd0) * DCT_COEF5_R_Q15) >> 17;
    }
}

static void riscv_dsp_dct_e_q15(q15_t *src, uint32_t m)
{
    q31_t tmp;

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
        q15_t riscv_dsp_dct_swap_buffer[k];
        q15_t *ptre = &src[0], *ptro = &src[k], *ptrz = &src[n - 1];
        q15_t *ptrx = &riscv_dsp_dct_swap_buffer[0];

        /* butterfly */
#ifdef ENA_DSP_ISA_32
        while (ptre + 4 <= ptro)
        {
            union32_t dual_e = simd_load_16(ptre, 0);
            union32_t dual_z = simd_load_16(ptrz - 1, 0);
            dual_z.l0 = NDS_DSP_SWAP16(dual_z.l0);

            uint32_t dual_x = NDS_DSP_RSUB16(dual_e.l0, dual_z.l0);
            dual_e.l0 = NDS_DSP_RADD16(dual_e.l0, dual_z.l0);
            simd_save_16(ptrx, dual_x);
            simd_save_16(ptre, dual_e.l0);

            ptrz -= 2;
            ptrx += 2;
            ptre += 2;
        }
#endif
        while (ptre != ptro)
        {
            q31_t data = *ptre;
            tmp = *ptrz--;
            *ptrx++ = (data - tmp) >> 1;
            *ptre++ = (data + tmp) >> 1;
        }
        /* After butterfly:
         * ptre = ptro = &src[k]
         * ptrz = &src[k - 1]
         * ptrx = &riscv_dsp_dct_swap_buffer[k]
         */

        /* even portion */
        riscv_dsp_dct_e_q15(&src[0], m - 1);

        /* odd portion */
        riscv_dsp_dct_o_q15(&riscv_dsp_dct_swap_buffer[0], m - 1);

        /* post-processing - shuffle the even/odd entries */
        ptre = &src[n];
        ptro = &src[0];
        while (ptrz != ptro)
        {
            ptre -= 2;
            *ptre = *ptrz--;
        }
        ptre = &src[n - 1];
        ptro = &riscv_dsp_dct_swap_buffer[0];
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
        q31_t x0, x1, x2, x3;

        /* butterfly */
        x0 = src[0];
        tmp = src[3];
        x1 = (x0 - tmp) >> 1;     /* w2 = x0 - x3 */
        x0 = (x0 + tmp) >> 1;     /* w0 = x0 + x3 */
        x2 = src[2];
        tmp = src[1];
        x3 = (tmp - x2) >> 1;     /* w3 = x1 - x2 */
        x2 = (x2 + tmp) >> 1;     /* w1 = x1 + x2 */

        /* even portion */
        src[0] = (x0 + x2) >> 1;                /* y0 = w0 + w1 */
        /* y2 = (w0 - w1) * cos(PI / 4) */
        src[2] = ((x0 - x2) * DCT_COEF4_Q15) >> 16;

        /* odd portion */
        /* y1 = w2 * cos(PI / 8) + w3 * cos(3 * PI / 8) */
        src[1] = (x1 * DCT_COEF2_Q15 + x3 * DCT_COEF6_Q15) >> 16;
        /* y3 = w2 * cos(3 * PI / 8) - w3 * cos(PI / 8) */
        src[3] = (x1 * DCT_COEF6_Q15 - x3 * DCT_COEF2_Q15) >> 16;
    }
}

/**
 * @brief Function to implement the q31 DCT Functions
 * @param[in]       *src     points to the input block data.
 * @param[in]       m        If not defined STATIC_VERSION, the data buffer
 *                           is of size 2 * n while n = 2 ** m.
 * @return none.
 */

void riscv_dsp_dct_q15(q15_t *src, uint32_t m)
{
    riscv_dsp_dct_e_q15(src, m);
}

/**
 * @brief Function to implement the q31 IDCT Functions
 * @param[in]       *src     points to the input block data.
 * @param[in]       m        If not defined STATIC_VERSION, the data buffer
 *                           is of size 2 * n while n = 2 ** m.
 * @return none.
 */

void riscv_dsp_idct_q15(q15_t *src, uint32_t m)
{
    riscv_dsp_idct_e_q15(src, m);
}

/**
 * @} end of dct
 */
