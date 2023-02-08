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
 * @addtogroup cfft_radix2
 * @{
 */

/**
 * @brief Function to implement the q31 Radix-2 Complex FFT Functions
 * @param[in]       *src     points to the input block data src[2 * k] and
 *                           src[2 * k + 1] contain the real and imaginary
 *                           parts of the k-th data..
 * @param[in]       m        If not defined STATIC_VERSION, the data buffer
 *                           is of size 2 * n while n = 2 ** m.
 * @return valaue   0  success.
 *                  -1 invalid range.
 */
#include "stdio.h"
/* function description */
int32_t riscv_dsp_cfft_rd2_q31(q31_t *src, uint32_t m)
{
    uint32_t j, k, n2, ie, ia;
    q31_t c, s;
    q31_t *ptrs, *ptre;
    q31_t xa, xb, ya, yb;
    q63_t xt, yt;
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
    q31_t p;
    p = riscv_dsp_recip_table_q31[m - 2]; /* 2 / FFT_N */
#endif /* FFT_LOGN > RES_LOGN */

    /* for first stage, 2 * n2 == n, thus the inner for loop only
     ** execute once. */
    n2 = n;
    ptre = &src[n];
    ptrs = &src[0];
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
    ia = 0;
    ptrs += 2;
    while (ptrs != ptre)
    {
        q31_t *ptr;

        /* get the cosine and sine values. */
        ia++;
#if FFT_LOGN > RES_LOGN
        GET_COS_SIN_VALUES(ia, c, s, p, q31);
#else
        GET_COS_SIN_VALUES(ia, c, s, m, q31);
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
        ptr[0] = (c * xt + s * yt) >> 32;
        /* yb' = (ya - yb) * cos + (xa - xb) * sin */
        ptr[1] = (c * yt - s * xt) >> 32;
        ptrs += 2;
    }

    /* for middle stages */
    ie = 2;
    k = m - 2;
    while (k)
    {
        q31_t *ptr1, *ptr2;

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
        ia = 0;
        ptrs += 2;
        while (ptrs != ptre)
        {
            /* get the cosine and sine values. */
            ia += ie;
#if FFT_LOGN > RES_LOGN
            GET_COS_SIN_VALUES(ia, c, s, p, q31);
#else
            GET_COS_SIN_VALUES(ia, c, s, m, q31);
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
                ptr2[0] = (c * xt + s * yt) >> 32;
                /* yb' = (ya - yb) * cos + (xa - xb) * sin */
                ptr2[1] = (c * yt - s * xt) >> 32;
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

    /* Bit reversal permutation */
    return riscv_dsp_bit_reversal_q31 (src, m);
}


/**
 * @brief Function to implement the q31 Radix-2 Complex IFFT Functions
 * @param[in]       *src     points to the input block data src[2 * k] and
 *                           src[2 * k + 1] contain the real and imaginary
 *                           parts of the k-th data..
 * @param[in]       m        If not defined STATIC_VERSION, the data buffer
 *                           is of size 2 * n while n = 2 ** m.
 * @return valaue   0  success.
 *                  -1 invalid range.
 */

int32_t riscv_dsp_cifft_rd2_q31(q31_t *src, uint32_t m)
{
    uint32_t j, k, n2, ie, ia;
    q31_t c, s, xa, ya, xb, yb;
    q31_t *ptrs, *ptre;
    q63_t xt, yt;
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
    q31_t p;
    p = riscv_dsp_recip_table_q31[m - 2]; /* 2 / FFT_N */
#endif

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
        q31_t *ptr1;

        /* get the cosine and sine values. */
        ia++;
#if FFT_LOGN > RES_LOGN
        GET_COS_SIN_VALUES(ia, c, s, p, q31);
#else
        GET_COS_SIN_VALUES(ia, c, s, m, q31);
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
        ptr1[0] = (c * xt + s * yt) >> 31;
        /* yb' = (ya - yb) * cos + (xa - xb) * sin */
        ptr1[1] = (c * yt - s * xt) >> 31;
        ptrs += 2;
    }

    /* for middle stages */
    ie = 2;
    k = m - 2;
    while (k)
    {
        q31_t *ptr1, *ptr2;

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
            GET_COS_SIN_VALUES(ia, c, s, p, q31);
#else
            GET_COS_SIN_VALUES(ia, c, s, m, q31);
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
                ptr2[0] = (c * xt + s * yt) >> 31;
                /* yb' = (ya - yb) * cos + (xa - xb) * sin */
                ptr2[1] = (c * yt - s * xt) >> 31;
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
        ptrs[0] = (xa + xb) << 1;   /* xa' = xa + xb */
        ptrs[1] = (-yb - ya) << 1;  /* ya' = ya + yb */
        ptrs[2] = (xa - xb) << 1;   /* xb' = (xa - xb) * cos + (ya - yb) * sin */
        ptrs[3] = (yb - ya) << 1;   /* yb' = (ya - yb) * cos + (xa - xb) * sin */
        xa = ptrs[4];
        ya = ptrs[5];
        xb = ptrs[6];
        yb = ptrs[7];
        ptrs[4] = (xa + xb) << 1;   /* xa' = xa + xb */
        ptrs[5] = (-yb - ya) << 1;  /* ya' = ya + yb */
        ptrs[6] = (xa - xb) << 1;   /* xb' = (xa - xb) * cos + (ya - yb) * sin */
        ptrs[7] = (yb - ya) << 1;   /* yb' = (ya - yb) * cos + (xa - xb) * sin */
        ptrs += 8;
    }

    /* Bit reversal permutation */
    return riscv_dsp_bit_reversal_q31 (src, m);
}

/**
 * @} end of cfft_radix2
 */
