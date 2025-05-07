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

//Declare the external functions
extern int32_t riscv_dsp_cfft_q31(q31_t *src, uint32_t m);
extern int32_t riscv_dsp_cifft_q31(q31_t *src, uint32_t m);

/**
 * @ingroup transform
 */

/**
 * @addtogroup rfft
 * @{
 */

/**
 * @brief Function to implement the q31 Real FFT Functions
 * @param[in]       *src     points to the input block data, src[2 * k] and
 *                           src[2 * k + 1] contain the real and imaginary
 *                           parts of the k-th data.
 * @param[in]       m        If not defined STATIC_VERSION, the data buffer
 *                           is of size 2 * n while n = 2 ** m.
 * @param[in]       inv      0 for forward transform, otherwise for
 *                           backward transform
 * @return valaue   0  success.
 *                  -1 invalid range.
 *
 * Function notes:
 * There is no saturation to avoid overflow. The input is in the q31
 * formats. For the forward transform(RFFT), the output will be scaled as
 * Q(FFT_LOGN).31-(FFT_LOGN). In the backward transform(RIFFT), the output
 * is still the q31 formats.
 */

/* function description */
int32_t riscv_dsp_rfft_q31(q31_t *src, uint32_t m)
{
    register unsigned int i, j;
    q31_t c, s;
    q63_t xt1, yt1;
    q63_t xt2, xt3, yt2, yt3;
    unsigned int n;
    n = 1 << m;

    /* Main loop */
#if FFT_LOGN > RES_LOGN
    q31_t p;
    p = riscv_dsp_recip_table_q31[m - 2]; /* 2 / FFT_N */
#endif

    riscv_dsp_cfft_q31(src, m - 1);

    /* post-processing */
    q31_t *ptr = src;

    /* for rFFT */
    i = j = n >> 2;
    while (i != 0)
    {
#if FFT_LOGN > RES_LOGN
        GET_COS_SIN_VALUES(i, c, s, p, q31);
#else
        GET_COS_SIN_VALUES(i, c, s, m, q31);
#endif /* FFT_LOGN > RES_LOGN */
        xt1 = (ptr[2 * i] + ptr[2 * j]) >> 2;
        xt2 = (ptr[2 * i] - ptr[2 * j]);
        yt1 = (ptr[2 * i + 1] + ptr[2 * j + 1]);
        yt2 = (ptr[2 * i + 1] - ptr[2 * j + 1]) >> 2;

        /* twiddle and butterfly */

#ifdef ENA_HIGHER_PERFORMANCE
        xt3 = ((q31_t)(((q63_t) yt1 * c) >> 32) - (q31_t)(((q63_t) xt2 * s) >> 32)) >> 1;
        yt3 = -(((q31_t)(((q63_t) xt2 * c) >> 32) + (q31_t)(((q63_t) yt1 * s) >> 32)) >> 1);
#else
        xt3 = ((yt1 * c - xt2 * s)) >> 33;
        yt3 = -((xt2 * c + yt1 * s) >> 33);
#endif
        ptr[2 * i] = (xt1 + xt3);
        ptr[2 * i + 1] = (yt3 + yt2);
        ptr[2 * j] = (xt1 - xt3);
        ptr[2 * j + 1] = (yt3 - yt2);
        j++;
        i--;
    }

    xt1 = ptr[0];
    ptr[0] = (xt1 + ptr[1]) >> 1;
    ptr[1] = (xt1 - ptr[1]) >> 1;

    return 0;
}

int32_t riscv_dsp_rifft_q31(q31_t *src, uint32_t m)
{
    register unsigned int i, j;
    q31_t c, s;
    q63_t xt1, yt1;
    q63_t xt2, xt3, yt2, yt3;
    unsigned int n;
    n = 1 << m;

    /* Main loop */
#if FFT_LOGN > RES_LOGN
    q31_t p;
    p = riscv_dsp_recip_table_q31[m - 2]; /* 2 / FFT_N */
#endif

    /* for rIFFT, pre-processing */
    q31_t *ptr =  src;

    i = j = n >> 2;
    while (i != 0)
    {
#if FFT_LOGN > RES_LOGN
        GET_COS_SIN_VALUES(i, c, s, p, q31);
#else
        GET_COS_SIN_VALUES(i, c, s, m, q31);
#endif /* FFT_LOGN > RES_LOGN */

        xt1 = (ptr[2 * i] + ptr[2 * j]);
        xt2 = (ptr[2 * i] - ptr[2 * j]);
        yt1 = (ptr[2 * i + 1] + ptr[2 * j + 1]);
        yt2 = (ptr[2 * i + 1] - ptr[2 * j + 1]);

        /* twiddle and butterfly */
#ifdef ENA_HIGHER_PERFORMANCE
        xt3 = (((q63_t) yt1 * c) >> 31) + (((q63_t) xt2 * s) >> 31);
        yt3 = ((q63_t) xt2 * c  >> 31) - (((q63_t) yt1 * s) >> 31);
#else
        xt3 = (yt1 * c + xt2 * s) >> 31;
        yt3 = (xt2 * c - yt1 * s) >> 31;
#endif
        ptr[2 * i] = (xt1 - xt3);
        ptr[2 * i + 1] = (yt3 + yt2);
        ptr[2 * j] = (xt1 + xt3);
        ptr[2 * j + 1] = (yt3 - yt2);
        j++;
        i--;
    }

    xt1 = ptr[0];
    ptr[0] = (xt1 + ptr[1]);
    ptr[1] = (xt1 - ptr[1]);

    riscv_dsp_cifft_q31(src, m - 1);

    return 0;
}

/**
 * @} end of rfft
 */
