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
 * @defgroup cfft_radix2 Radix-2 Complex FFT Functions
 *
 * Complex Fast Fourier Transform(CFFT) and Complex Inverse Fast Fourier
 * Transformm(CIFFT) is an efficient algorithm to compute Discrete Fourier
 * Transform(DFT) and Inverse Discrete Fourier Transform(IDFT). This
 * function is the implementaion of a very simple Cooley-Tukey Radix-2
 * DIF Complex FFT.
 *
 */

/**
 * @addtogroup cfft_radix2
 * @{
 */

/**
 * @brief Function to implement the fp16 Radix-2 Complex FFT
 *        Functions
 * @param[in]       *src     points to the input block data src[2 * k] and
 *                           src[2 * k + 1] contain the real and imaginary
 *                           parts of the k-th data.
 * @param[in]       m        If not defined STATIC_VERSION, the data buffer
 *                           is of size 2 * n while n = 2 ** m.
 * @return valaue   0  success.
 *                  -1 invalid range.
 */

/* function description */
int32_t riscv_dsp_cfft_rd2_f16(float16_t *src, uint32_t m)
{
    uint32_t j, k, n2, ie, ia;
    float16_t c, s, xa, xb, ya, yb;
    float16_t *ptrs, *ptre;
    float16_t xt, yt;
    uint32_t n;

#ifdef CHECK_RANGE
    /* Quick return for m < 3 or m > 14 */
    if (m < 3 || m > 14)
    {
        /* only support n = 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096,
         ** 8192, and 16384. that is, m = 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,
         ** 13, and 14. */
        return -1;
    }
#endif /* CHECK_RANGE */

    n = 1 << m;

    /* Main loop */
#if FFT_LOGN > RES_LOGN
    float16_t p = TWO_PI / n;
#endif /* FFT_LOGN > RES_LOGN */

    /* for first stage, 2 * n2 == n, thus the inner for loop only
     ** execute once. */
    n2 = n;
    ptre = &src[n];

    ptrs = &src[0];
    /* a = xa + ya * i and b = xb + yb * i */
    xa = ptrs[0];
    ya = ptrs[1];
    xb = ptre[0];
    yb = ptre[1];
    /* xa' = xa + xb */
    ptrs[0] = (xa + xb);
    /* ya' = ya + yb */
    ptrs[1] = (ya + yb);
    /* xb' = (xa - xb) * cos + (ya - yb) * sin */
    ptre[0] = (xa - xb);
    /* yb' = (ya - yb) * cos + (xa - xb) * sin */
    ptre[1] = (ya - yb);

    ia = 0;
    ptrs += 2;
    while (ptrs != ptre)
    {
        float16_t *ptr;

        /* get the cosine and sine values. */
        ia++;
#if FFT_LOGN > RES_LOGN
        GET_COS_SIN_VALUES(ia, c, s, p, f16);
#else
        GET_COS_SIN_VALUES(ia, c, s, m, f16);
#endif /* FFT_LOGN > RES_LOGN */

        /* a = xa + ya * i and b = xb + yb * i */
        ptr = ptrs + n;
        xa = ptrs[0];
        ya = ptrs[1];
        xb = ptr[0];
        yb = ptr[1];
        xt = xa - xb;       /* xt = xa - xb */
        /* xa' = xa + xb */
        ptrs[0] = (xa + xb);
        yt = ya - yb;       /* yt = ya - yb */
        /* ya' = ya + yb */
        ptrs[1] = (ya + yb);
        /* xb' = (xa - xb) * cos + (ya - yb) * sin */
        ptr[0] = (c * xt + s * yt);
        /* yb' = (ya - yb) * cos + (xa - xb) * sin */
        ptr[1] = (c * yt - s * xt);

        ptrs += 2;
    }

    /* for middle stages */
    ie = 2;
    k = m - 2;
    while (k)
    {
        float16_t *ptr1, *ptr2;

        n2 >>= 1;
        ptrs = &src[0];
        ptre = &src[n2];

        ptr1 = ptrs;
        j = ie;
        while (j)
        {
            /* a = xa + ya * i and b = xb + yb * i */
            ptr2 = ptr1 + n2;
            xa = ptr1[0];
            ya = ptr1[1];
            xb = ptr2[0];
            yb = ptr2[1];
            /* xa' = xa + xb */
            ptr1[0] = (xa + xb);
            /* ya' = ya + yb */
            ptr1[1] = (ya + yb);
            /* xb' = (xa - xb) * cos + (ya - yb) * sin */
            ptr2[0] = (xa - xb);
            /* yb' = (ya - yb) * cos + (xa - xb) * sin */
            ptr2[1] = (ya - yb);
            ptr1 = ptr2 + n2;
            j--;
        }

        ia = 0;
        ptrs += 2;
        while (ptrs != ptre)
        {
            /* get the cosine and sine values. */
            ia += ie;
#if FFT_LOGN > RES_LOGN
            GET_COS_SIN_VALUES(ia, c, s, p, f16);
#else
            GET_COS_SIN_VALUES(ia, c, s, m, f16);
#endif /* FFT_LOGN > RES_LOGN */

            ptr1 = ptrs;
            j = ie;
            while (j)
            {
                /* a = xa + ya * i and b = xb + yb * i */
                ptr2 = ptr1 + n2;
                xa = ptr1[0];
                ya = ptr1[1];
                xb = ptr2[0];
                yb = ptr2[1];
                xt = xa - xb;             /* xt = xa - xb */
                /* xa' = xa + xb */
                ptr1[0] = (xa + xb);
                yt = ya - yb;             /* yt = ya - yb */
                /* ya' = ya + yb */
                ptr1[1] = (ya + yb);
                /* xb' = (xa - xb) * cos + (ya - yb) * sin */
                ptr2[0] = (c * xt + s * yt);
                /* yb' = (ya - yb) * cos + (xa - xb) * sin */
                ptr2[1] = (c * yt - s * xt);
                ptr1 = ptr2 + n2;
                j--;
            }
            ptrs += 2;
        }
        ie <<= 1;
        k--;
    }

    /* for last stage, n2 == 1; thus the outer for loop only
     * execute onece. also, cos(0) == 1 and sin(0) == 0. */
    ptrs = &src[0];
    ptre = &src[2 * n];
    while (ptrs != ptre)
    {
        xa = ptrs[0];
        ya = ptrs[1];
        xb = ptrs[2];
        yb = ptrs[3];
        ptrs[0] = (xa + xb);    /* xa' = xa + xb */
        ptrs[1] = (ya + yb);    /* ya' = ya + yb */
        ptrs[2] = (xa - xb);    /* xb' = (xa - xb) * cos + (ya - yb) * sin */
        ptrs[3] = (ya - yb);    /* yb' = (ya - yb) * cos + (xa - xb) * sin */
        xa = ptrs[4];
        ya = ptrs[5];
        xb = ptrs[6];
        yb = ptrs[7];
        ptrs[4] = (xa + xb);    /* xa' = xa + xb */
        ptrs[5] = (ya + yb);    /* ya' = ya + yb */
        ptrs[6] = (xa - xb);    /* xb' = (xa - xb) * cos + (ya - yb) * sin */
        ptrs[7] = (ya - yb);    /* yb' = (ya - yb) * cos + (xa - xb) * sin */
        ptrs += 8;
    }

    /* Bit reversal permutation */
    return riscv_dsp_bit_reversal_f16 (src, m);
}

