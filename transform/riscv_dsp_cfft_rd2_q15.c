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
 * @addtogroup cfft_radix2
 * @{
 */

/**
 * @brief Function to implement the q15 Radix-2 Complex FFT Functions
 * @param[in]       *src     points to the input block data src[2 * k] and
 *                           src[2 * k + 1] contain the real and imaginary
 *                           parts of the k-th data..
 * @param[in]       m        If not defined STATIC_VERSION, the data buffer
 *                           is of size 2 * n while n = 2 ** m.
 * @return valaue   0  success.
 *                  -1 invalid range.
 */

/* function description */
int32_t riscv_dsp_cfft_rd2_q15(q15_t *src, uint32_t m)
{
    uint32_t j, k, n2, ie, ia;
    q15_t c, s;
    q15_t *ptrs, *ptre;
    q31_t xt;
    q15_t xa, xb, ya, yb;
    q31_t yt;
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
    q15_t p;
    p = riscv_dsp_recip_table_q15[m - 2]; /* 2 / FFT_N */
#endif /* FFT_LOGN > RES_LOGN */

    /* for first stage, 2 * n2 == n, thus the inner for loop only
     ** execute once. */
    n2 = n;
    ptre = &src[n];
    ptrs = &src[0];

    dump_data_q15(src, n * 2, "input data");

    /* a = xa + ya * i and b = xb + yb * i */
    xa = ptrs[0] >> 1;
    ya = ptrs[1] >> 1;
    xb = ptre[0] >> 1;
    yb = ptre[1] >> 1;
    /* xa' = xa + xb */
    ptrs[0] = (xa + xb) >> 1;
    /* ya' = ya + yb */
    ptrs[1] = (ya + yb) >> 1;
    /* xb' = (xa - xb) * cos + (ya - yb) * sin */
    ptre[0] = (xa - xb) >> 1;
    /* yb' = (ya - yb) * cos + (xa - xb) * sin */
    ptre[1] = (ya - yb) >> 1;

    dump_data_q15(src, n * 2, "before 1st stage");

    ia = 0;
    ptrs += 2;
    while (ptrs != ptre)
    {
        q15_t *ptr;

        /* get the cosine and sine values. */
        ia++;
#if FFT_LOGN > RES_LOGN
        GET_COS_SIN_VALUES(ia, c, s, p, q15);
#else
        GET_COS_SIN_VALUES(ia, c, s, m, q15);
#endif /* FFT_LOGN > RES_LOGN */

        /* a = xa + ya * i and b = xb + yb * i */
        ptr = ptrs + n;

        xa = ptrs[0] >> 1;
        ya = ptrs[1] >> 1;
        xb = ptr[0] >> 1;
        yb = ptr[1] >> 1;
        xt = xa - xb;       /* xt = xa - xb */
        /* xa' = xa + xb */
        ptrs[0] = (xa + xb) >> 1;
        yt = ya - yb;       /* yt = ya - yb */
        /* ya' = ya + yb */
        ptrs[1] = (ya + yb) >> 1;
        /* xb' = (xa - xb) * cos + (ya - yb) * sin */
        ptr[0] = (c * xt + s * yt) >> 16;
        /* yb' = (ya - yb) * cos + (xa - xb) * sin */
        ptr[1] = (c * yt - s * xt) >> 16;
        ptrs += 2;
    }

    dump_data_q15(src, n * 2, "after 1st stage");

    /* for middle stages */
    ie = 2;
    k = m - 2;
    while (k)
    {
        q15_t *ptr1, *ptr2;

        n2 >>= 1;
        ptrs = &src[0];
        ptre = &src[n2];

        ptr1 = ptrs;
        j = ie;

        dump_data_q15(src, n * 2, "\tbegin 2nd stage");

        while (j)
        {
            /* a = xa + ya * i and b = xb + yb * i */
            ptr2 = ptr1 + n2;
            xa = ptr1[0];
            ya = ptr1[1];
            xb = ptr2[0];
            yb = ptr2[1];
            /* xa' = xa + xb */
            ptr1[0] = (xa + xb) >> 1;
            /* ya' = ya + yb */
            ptr1[1] = (ya + yb) >> 1;
            /* xb' = (xa - xb) * cos + (ya - yb) * sin */
            ptr2[0] = (xa - xb) >> 1;
            /* yb' = (ya - yb) * cos + (xa - xb) * sin */
            ptr2[1] = (ya - yb) >> 1;
            ptr1 = ptr2 + n2;
            j--;
        }

        dump_data_q15(src, n * 2, "\tstage 2.1");

        ia = 0;
        ptrs += 2;
        while (ptrs != ptre)
        {
            /* get the cosine and sine values. */
            ia += ie;
#if FFT_LOGN > RES_LOGN
            GET_COS_SIN_VALUES(ia, c, s, p, q15);
#else
            GET_COS_SIN_VALUES(ia, c, s, m, q15);
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
                ptr1[0] = (xa + xb) >> 1;
                yt = ya - yb;             /* yt = ya - yb */
                /* ya' = ya + yb */
                ptr1[1] = (ya + yb) >> 1;
                /* xb' = (xa - xb) * cos + (ya - yb) * sin */
                ptr2[0] = (c * xt + s * yt) >> 16;
                /* yb' = (ya - yb) * cos + (xa - xb) * sin */
                ptr2[1] = (c * yt - s * xt) >> 16;
                ptr1 = ptr2 + n2;
                j--;
            }

            //dump_data_q15(src, n * 2, "\tstage 2.1.1");

            ptrs += 2;
        }

        //dump_data_q15(src, n * 2, "\tstage 2.2");

        ie <<= 1;
        k--;
    }

    dump_data_q15(src, n * 2, "after 2nd stage");

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
        ptrs[0] = (xa + xb) >> 1;   /* xa' = xa + xb */
        ptrs[1] = (ya + yb) >> 1;   /* ya' = ya + yb */
        ptrs[2] = (xa - xb) >> 1;   /* xb' = (xa - xb) * cos + (ya - yb) * sin */
        ptrs[3] = (ya - yb) >> 1;   /* yb' = (ya - yb) * cos + (xa - xb) * sin */
        xa = ptrs[4];
        ya = ptrs[5];
        xb = ptrs[6];
        yb = ptrs[7];
        ptrs[4] = (xa + xb) >> 1;   /* xa' = xa + xb */
        ptrs[5] = (ya + yb) >> 1;   /* ya' = ya + yb */
        ptrs[6] = (xa - xb) >> 1;   /* xb' = (xa - xb) * cos + (ya - yb) * sin */
        ptrs[7] = (ya - yb) >> 1;   /* yb' = (ya - yb) * cos + (xa - xb) * sin */
        ptrs += 8;
    }

    dump_data_q15(src, n * 2, "after 3rd stage");

    /* Bit reversal permutation */
    riscv_dsp_bit_reversal_q15(src, m);

    dump_data_q15(src, n * 2, "after bit rev");
    return 0;
}


