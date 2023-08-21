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
#include <stdio.h>
#include "internal_transform_math.h"
/**
 * @ingroup transform
 */

/**
 * @addtogroup cfft_radix4
 * @{
 */

/**
 * @brief Function to implement the q15 Radix-2 Complex FFT
 *        Functions
 * @param[in]       *src     points to the input block data, src[2 * k] and
 *                           src[2 * k + 1] contain the real and imaginary
 *                           parts of the k-th data.
 * @param[in]       m        If not defined STATIC_VERSION, the data buffer
 *                           is of size 2 * n while n = 2 ** m.
 * @return valaue   0  success.
 *                  -1 invalid range.
 */

/* function description */
int32_t riscv_dsp_cfft_rd4_q15(q15_t *src, uint32_t m)
{
    register unsigned int j, k, n2, ie, ia1, ia2, ia3;
    q15_t c1, c2, c3, s1, s2, s3;
    q15_t *ptrs, *ptre, *ptr1, *ptr2, *ptr3;
    q31_t xt1, xt2, yt1, yt2;
    q15_t tmp, xa, ya, xb, yb;
    q15_t xc, yc, xd, yd;
    unsigned int n;

#ifdef CHECK_RANGE
    /* Quick return for m < 4, m > 14, and m is odd */
    if (m < 4 || m > 14 || ((m & 0x1) != 0))
    {
        /* only support n = 16, 64, 256, 1024, 4096, and 16384 that is,
         ** m = 4, 6, 8, 10, 12, and 14. */
        return -1;
    }
#endif /* CHECK_RANGE */

    n = 1 << m;

    /* Main loop */
#if FFT_LOGN > RES_LOGN
    q15_t p;
    p = riscv_dsp_recip_table_q15[m - 2]; /* 2 / FFT_N */
#endif

    /* for first stage, n1 == n, thus the inner for loop only execute
     ** once. */
    n2 = n >> 1;
    ptre = &src[n2];
    ptrs = &src[0];

    dump_data_q15(src, n * 2, "input data");

    /* a = xa + ya * i, b = xb + yb * i, c = xc + yc * i,
     ** and d = xd + yd * i */
    ptr1 = ptre;
    ptr2 = ptr1 + n2;
    ptr3 = ptr2 + n2;
    tmp = ptrs[0] >> 1;
    xc = ptr2[0] >> 1;
    xa = (tmp + xc) >> 1;   /* xa + xc */
    xc = (tmp - xc) >> 1;   /* xa - xc */
    tmp = ptr1[0] >> 1;
    xd = ptr3[0] >> 1;
    xb = (tmp + xd) >> 1;   /* xb + xd */
    xd = (tmp - xd) >> 1;   /* xb - xd */
    /* xa' = xa + xb + xc + xd */
    ptrs[0] = (xa + xb) >> 1;
    tmp = ptrs[1] >> 1;
    yc = ptr2[1] >> 1;
    ya = (tmp + yc) >> 1;   /* ya + yc */
    yc = (tmp - yc) >> 1;   /* ya - yc */
    tmp = ptr1[1] >> 1;
    yd = ptr3[1] >> 1;
    yb = (tmp + yd) >> 1;   /* yb + yd */
    yd = (tmp - yd) >> 1;   /* yb - yd */
    /* ya' = ya + yb + yc + yd */
    ptrs[1] = (ya + yb) >> 1;
    /* xb' = (xa - xb + xc - cd) * cos2 + (ya - yb + yc - yd) * sin2 */
    ptr1[0] = (xa - xb) >> 1;
    /* yb' = (ya - yb + yc - yd) * cos2 - (xa - xb + xc - xd) * sin2 */
    ptr1[1] = (ya - yb) >> 1;
    /* xc' = (xa + yb - xc - yd) * cos1 + (ya - xb - yc + xd) * sin1 */
    ptr2[0] = (xc + yd) >> 1;
    /* yc' = (ya - xb - yc + xd) * cos1 + (xa + yb - xc - yd) * sin1 */
    ptr2[1] = (yc - xd) >> 1;
    /* xd' = (xa - yb - xc + yd) * cos3 + (ya + xb - yc - xd) * sin3 */
    ptr3[0] = (xc - yd) >> 1;
    /* yd' = (ya + xb - yc - xd) * cos3 + (xa - yb - xc + yd) * sin3 */
    ptr3[1] = (yc + xd) >> 1;

    dump_data_q15(src, n * 2, "before 1st stage");

    ia1 = 0;
    ptrs += 2;
    while (ptrs != ptre)
    {
        /* a = xa + ya * i, b = xb + yb * i, c = xc + yc * i,
         ** and d = xd + yd * i */
        ptr1 = ptrs + n2;
        ptr2 = ptr1 + n2;
        ptr3 = ptr2 + n2;
        tmp = ptrs[0] >> 1;
        xc = ptr2[0] >> 1;
        xa = (tmp + xc) >> 1;   /* xa + xc */
        xc = (tmp - xc) >> 1;   /* xa - xc */
        tmp = ptr1[0] >> 1;
        xd = ptr3[0] >> 1;
        xb = (tmp + xd) >> 1;   /* xb + xd */
        xd = (tmp - xd) >> 1;   /* xb - xd */
        ptrs[0] = (xa + xb) >> 1;
        xt1 = xa - xb;      /* xt1 = xa - xb + xc - xd */
        tmp = ptrs[1] >> 1;
        yc = ptr2[1] >> 1;
        ya = (tmp + yc) >> 1;   /* ya + yc */
        yc = (tmp - yc) >> 1;   /* ya - yc */
        tmp = ptr1[1] >> 1;
        yd = ptr3[1] >> 1;
        yb = (tmp + yd) >> 1;   /* yb + yd */
        yd = (tmp - yd) >> 1;   /* yb - yd */
        ptrs[1] = (ya + yb) >> 1;
        yt1 = ya - yb;      /* yt1 = ya - yb + yc - yd */

        /* get cosine and sine values */
        ia1++;
        ia2 = ia1 + ia1;
#if FFT_LOGN > RES_LOGN
        GET_COS_SIN_VALUES(ia2, c2, s2, p, q15);
#else
        GET_COS_SIN_VALUES(ia2, c2, s2, m, q15);
#endif /* FFT_LOGN > RES_LOGN */

        ptr1[0] = (c2 * xt1 + s2 * yt1) >> 16;
        ptr1[1] = (c2 * yt1 - s2 * xt1) >> 16;
        xt1 = xc + yd;      /* xt1 = xa + yb - xc - yd */
        xt2 = xc - yd;      /* xt2 = xa - yb - xc + yd */
        yt1 = yc - xd;      /* yt1 = ya - xb - yc + xd */
        yt2 = yc + xd;      /* yt2 = ya + xb - yc - yd */

        /* get cosine and sine values */
#if FFT_LOGN > RES_LOGN
        GET_COS_SIN_VALUES(ia1, c1, s1, p, q15);
#else
        GET_COS_SIN_VALUES(ia1, c1, s1, m, q15);
#endif /* FFT_LOGN > RES_LOGN */

        ptr2[0] = (c1 * xt1 + s1 * yt1) >> 16;
        ptr2[1] = (c1 * yt1 - s1 * xt1) >> 16;

        /* get cosine and sine values */
        ia3 = ia2 + ia1;
#if FFT_LOGN > RES_LOGN
        GET_COS_SIN_VALUES(ia3, c3, s3, p, q15);
#else
        GET_COS_SIN_VALUES(ia3, c3, s3, m, q15);
#endif /* FFT_LOGN > RES_LOGN */

        ptr3[0] = (c3 * xt2 + s3 * yt2) >> 16;
        ptr3[1] = (c3 * yt2 - s3 * xt2) >> 16;
        ptrs += 2;
    }

    dump_data_q15(src, n * 2, "after 1st stage");

    /* for middle stages:                           */
    /* there is no middle stage for N = 16 (m = 4). */
    ie = 4;
    for (k = (m >> 1) - 2; 0 != k; k--)
    {
        q15_t *ptr0;

        n2 >>= 2;
        ptre = &src[n2];
        ptrs = &src[0];

        ptr0 = ptrs;
        j = ie;

        dump_data_q15(src, n * 2, "\tbegin 2nd stage");

        while (j)
        {
            /* a = xa + ya * i, b = xb + yb * i, c = xc + yc * i,
             ** and d = xd + yd * i */
            ptr1 = ptr0 + n2;
            ptr2 = ptr1 + n2;
            ptr3 = ptr2 + n2;
            xa = (ptr0[0] + ptr2[0]) >> 1;  /* xa + xc */
            xc = (ptr0[0] - ptr2[0]) >> 1;  /* xa - xc */
            xb = (ptr1[0] + ptr3[0]) >> 1;  /* xb + xd */
            xd = (ptr1[0] - ptr3[0]) >> 1;  /* xb - xd */
            /* xa' = xa + xb + xc + xd */
            ptr0[0] = (xa + xb) >> 1;
            ya = (ptr0[1] + ptr2[1]) >> 1;  /* ya + yc */
            yc = (ptr0[1] - ptr2[1]) >> 1;  /* ya - yc */
            yb = (ptr1[1] + ptr3[1]) >> 1;  /* yb + yd */
            yd = (ptr1[1] - ptr3[1]) >> 1;  /* yb - yd */
            /* ya' = ya + yb + yc + yd */
            ptr0[1] = (ya + yb) >> 1;
            /* xb' = (xa - xb + xc - cd) * cos2 + (ya - yb
             ** + yc - yd) * sin2 */
            ptr1[0] = (xa - xb) >> 1;
            /* yb' = (ya - yb + yc - yd) * cos2 - (xa - xb
             ** + xc - xd) * sin2 */
            ptr1[1] = (ya - yb) >> 1;
            /* xc' = (xa + yb - xc - yd) * cos1 + (ya - xb
             ** - yc + xd) * sin1 */
            ptr2[0] = (xc + yd) >> 1;
            /* yc' = (ya - xb - yc + xd) * cos1 + (xa + yb
             ** - xc - yd) * sin1 */
            ptr2[1] = (yc - xd) >> 1;
            /* xd' = (xa - yb - xc + yd) * cos3 + (ya + xb
             ** - yc - xd) * sin3 */
            ptr3[0] = (xc - yd) >> 1;
            /* yd' = (ya + xb - yc - xd) * cos3 + (xa - yb
             ** - xc + yd) * sin3 */
            ptr3[1] = (yc + xd) >> 1;
            ptr0 = ptr3 + n2;
            j--;
        }

        dump_data_q15(src, n * 2, "\tstage 2.1");

        ia1 = 0;
        ptrs += 2;
        while (ptrs != ptre)
        {
            /* get cosine and sine values */
            ia1 += ie;
            ia2 = ia1 + ia1;
            ia3 = ia2 + ia1;
#if FFT_LOGN > RES_LOGN
            GET_COS_SIN_VALUES(ia1, c1, s1, p, q15);
            GET_COS_SIN_VALUES(ia2, c2, s2, p, q15);
            GET_COS_SIN_VALUES(ia3, c3, s3, p, q15);
#else
            GET_COS_SIN_VALUES(ia1, c1, s1, m, q15);
            GET_COS_SIN_VALUES(ia2, c2, s2, m, q15);
            GET_COS_SIN_VALUES(ia3, c3, s3, m, q15);
#endif /* FFT_LOGN > RES_LOGN */

            ptr0 = ptrs;
            j = ie;
            while (j)
            {
                /* a = xa + ya * i, b = xb + yb * i, c = xc + yc * i,
                 ** and d = xd + yd * i */
                ptr1 = ptr0 + n2;
                ptr2 = ptr1 + n2;
                ptr3 = ptr2 + n2;
                xa = (ptr0[0] + ptr2[0]) >> 1;  /* xa + xc */
                xc = (ptr0[0] - ptr2[0]) >> 1;  /* xa - xc */
                xb = (ptr1[0] + ptr3[0]) >> 1;  /* xb + xd */
                xd = (ptr1[0] - ptr3[0]) >> 1;  /* xb - xd */

                ptr0[0] = (xa + xb) >> 1;

                ya = (ptr0[1] + ptr2[1]) >> 1;  /* ya + yc */
                yc = (ptr0[1] - ptr2[1]) >> 1;  /* ya - yc */
                yb = (ptr1[1] + ptr3[1]) >> 1;  /* yb + yd */
                yd = (ptr1[1] - ptr3[1]) >> 1;  /* yb - yd */

                ptr0[1] = (ya + yb) >> 1;

                xt1 = xa - xb;      /* xt1 = xa - xb + xc - xd */
                yt1 = ya - yb;      /* yt1 = ya - yb + yc - yd */

                ptr1[0] = (c2 * xt1 + s2 * yt1) >> 16;
                ptr1[1] = (c2 * yt1 - s2 * xt1) >> 16;

                //printf("ptr1[0] %04X\n", (unsigned int)ptr1[0]);
                //printf("ptr1[1] %04X\n", (unsigned int)ptr1[1]);

                xt1 = xc + yd;      /* xt1 = xa + yb - xc - yd */
                xt2 = xc - yd;      /* xt2 = xa - yb - xc + yd */
                yt1 = yc - xd;      /* yt1 = ya - xb  - yc + xd */
                yt2 = yc + xd;      /* yt2 = ya + xb - yc - yd */

                ptr2[0] = (c1 * xt1 + s1 * yt1) >> 16;
                ptr2[1] = (c1 * yt1 - s1 * xt1) >> 16;
                ptr3[0] = (c3 * xt2 + s3 * yt2) >> 16;
                ptr3[1] = (c3 * yt2 - s3 * xt2) >> 16;
                //printf("ptr2[0] %08X\n", (unsigned int)ptr2[0]);
                //printf("ptr2[1] %08X\n", (unsigned int)ptr2[1]);
                //printf("ptr3[0] %08X\n", (unsigned int)ptr3[0]);
                //printf("ptr3[1] %08X\n", (unsigned int)ptr3[1]);

                DBGMSG("ptr2[0] %08X\n", (unsigned int)ptr2[0]);
                DBGMSG("ptr2[1] %08X\n", (unsigned int)ptr2[1]);
                DBGMSG("ptr3[0] %08X\n", (unsigned int)ptr3[0]);
                DBGMSG("ptr3[1] %08X\n", (unsigned int)ptr3[1]);

                ptr0 = ptr3 + n2;
                j--;
            }
            ptrs += 2;
        }
        ie <<= 2;
    }

    dump_data_q15(src, n * 2, "after 2nd stage");

    /* for last stage, n2 == 1; thus the outer for loop only
     ** execute once. also, cos(0) == 1 and sin(0) == 0. */
    ptrs = &src[0];
    ptre = &src[n << 1];
    while (ptrs != ptre)
    {
        /* a = xa + ya * i, b = xb + yb * i, c = xc + yc * i,
         ** and d = xd + yd * i */
        xa = (ptrs[0] + ptrs[4]) >> 1;  /* xa + xc */
        xc = (ptrs[0] - ptrs[4]) >> 1;  /* xa - xc */
        xb = (ptrs[2] + ptrs[6]) >> 1;  /* xb + xd */
        xd = (ptrs[2] - ptrs[6]) >> 1;  /* xb - xd */
        /* xa' = xa + xb + xc + xd */
        ptrs[0] = (xa + xb) >> 1;
        ya = (ptrs[1] + ptrs[5]) >> 1;  /* ya + yc */
        yc = (ptrs[1] - ptrs[5]) >> 1;   /* ya - yc */
        yb = (ptrs[3] + ptrs[7]) >> 1;  /* yb + yd */
        yd = (ptrs[3] - ptrs[7]) >> 1;  /* yb - yd */
        /* ya' = ya + yb + yc + yd */
        ptrs[1] = (ya + yb) >> 1;
        /* xb' = (xa - xb + xc - cd) * cos2 + (ya - yb + yc - yd) * sin2 */
        ptrs[2] = (xa - xb) >> 1;
        /* yb' = (ya - yb + yc - yd) * cos2 - (xa - xb + xc - xd) * sin2 */
        ptrs[3] = (ya - yb) >> 1;
        /* xc' = (xa + yb - xc - yd) * cos1 + (ya - xb - yc + xd) * sin1 */
        ptrs[4] = (xc + yd) >> 1;
        /* yc' = (ya - xb - yc + xd) * cos1 + (xa + yb - xc - yd) * sin1 */
        ptrs[5] = (yc - xd) >> 1;
        /* xd' = (xa - yb - xc + yd) * cos3 + (ya + xb - yc - xd) * sin3 */
        ptrs[6] = (xc - yd) >> 1;
        /* yd' = (ya + xb - yc - xd) * cos3 + (xa - yb - xc + yd) * sin3 */
        ptrs[7] = (yc + xd) >> 1;
        ptrs += 8;
    }

    dump_data_q15(src, n * 2, "after 3rd stage");

    /* Bit reversal permutation */
    riscv_dsp_bit_reversal_q15(src, m);

    dump_data_q15(src, n * 2, "after bit rev");
    return 0;
}