/**
 * @brief Function to implement the fp16 Radix-2 Complex IFFT
 *        Functions
 * @param[in]       *src     points to the input block data src[2 * k] and
 *                           src[2 * k + 1] contain the real and imaginary
 *                           parts of the k-th data.
 * @param[in]       m        If not defined STATIC_VERSION, the data buffer
 *                           is of size 2 * n while n = 2 ** m.
 * @return valaue   0  success.
 *                  -1 invalid range.
 */

int32_t riscv_dsp_cifft_rd2_f16(float16_t *src, uint32_t m)
{
    uint32_t j, k, n2, ie, ia;
    float16_t c, s, xa, ya, xb, yb;
    float16_t *ptrs, *ptre;
    float16_t xt, yt;
    uint32_t n;

#ifdef CHECK_RANGE
    /* Quick return for m < 3 or m > 14 */
    if (m < 3 || m > 14)
    {
        /* only support n = 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096,
         ** 8192, and 16384. that is, m = 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,
         ** 13, and 14. */
        return -1;
    }
#endif /* CHECK_RANGE */

    n = 1 << m;

    /* Main loop */
#if FFT_LOGN > RES_LOGN
    float16_t p = TWO_PI / n;
#endif /* FFT_LOGN > RES_LOGN */

    /* for first stage, 2 * n2 == n, thus the inner for loop only
     ** execute once. */
    n2 = n;
    ptre = &src[n];
    ptrs = &src[0];
    /* a = xa + ya * i and b = xb + yb * i */
    xa = ptrs[0];
    ya = ptrs[1];
    xb = ptre[0];
    yb = ptre[1];
    /* xa' = xa + xb */
    ptrs[0] = xa + xb;
    /* ya' = ya + yb */
    ptrs[1] = -yb - ya;
    /* xb' = (xa - xb) * cos + (ya - yb) * sin */
    ptre[0] = xa - xb;
    /* yb' = (ya - yb) * cos + (xa - xb) * sin */
    ptre[1] = yb - ya;

    ia = 0;
    ptrs += 2;
    while (ptrs != ptre)
    {
        float16_t *ptr1;

        /* get the cosine and sine values. */
        ia++;
#if FFT_LOGN > RES_LOGN
        GET_COS_SIN_VALUES(ia, c, s, p, f16);
#else
        GET_COS_SIN_VALUES(ia, c, s, m, f16);
#endif /* FFT_LOGN > RES_LOGN */

        /* a = xa + ya * i and b = xb + yb * i */
        ptr1 = ptrs + n;
        xa = ptrs[0];
        ya = ptrs[1];
        xb = ptr1[0];
        yb = ptr1[1];
        xt = xa - xb;           /* xt = xa - xb */
        /* xa' = xa + xb */
        ptrs[0] = xa + xb;
        yt = yb - ya;    /* yt = ya - yb */
        /* ya' = ya + yb */
        ptrs[1] = -yb - ya;
        /* xb' = (xa - xb) * cos + (ya - yb) * sin */
        ptr1[0] = (c * xt + s * yt);
        /* yb' = (ya - yb) * cos + (xa - xb) * sin */
        ptr1[1] = (c * yt - s * xt);

        ptrs += 2;
    }

    /* for middle stages */
    ie = 2;
    k = m - 2;
    while (k)
    {
        float16_t *ptr1, *ptr2;

        n2 >>= 1;
        ptrs = &src[0];
        ptre = &src[n2];

        ptr1 = ptrs;
        j = ie;
        while (j)
        {
            /* a = xa + ya * i and b = xb + yb * i */
            ptr2 = ptr1 + n2;
            xa = ptr1[0];
            ya = ptr1[1];
            xb = ptr2[0];
            yb = ptr2[1];
            /*  xa' = xa + xb */
            ptr1[0] = xa + xb;
            /* ya' = ya + yb */
            ptr1[1] = ya + yb;
            /* xb' = (xa - xb) * cos + (ya - yb) * sin */
            ptr2[0] = xa - xb;
            /* yb' = (ya - yb) * cos + (xa - xb) * sin */
            ptr2[1] = ya - yb;
            ptr1 = ptr2 + n2;
            j--;
        }

        ia = 0;
        ptrs += 2;
        while (ptrs != ptre)
        {
            /* get the cosine and sine values. */
            ia += ie;
#if FFT_LOGN > RES_LOGN
            GET_COS_SIN_VALUES(ia, c, s, p, f16);
#else
            GET_COS_SIN_VALUES(ia, c, s, m, f16);
#endif /* FFT_LOGN > RES_LOGN */

            ptr1 = ptrs;
            j = ie;
            while (j)
            {
                /* a = xa + ya * i and b = xb + yb * i */
                ptr2 = ptr1 + n2;
                xa = ptr1[0];
                ya = ptr1[1];
                xb = ptr2[0];
                yb = ptr2[1];
                xt = xa - xb;       /* xt = xa - xb */
                /* xa' = xa + xb */
                ptr1[0] = xa + xb;
                yt = ya - yb;       /* yt = ya - yb */
                /* ya' = ya + yb */
                ptr1[1] = ya + yb;
                /* xb' = (xa - xb) * cos + (ya - yb) * sin */
                ptr2[0] = (c * xt + s * yt);
                /* yb' = (ya - yb) * cos + (xa - xb) * sin */
                ptr2[1] = (c * yt - s * xt);
                ptr1 = ptr2 + n2;
                j--;
            }
            ptrs += 2;
        }
        ie <<= 1;
        k--;
    }

    /* for last stage, n2 == 1; thus the outer for loop only
     * execute onece. also, cos(0) == 1 and sin(0) == 0. */
    ptrs = &src[0];
    float16_t pp = riscv_dsp_recip_table_f16[m - 1];
    ptre = &src[2 * n];
    while (ptrs != ptre)
    {
        xa = ptrs[0];
        ya = ptrs[1];
        xb = ptrs[2];
        yb = ptrs[3];
        ptrs[0] = (xa + xb) * pp;   /* xa' = xa + xb */
        ptrs[1] = (-yb - ya) * pp;  /* ya' = ya + yb */
        ptrs[2] = (xa - xb) * pp;   /* xb' = (xa - xb) * cos + (ya - yb) * sin */
        ptrs[3] = (yb - ya) * pp;   /* yb' = (ya - yb) * cos + (xa - xb) * sin */
        xa = ptrs[4];
        ya = ptrs[5];
        xb = ptrs[6];
        yb = ptrs[7];
        ptrs[4] = (xa + xb) * pp;   /* xa' = xa + xb */
        ptrs[5] = (-yb - ya) * pp;  /* ya' = ya + yb */
        ptrs[6] = (xa - xb) * pp;   /* xb' = (xa - xb) * cos + (ya - yb) * sin */
        ptrs[7] = (yb - ya) * pp;   /* yb' = (ya - yb) * cos + (xa - xb) * sin */
        ptrs += 8;
    }

    /* Bit reversal permutation */
    return riscv_dsp_bit_reversal_f16 (src, m);
}