/**
 * @brief Function to implement the q15 Radix-2 Complex IFFT Functions
 * @param[in]       *src     points to the input block data src[2 * k] and
 *                           src[2 * k + 1] contain the real and imaginary
 *                           parts of the k-th data..
 * @param[in]       m        If not defined STATIC_VERSION, the data buffer
 *                           is of size 2 * n while n = 2 ** m.
 * @return valaue   0  success.
 *                  -1 invalid range.
 */

int32_t riscv_dsp_cifft_rd2_q15(q15_t *src, uint32_t m)
{
    uint32_t j, k, n2, ie, ia;
    q15_t c, s;
    q15_t *ptrs, *ptre;
    q31_t xt, yt;
    q15_t xa, ya, xb, yb;


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
    q15_t p;
    p = riscv_dsp_recip_table_q15[m - 2]; /* 2 / FFT_N */
#endif

    /* for first stage, 2 * n2 == n, thus the inner for loop only
     ** execute once. */
    n2 = n;
    ptre = &src[n];
    ptrs = &src[0];

    dump_data_q15(src, n * 2, "CIFFT input data");

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

    dump_data_q15(src, n * 2, "CIFFT before 1st stage");

    ia = 0;
    ptrs += 2;
    while (ptrs != ptre)
    {
        q15_t *ptr1;

        /* get the cosine and sine values. */
        ia++;
#if FFT_LOGN > RES_LOGN
        GET_COS_SIN_VALUES(ia, c, s, p, q15);
#else
        GET_COS_SIN_VALUES(ia, c, s, m, q15);
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
        ptr1[0] = (c * xt + s * yt) >> 15;
        /* yb' = (ya - yb) * cos + (xa - xb) * sin */
        ptr1[1] = (c * yt - s * xt) >> 15;
        ptrs += 2;
    }

    dump_data_q15(src, n * 2, "CIFFT after 1st stage");

    /* for middle stages */
    ie = 2;
    k = m - 2;
    while (k)
    {
        q15_t *ptr1, *ptr2;

        n2 >>= 1;
        ptrs = &src[0];
        ptre = &src[n2];

        ptr1 = ptrs;
        j = ie;

        //dump_data_q15(src, n * 2, "\tCIFFT begin 2nd stage");

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

        //dump_data_q15(src, n * 2, "\tCIFFT stage 2.1");

        ia = 0;
        ptrs += 2;
        while (ptrs != ptre)
        {
            /* get the cosine and sine values. */
            ia += ie;
#if FFT_LOGN > RES_LOGN
            GET_COS_SIN_VALUES(ia, c, s, p, q15);
#else
            GET_COS_SIN_VALUES(ia, c, s, m, q15);
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
                ptr2[0] = (c * xt + s * yt) >> 15;
                /* yb' = (ya - yb) * cos + (xa - xb) * sin */
                ptr2[1] = (c * yt - s * xt) >> 15;
                ptr1 = ptr2 + n2;
                j--;
            }

            //dump_data_q15(src, n * 2, "\tCIFFT stage 2.1.1");

            ptrs += 2;
        }

        //dump_data_q15(src, n * 2, "\tCIFFT stage 2.2");

        ie <<= 1;
        k--;
    }

    dump_data_q15(src, n * 2, "CIFFT after 2nd stage");

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
        q31_t tmp_s_r, tmp_s_i;
        tmp_s_r = ((q31_t) xa + (q31_t) xb) << 1 ;
        tmp_s_i = -(((q31_t) ya + (q31_t)yb) << 1);
        ptrs[0] = (q15_t) NDS_ISA_SATS(tmp_s_r, 16);
        ptrs[1] = (q15_t) NDS_ISA_SATS(tmp_s_i, 16);

        tmp_s_r = ((q31_t) xa - (q31_t) xb) << 1 ;
        tmp_s_i = ((q31_t) yb - (q31_t) ya) << 1 ;
        ptrs[2] = (q15_t) NDS_ISA_SATS(tmp_s_r, 16);
        ptrs[3] = (q15_t) NDS_ISA_SATS(tmp_s_i, 16);

        xa = ptrs[4];
        ya = ptrs[5];
        xb = ptrs[6];
        yb = ptrs[7];
        tmp_s_r = ((q31_t) xa + (q31_t) xb) << 1 ;
        tmp_s_i = -(((q31_t) ya + (q31_t)yb) << 1);
        ptrs[4] = (q15_t) NDS_ISA_SATS(tmp_s_r, 16);
        ptrs[5] = (q15_t) NDS_ISA_SATS(tmp_s_i, 16);

        tmp_s_r = ((q31_t) xa - (q31_t) xb) << 1 ;
        tmp_s_i = ((q31_t) yb - (q31_t) ya) << 1 ;
        ptrs[6] = (q15_t) NDS_ISA_SATS(tmp_s_r, 16);
        ptrs[7] = (q15_t) NDS_ISA_SATS(tmp_s_i, 16);

        
        ptrs += 8;
    }

    dump_data_q15(src, n * 2, "CIFFT after 3rd stage");

    /* Bit reversal permutation */
    riscv_dsp_bit_reversal_q15(src, m);

    dump_data_q15(src, n * 2, "CIFFT after bit rev");
    return 0;
}

/**
 * @} end of cfft_radix2
 */