/**
 * @brief Function to implement the q15 Radix-2 Complex IFFT
 *        Functions
 * @param[in]       *src     points to the input block data, src[2 * k] and
 *                           src[2 * k + 1] contain the real and imaginary
 *                           parts of the k-th data.
 * @param[in]       m        If not defined STATIC_VERSION, the data buffer
 *                           is of size 2 * n while n = 2 ** m.
 * @return valaue   0  success.
 *                  -1 invalid range.
 */

int32_t riscv_dsp_cifft_rd4_q15(q15_t *src, uint32_t m)
{
    register unsigned int j, k, n2, ie, ia1, ia2, ia3;
    q15_t c1, c2, c3, s1, s2, s3;
    q15_t *ptrs, *ptre, *ptr1, *ptr2, *ptr3;
    unsigned int n;
    q31_t xt1, xt2, yt1, yt2;
    q15_t xc, yc, xd, yd;
    q15_t xa, ya, xb, yb;


#ifdef CHECK_RANGE
    /* Quick return for m < 4, m > 14, and m is odd */
    if (m < 4 || m > 14 || ((m & 0x1) != 0))
    {
        /* only support n = 16, 64, 256, 1024, 4096, and 16384 that is,
         ** m = 4, 6, 8, 10, 12, and 14. */
        return -1;
    }
#endif /* CHECK_RANGE */

    n = 1 << m;

    /* backward transform */
    /* Main loop */
#if FFT_LOGN > RES_LOGN
    q15_t p;
    p = riscv_dsp_recip_table_q15[m - 2]; /* 2 / FFT_N */
#endif

    /* for first stage, n1 == n, thus the inner for loop only execute
     ** once. */
    n2 = (n >> 1);
    ptre = &src[n2];
    ptrs = &src[0];
    /* a = xa + ya * i, b = xb + yb * i, c = xc + yc * i,
     ** and d = xd + yd * i */
    ptr1 = ptre;
    ptr2 = ptr1 + n2;
    ptr3 = ptr2 + n2;
    xa = ptrs[0] + ptr2[0];     /* xa + xc */
    xc = ptrs[0] - ptr2[0];     /* xa - xc */
    xb = ptr1[0] + ptr3[0];     /* xb + xd */
    xd = ptr1[0] - ptr3[0];     /* xb - xd */
    /* xa' = xa + xb + xc + xd */
    ptrs[0] = xa + xb;
    ya = -ptr2[1] - ptrs[1];    /* ya + yc */
    yc = ptr2[1] - ptrs[1];     /* ya - yc */
    yb = -ptr3[1] - ptr1[1];    /* yb + yd */
    yd = ptr3[1] - ptr1[1];     /* yb - yd */
    /* ya' = ya + yb + yc + yd */
    ptrs[1] = ya + yb;
    /* xb' = (xa - xb + xc - cd) * cos2 + (ya - yb + yc - yd) * sin2 */
    ptr1[0] = xa - xb;
    /* yb' = (ya - yb + yc - yd) * cos2 - (xa - xb + xc - xd) * sin2 */
    ptr1[1] = ya - yb;
    /* xc' = (xa + yb - xc - yd) * cos1 + (ya - xb - yc + xd) * sin1 */
    ptr2[0] = xc + yd;
    /* yc' = (ya - xb - yc + xd) * cos1 + (xa + yb - xc - yd) * sin1 */
    ptr2[1] = yc - xd;
    /* xd' = (xa - yb - xc + yd) * cos3 + (ya + xb - yc - xd) * sin3 */
    ptr3[0] = xc - yd;
    /* yd' = (ya + xb - yc - xd) * cos3 + (xa - yb - xc + yd) * sin3 */
    ptr3[1] = yc + xd;

    ia1 = 0;
    ptrs += 2;
    while (ptrs != ptre)
    {
        /* a = xa + ya * i, b = xb + yb * i, c = xc + yc * i,
         ** and d = xd + yd * i */
        ptr1 = ptrs + n2;
        ptr2 = ptr1 + n2;
        ptr3 = ptr2 + n2;
        xa = ptrs[0] + ptr2[0];     /* xa + xc */
        xc = ptrs[0] - ptr2[0];     /* xa - xc */
        xb = ptr1[0] + ptr3[0];     /* xb + xd */
        xd = ptr1[0] - ptr3[0];     /* xb - xd */
        ptrs[0] = xa + xb;
        xt1 = xa - xb;      /* xt1 = xa - xb + xc - xd */
        ya = -ptr2[1] - ptrs[1];    /* ya + yc */
        yc = ptr2[1] - ptrs[1];     /* ya - yc */
        yb = -ptr3[1] - ptr1[1];    /* yb + yd */
        yd = ptr3[1] - ptr1[1];     /* yb - yd */
        ptrs[1] = ya + yb;
        yt1 = ya - yb;      /* yt1 = ya - yb + yc - yd */
        /* get cosine and sine values */
        ia1++;
        ia2 = ia1 + ia1;
#if FFT_LOGN > RES_LOGN
        GET_COS_SIN_VALUES(ia2, c2, s2, p, q15);
#else
        GET_COS_SIN_VALUES(ia2, c2, s2, m, q15);
#endif /* FFT_LOGN > RES_LOGN */

        ptr1[0] = (c2 * xt1 + s2 * yt1) >> 15;
        ptr1[1] = (c2 * yt1 - s2 * xt1) >> 15;
        xt1 = xc + yd;      /* xt1 = xa + yb - xc - yd */
        xt2 = xc - yd;      /* xt2 = xa - yb - xc + yd */
        yt1 = yc - xd;      /* yt1 = ya - xb - yc + xd */
        yt2 = yc + xd;      /* yt2 = ya + xb - yc - yd */
        /* get cosine and sine values */
#if FFT_LOGN > RES_LOGN
        GET_COS_SIN_VALUES(ia1, c1, s1, p, q15);
#else
        GET_COS_SIN_VALUES(ia1, c1, s1, m, q15);
#endif /* FFT_LOGN > RES_LOGN */

        ptr2[0] = (c1 * xt1 + s1 * yt1) >> 15;
        ptr2[1] = (c1 * yt1 - s1 * xt1) >> 15;

        /* get cosine and sine values */
        ia3 = ia2 + ia1;
#if FFT_LOGN > RES_LOGN
        GET_COS_SIN_VALUES(ia3, c3, s3, p, q15);
#else
        GET_COS_SIN_VALUES(ia3, c3, s3, m, q15);
#endif /* FFT_LOGN > RES_LOGN */

        ptr3[0] = (c3 * xt2 + s3 * yt2) >> 15;
        ptr3[1] = (c3 * yt2 - s3 * xt2) >> 15;

        ptrs += 2;
    }

    /* for middle stages:                           */
    /* there is no middle stage for N = 16 (m = 4). */
    ie = 4;
    for (k = (m >> 1) - 2; k != 0; k--)
    {
        q15_t *ptr0;

        n2 >>= 2;
        ptre = &src[n2];
        ptrs = &src[0];

        ptr0 = ptrs;
        j = ie;
        while (j)
        {
            /* a = xa + ya * i, b = xb + yb * i, c = xc + yc * i,
             ** and d = xd + yd * i */
            ptr1 = ptr0 + n2;
            ptr2 = ptr1 + n2;
            ptr3 = ptr2 + n2;
            xa = ptr0[0] + ptr2[0]; /* xa + xc */
            xc = ptr0[0] - ptr2[0]; /* xa - xc */
            xb = ptr1[0] + ptr3[0]; /* xb + xd */
            xd = ptr1[0] - ptr3[0]; /* xb - xd */
            /* xa' = xa + xb + xc + xd */
            ptr0[0] = xa + xb;
            ya = ptr0[1] + ptr2[1]; /* ya + yc */
            yc = ptr0[1] - ptr2[1]; /* ya - yc */
            yb = ptr1[1] + ptr3[1]; /* yb + yd */
            yd = ptr1[1] - ptr3[1]; /* yb - yd */
            /* ya' = ya + yb + yc + yd */
            ptr0[1] = ya + yb;
            /* xb' = (xa - xb + xc - cd) * cos2 + (ya - yb
             ** + yc - yd) * sin2 */
            ptr1[0] = xa - xb;
            /* yb' = (ya - yb + yc - yd) * cos2 - (xa - xb
             ** + xc - xd) * sin2 */
            ptr1[1] = ya - yb;
            /* xc' = (xa + yb - xc - yd) * cos1 + (ya - xb
             ** - yc + xd) * sin1 */
            ptr2[0] = xc + yd;
            /* yc' = (ya - xb - yc + xd) * cos1 + (xa + yb
             ** - xc - yd) * sin1 */
            ptr2[1] = yc - xd;
            /* xd' = (xa - yb - xc + yd) * cos3 + (ya + xb
             ** - yc - xd) * sin3 */
            ptr3[0] = xc - yd;
            /* yd' = (ya + xb - yc - xd) * cos3 + (xa - yb
             ** - xc + yd) * sin3 */
            ptr3[1] = yc + xd;
            ptr0 = ptr3 + n2;
            j--;
        }

        ia1 = 0;
        ptrs += 2;
        while (ptrs != ptre)
        {
            /* get cosine and sine values */
            ia1 += ie;
            ia2 = ia1 + ia1;
            ia3 = ia2 + ia1;
#if FFT_LOGN > RES_LOGN
            GET_COS_SIN_VALUES(ia1, c1, s1, p, q15);
            GET_COS_SIN_VALUES(ia2, c2, s2, p, q15);
            GET_COS_SIN_VALUES(ia3, c3, s3, p, q15);
#else
            GET_COS_SIN_VALUES(ia1, c1, s1, m, q15);
            GET_COS_SIN_VALUES(ia2, c2, s2, m, q15);
            GET_COS_SIN_VALUES(ia3, c3, s3, m, q15);
#endif /* FFT_LOGN > RES_LOGN */

            ptr0 = ptrs;
            j = ie;
            while (j)
            {
                /* a = xa + ya * i, b = xb + yb * i, c = xc + yc * i,
                 ** and d = xd + yd * i */
                ptr1 = ptr0 + n2;
                ptr2 = ptr1 + n2;
                ptr3 = ptr2 + n2;
                xa = ptr0[0] + ptr2[0]; /* xa + xc */
                xc = ptr0[0] - ptr2[0]; /* xa - xc */
                xb = ptr1[0] + ptr3[0]; /* xb + xd */
                xd = ptr1[0] - ptr3[0]; /* xb - xd */

                ptr0[0] = xa + xb;

                ya = ptr0[1] + ptr2[1]; /* ya + yc */
                yc = ptr0[1] - ptr2[1]; /* ya - yc */
                yb = ptr1[1] + ptr3[1]; /* yb + yd */
                yd = ptr1[1] - ptr3[1]; /* yb - yd */

                ptr0[1] = ya + yb;

                xt1 = xa - xb;  /* xt1 = xa - xb + xc - xd */
                yt1 = ya - yb;  /* yt1 = ya - yb + yc - yd */

                ptr1[0] = (c2 * xt1 + s2 * yt1) >> 15;
                ptr1[1] = (c2 * yt1 - s2 * xt1) >> 15;
                xt1 = (xc + yd);
                xt2 = (xc - yd);
                yt1 = (yc - xd);
                yt2 = (yc + xd);
                ptr2[0] = (c1 * xt1 + s1 * yt1) >> 15;
                ptr2[1] = (c1 * yt1 - s1 * xt1) >> 15;
                ptr3[0] = (c3 * xt2 + s3 * yt2) >> 15;
                ptr3[1] = (c3 * yt2 - s3 * xt2) >> 15;
                ptr0 = ptr3 + n2;
                j--;
            }

            ptrs += 2;
        }
        ie <<= 2;
    }

    /* for last stage, (n1 == 4) and n2 == 1; thus the outer for loop only
     ** execute once. also, cos(0) == 1 and sin(0) == 0. */
    ptrs = &src[0];
    ptre = &src[n << 1];
    while (ptrs != ptre)
    {
        /* a = xa + ya * i, b = xb + yb * i, c = xc + yc * i,
         ** and d = xd + yd * i */
        xa = ptrs[0] + ptrs[4]; /* xa + xc */
        xc = ptrs[0] - ptrs[4]; /* xa - xc */
        xb = ptrs[2] + ptrs[6]; /* xb + xd */
        xd = ptrs[2] - ptrs[6]; /* xb - xd */
        /* xa' = xa + xb + xc + xd */
        ptrs[0] = (xa + xb) << 1;
        ya = ptrs[1] + ptrs[5]; /* ya + yc */
        yc = ptrs[1] - ptrs[5]; /* ya - yc */
        yb = ptrs[3] + ptrs[7]; /* yb + yd */
        yd = ptrs[3] - ptrs[7]; /* yb - yd */
        /* ya' = ya + yb + yc + yd */
        ptrs[1] = -((ya + yb) << 1);
        /* xb' = (xa - xb + xc - cd) * cos2 + (ya - yb + yc - yd) * sin2 */
        ptrs[2] = (xa - xb) << 1;
        /* yb' = (ya - yb + yc - yd) * cos2 - (xa - xb + xc - xd) * sin2 */
        ptrs[3] = -((ya - yb) << 1);
        /* xc' = (xa + yb - xc - yd) * cos1 + (ya - xb - yc + xd) * sin1 */
        ptrs[4] = (xc + yd) << 1;
        /* yc' = (ya - xb - yc + xd) * cos1 + (xa + yb - xc - yd) * sin1 */
        ptrs[5] = -((yc - xd) << 1);
        /* xd' = (xa - yb - xc + yd) * cos3 + (ya + xb - yc - xd) * sin3 */
        ptrs[6] = (xc - yd) << 1;
        /* yd' = (ya + xb - yc - xd) * cos3 + (xa - yb - xc + yd) * sin3 */
        ptrs[7] = -((yc + xd) << 1);
        ptrs += 8;
    }

    /* Bit reversal permutation */
    return riscv_dsp_bit_reversal_q15 (src, m);
}

/**
 * @} end of cfft_radix4
 */