int32_t riscv_dsp_cfft_8pt_f16(float16_t *src, uint32_t m)
{
    (void) m;
    float16_t tmp_cal_float_r[8];
    float16_t tmp_cal_float_i[8];
    float16_t TW_81 = (float16_t)0.70710678f;
    float16_t tmp_cpx_r[16];
    float16_t tmp_cpx_i[16];

    tmp_cal_float_r[0] = src[0] + src[8];
    tmp_cal_float_i[0] = src[1] + src[9];
    tmp_cal_float_r[1] = src[0] - src[8];
    tmp_cal_float_i[1] = src[1] - src[9];
    tmp_cal_float_r[2] = src[2] + src[10];
    tmp_cal_float_i[2] = src[3] + src[11];
    tmp_cal_float_r[3] = src[2] - src[10];
    tmp_cal_float_i[3] = src[3] - src[11];
    tmp_cal_float_r[4] = src[4] + src[12];
    tmp_cal_float_i[4] = src[5] + src[13];
    tmp_cal_float_r[5] = src[4] - src[12];
    tmp_cal_float_i[5] = src[5] - src[13];
    tmp_cal_float_r[6] = src[6] + src[14];
    tmp_cal_float_i[6] = src[7] + src[15];
    tmp_cal_float_r[7] = src[6] - src[14];
    tmp_cal_float_i[7] = src[7] - src[15];

    tmp_cpx_r[0] = tmp_cal_float_r[0];
    tmp_cpx_i[0] = tmp_cal_float_i[0];
    tmp_cpx_r[1] = tmp_cal_float_r[1];
    tmp_cpx_i[1] = tmp_cal_float_i[1];
    tmp_cpx_r[2] = tmp_cal_float_r[2];
    tmp_cpx_i[2] = tmp_cal_float_i[2];
    tmp_cpx_r[4] = tmp_cal_float_r[4];
    tmp_cpx_i[4] = tmp_cal_float_i[4];
    tmp_cpx_r[6] = tmp_cal_float_r[6];
    tmp_cpx_i[6] = tmp_cal_float_i[6];
    tmp_cpx_r[5] = tmp_cal_float_i[5];
    tmp_cpx_i[5] = -tmp_cal_float_r[5];
    tmp_cpx_r[3] = (tmp_cal_float_r[3] + tmp_cal_float_i[3]) * TW_81;
    tmp_cpx_i[3] = (tmp_cal_float_i[3] - tmp_cal_float_r[3]) * TW_81;
    tmp_cpx_r[7] = (tmp_cal_float_r[7] - tmp_cal_float_i[7]) * TW_81;
    tmp_cpx_i[7] = (tmp_cal_float_i[7] + tmp_cal_float_r[7]) * TW_81;

    tmp_cpx_r[8] = tmp_cpx_r[0] + tmp_cpx_r[4];
    tmp_cpx_i[8] = tmp_cpx_i[0] + tmp_cpx_i[4];
    tmp_cpx_r[9] = tmp_cpx_r[1] + tmp_cpx_r[5];
    tmp_cpx_i[9] = tmp_cpx_i[1] + tmp_cpx_i[5];
    tmp_cpx_r[10] = tmp_cpx_r[0] - tmp_cpx_r[4];
    tmp_cpx_i[10] = tmp_cpx_i[0] - tmp_cpx_i[4];
    tmp_cpx_r[11] = tmp_cpx_r[1] - tmp_cpx_r[5];
    tmp_cpx_i[11] = tmp_cpx_i[1] - tmp_cpx_i[5];

    tmp_cpx_r[12] = tmp_cpx_r[2] + tmp_cpx_r[6];
    tmp_cpx_i[12] = tmp_cpx_i[2] + tmp_cpx_i[6];
    tmp_cpx_r[13] = tmp_cpx_r[3] - tmp_cpx_r[7];
    tmp_cpx_i[13] = tmp_cpx_i[3] - tmp_cpx_i[7];
    tmp_cpx_r[14] = tmp_cpx_r[2] - tmp_cpx_r[6];
    tmp_cpx_i[14] = tmp_cpx_i[2] - tmp_cpx_i[6];
    tmp_cpx_r[15] = tmp_cpx_r[3] + tmp_cpx_r[7];
    tmp_cpx_i[15] = tmp_cpx_i[3] + tmp_cpx_i[7];

    src[0]  = tmp_cpx_r[8] + tmp_cpx_r[12];
    src[1]  = tmp_cpx_i[8] + tmp_cpx_i[12];
    src[2]  = tmp_cpx_r[9] + tmp_cpx_r[13];
    src[3]  = tmp_cpx_i[9] + tmp_cpx_i[13];
    src[4]  = tmp_cpx_r[10] + tmp_cpx_i[14];
    src[5]  = tmp_cpx_i[10] - tmp_cpx_r[14];
    src[6]  = tmp_cpx_r[11] + tmp_cpx_i[15];
    src[7]  = tmp_cpx_i[11] - tmp_cpx_r[15];
    src[8]  = tmp_cpx_r[8] - tmp_cpx_r[12];
    src[9]  = tmp_cpx_i[8] - tmp_cpx_i[12];
    src[10] = tmp_cpx_r[9] - tmp_cpx_r[13];
    src[11] = tmp_cpx_i[9] - tmp_cpx_i[13];
    src[12] = tmp_cpx_r[10] - tmp_cpx_i[14];
    src[13] = tmp_cpx_i[10] + tmp_cpx_r[14];
    src[14] = tmp_cpx_r[11] - tmp_cpx_i[15];
    src[15] = tmp_cpx_i[11] + tmp_cpx_r[15];

    return 0;
}

int32_t riscv_dsp_cfft_4pt_f16(float16_t *src, uint32_t m)
{
    // used for 3pts rfft
    (void) m;
    float16_t tmp_cal_float_r[4];
    float16_t tmp_cal_float_i[4];

    tmp_cal_float_r[0] = src[0] + src[4];
    tmp_cal_float_i[0] = src[1] + src[5];
    tmp_cal_float_r[1] = src[0] - src[4];
    tmp_cal_float_i[1] = src[1] - src[5];

    tmp_cal_float_r[2] = src[2] + src[6];
    tmp_cal_float_i[2] = src[3] + src[7];
    tmp_cal_float_r[3] = src[2] - src[6];
    tmp_cal_float_i[3] = src[3] - src[7];

    src[0] = tmp_cal_float_r[0] + tmp_cal_float_r[2];
    src[1] = tmp_cal_float_i[0] + tmp_cal_float_i[2];

    src[2] = tmp_cal_float_r[1] + tmp_cal_float_i[3];
    src[3] = tmp_cal_float_i[1] - tmp_cal_float_r[3];

    src[4] = tmp_cal_float_r[0] - tmp_cal_float_r[2];
    src[5] = tmp_cal_float_i[0] - tmp_cal_float_i[2];

    src[6] = tmp_cal_float_r[1] - tmp_cal_float_i[3];
    src[7] = tmp_cal_float_i[1] + tmp_cal_float_r[3];

    return 0;

}

int32_t riscv_dsp_cifft_8pt_f16(float16_t *src, uint32_t m)
{
    (void) m;
    float16_t tmp_cal_float_r[8];
    float16_t tmp_cal_float_i[8];
    float16_t TW_81 = (float16_t)0.70710678f;
    float16_t tmp_cpx_r[16];
    float16_t tmp_cpx_i[16];
    float16_t inv_factor = (float16_t)1.0f/ 8.0f ;

    tmp_cal_float_r[0] = src[0] + src[8];
    tmp_cal_float_i[0] = src[1] + src[9];
    tmp_cal_float_r[1] = src[0] - src[8];
    tmp_cal_float_i[1] = src[1] - src[9];
    tmp_cal_float_r[2] = src[2] + src[10];
    tmp_cal_float_i[2] = src[3] + src[11];
    tmp_cal_float_r[3] = src[2] - src[10];
    tmp_cal_float_i[3] = src[3] - src[11];
    tmp_cal_float_r[4] = src[4] + src[12];
    tmp_cal_float_i[4] = src[5] + src[13];
    tmp_cal_float_r[5] = src[4] - src[12];
    tmp_cal_float_i[5] = src[5] - src[13];
    tmp_cal_float_r[6] = src[6] + src[14];
    tmp_cal_float_i[6] = src[7] + src[15];
    tmp_cal_float_r[7] = src[6] - src[14];
    tmp_cal_float_i[7] = src[7] - src[15];

    tmp_cpx_r[0] = tmp_cal_float_r[0];
    tmp_cpx_i[0] = tmp_cal_float_i[0];
    tmp_cpx_r[1] = tmp_cal_float_r[1];
    tmp_cpx_i[1] = tmp_cal_float_i[1];
    tmp_cpx_r[2] = tmp_cal_float_r[2];
    tmp_cpx_i[2] = tmp_cal_float_i[2];
    tmp_cpx_r[4] = tmp_cal_float_r[4];
    tmp_cpx_i[4] = tmp_cal_float_i[4];
    tmp_cpx_r[6] = tmp_cal_float_r[6];
    tmp_cpx_i[6] = tmp_cal_float_i[6];
    tmp_cpx_r[5] = -tmp_cal_float_i[5];
    tmp_cpx_i[5] = tmp_cal_float_r[5];
    tmp_cpx_r[3] = (tmp_cal_float_r[3] - tmp_cal_float_i[3]) * TW_81;
    tmp_cpx_i[3] = (tmp_cal_float_i[3] + tmp_cal_float_r[3]) * TW_81;
    tmp_cpx_r[7] = (tmp_cal_float_r[7] + tmp_cal_float_i[7]) * TW_81;
    tmp_cpx_i[7] = (tmp_cal_float_i[7] - tmp_cal_float_r[7]) * TW_81;

    tmp_cpx_r[8] = tmp_cpx_r[0] + tmp_cpx_r[4];
    tmp_cpx_i[8] = tmp_cpx_i[0] + tmp_cpx_i[4];
    tmp_cpx_r[9] = tmp_cpx_r[1] + tmp_cpx_r[5];
    tmp_cpx_i[9] = tmp_cpx_i[1] + tmp_cpx_i[5];
    tmp_cpx_r[10] = tmp_cpx_r[0] - tmp_cpx_r[4];
    tmp_cpx_i[10] = tmp_cpx_i[0] - tmp_cpx_i[4];
    tmp_cpx_r[11] = tmp_cpx_r[1] - tmp_cpx_r[5];
    tmp_cpx_i[11] = tmp_cpx_i[1] - tmp_cpx_i[5];

    tmp_cpx_r[12] = tmp_cpx_r[2] + tmp_cpx_r[6];
    tmp_cpx_i[12] = tmp_cpx_i[2] + tmp_cpx_i[6];
    tmp_cpx_r[13] = tmp_cpx_r[3] - tmp_cpx_r[7];
    tmp_cpx_i[13] = tmp_cpx_i[3] - tmp_cpx_i[7];
    tmp_cpx_r[14] = tmp_cpx_r[2] - tmp_cpx_r[6];
    tmp_cpx_i[14] = tmp_cpx_i[2] - tmp_cpx_i[6];
    tmp_cpx_r[15] = tmp_cpx_r[3] + tmp_cpx_r[7];
    tmp_cpx_i[15] = tmp_cpx_i[3] + tmp_cpx_i[7];

    src[0]  = (tmp_cpx_r[8] + tmp_cpx_r[12]) * inv_factor;
    src[1]  = (tmp_cpx_i[8] + tmp_cpx_i[12]) * inv_factor;
    src[2]  = (tmp_cpx_r[9] + tmp_cpx_r[13]) * inv_factor;
    src[3]  = (tmp_cpx_i[9] + tmp_cpx_i[13]) * inv_factor;
    src[4]  = (tmp_cpx_r[10] - tmp_cpx_i[14]) * inv_factor;
    src[5]  = (tmp_cpx_i[10] + tmp_cpx_r[14]) * inv_factor;
    src[6]  = (tmp_cpx_r[11] - tmp_cpx_i[15]) * inv_factor;
    src[7]  = (tmp_cpx_i[11] + tmp_cpx_r[15]) * inv_factor;
    src[8]  = (tmp_cpx_r[8] - tmp_cpx_r[12]) * inv_factor;
    src[9]  = (tmp_cpx_i[8] - tmp_cpx_i[12]) * inv_factor;
    src[10] = (tmp_cpx_r[9] - tmp_cpx_r[13]) * inv_factor;
    src[11] = (tmp_cpx_i[9] - tmp_cpx_i[13]) * inv_factor;
    src[12] = (tmp_cpx_r[10] + tmp_cpx_i[14]) * inv_factor;
    src[13] = (tmp_cpx_i[10] - tmp_cpx_r[14]) * inv_factor;
    src[14] = (tmp_cpx_r[11] + tmp_cpx_i[15]) * inv_factor;
    src[15] = (tmp_cpx_i[11] - tmp_cpx_r[15]) * inv_factor;

    return 0;
}

int32_t riscv_dsp_cifft_4pt_f16(float16_t *src, uint32_t m)
{
    // used for 3pts rfft
    (void) m;
    float16_t inv_factor = 1.0f / 4.0f;
    float16_t tmp_cal_float_r[4];
    float16_t tmp_cal_float_i[4];

    tmp_cal_float_r[0] = src[0] + src[4];
    tmp_cal_float_i[0] = src[1] + src[5];
    tmp_cal_float_r[1] = src[0] - src[4];
    tmp_cal_float_i[1] = src[1] - src[5];

    tmp_cal_float_r[2] = src[2] + src[6];
    tmp_cal_float_i[2] = src[3] + src[7];
    tmp_cal_float_r[3] = src[2] - src[6];
    tmp_cal_float_i[3] = src[3] - src[7];

    src[0] = (tmp_cal_float_r[0] + tmp_cal_float_r[2]) * inv_factor;
    src[1] = (tmp_cal_float_i[0] + tmp_cal_float_i[2]) * inv_factor;

    src[2] = (tmp_cal_float_r[1] - tmp_cal_float_i[3]) * inv_factor;
    src[3] = (tmp_cal_float_i[1] + tmp_cal_float_r[3]) * inv_factor;

    src[4] = (tmp_cal_float_r[0] - tmp_cal_float_r[2]) * inv_factor;
    src[5] = (tmp_cal_float_i[0] - tmp_cal_float_i[2]) * inv_factor;

    src[6] = (tmp_cal_float_r[1] + tmp_cal_float_i[3]) * inv_factor;
    src[7] = (tmp_cal_float_i[1] - tmp_cal_float_r[3]) * inv_factor;

    return 0;

}


