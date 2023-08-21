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

//Declare the external functions
extern int32_t riscv_dsp_cfft_f32(float32_t *src, uint32_t m);
extern int32_t riscv_dsp_cifft_f32(float32_t *src, uint32_t m);

/**
 * @ingroup transform
 */

/**
 * @defgroup rfft Real FFT Functions
 *
 * This function implements the real FFT of N points. It is calculated using
 * complex FFT of N / 2 points:
 * Real:    x[0], x[1], x[2], x[3], ..., x[N -2], x[N -1]
 * Complex: re[0], im[0], re[1], im[1], ..., re[N / 2 - 2], im[N / 2 - 1]
 */

/**
 * @addtogroup rfft
 * @{
 */

/**
 * @brief Function to implement the floating-point Real FFT Functions
 * @param[in]       *src     points to the input block data, src[2 * k] and
 *                           src[2 * k + 1] contain the real and imaginary
 *                           parts of the k-th data.
 * @param[in]       m        If not defined STATIC_VERSION, the data buffer
 *                           is of size 2 * n while n = 2 ** m.
 * @param[in]       inv      0 for forward transform, otherwise for
 *                           backward transform
 * @return valaue   0  success.
 *                  -1 invalid range.
 */

/* function description */
int32_t riscv_dsp_rfft_f32(float32_t *src, uint32_t m)
{
    register unsigned int i, j;
    float32_t c, s, xt1, xt2, xt3, yt1, yt2, yt3;
    unsigned int n;
    n = 1 << m;

    /* Main loop */
#if FFT_LOGN > RES_LOGN
    float32_t p = TWO_PI / n;
#endif /* FFT_LOGN > RES_LOGN */

    riscv_dsp_cfft_f32(src, m - 1);

    /* post-processing */
    float32_t *ptr = src;

    /* for rFFT */
    i = j = n >> 2;
    while (i != 0)
    {
#if FFT_LOGN > RES_LOGN
        GET_COS_SIN_VALUES(i, c, s, p, f32);
#else
        GET_COS_SIN_VALUES(i, c, s, m, f32);
#endif /* FFT_LOGN > RES_LOGN */
        xt1 = 0.5f * (ptr[2 * i] + ptr[2 * j]);
        xt2 = 0.5f * (ptr[2 * i] - ptr[2 * j]);
        yt1 = 0.5f * (ptr[2 * i + 1] + ptr[2 * j + 1]);
        yt2 = 0.5f * (ptr[2 * i + 1] - ptr[2 * j + 1]);

        /* twiddle and butterfly */
        xt3 = yt1 * c - xt2 * s;
        yt3 = -(xt2 * c + yt1 * s);
        ptr[2 * i] = xt1 + xt3;
        ptr[2 * i + 1] = yt3 + yt2;
        ptr[2 * j] = xt1 - xt3;
        ptr[2 * j + 1] = yt3 - yt2;
        j++;
        i--;
    }

    xt1 = ptr[0];
    ptr[0] += ptr[1];
    ptr[1] = xt1 - ptr[1];

    return 0;
}

int32_t riscv_dsp_rifft_f32(float32_t *src, uint32_t m)
{
    register unsigned int i, j;
    float32_t c, s, xt1, xt2, xt3, yt1, yt2, yt3;
    unsigned int n;
    n = 1 << m;

    /* Main loop */
#if FFT_LOGN > RES_LOGN
    float32_t p = TWO_PI / n;
#endif /* FFT_LOGN > RES_LOGN */

    /* for rIFFT, pre-processing */
    float32_t *ptr =  src;

    i = j = n >> 2;
    while (i != 0)
    {
#if FFT_LOGN > RES_LOGN
        GET_COS_SIN_VALUES(i, c, s, p, f32);
#else
        GET_COS_SIN_VALUES(i, c, s, m, f32);
#endif /* FFT_LOGN > RES_LOGN */

        xt1 = 0.5f * (ptr[2 * i] + ptr[2 * j]);
        xt2 = 0.5f * (ptr[2 * i] - ptr[2 * j]);
        yt1 = 0.5f * (ptr[2 * i + 1] + ptr[2 * j + 1]);
        yt2 = 0.5f * (ptr[2 * i + 1] - ptr[2 * j + 1]);

        /* twiddle and butterfly */
        xt3 = yt1 * c + xt2 * s;
        yt3 = xt2 * c - yt1 * s;
        ptr[2 * i] = xt1 - xt3;
        ptr[2 * i + 1] = yt3 + yt2;
        ptr[2 * j] = xt1 + xt3;
        ptr[2 * j + 1] = yt3 - yt2;
        j++;
        i--;
    }

    xt1 = ptr[0];
    ptr[0] = 0.5f * (xt1 + ptr[1]);
    ptr[1] = 0.5f * (xt1 - ptr[1]);

    riscv_dsp_cifft_f32(src, m - 1);

    return 0;
}

/**
 * @} end of rfft
 */
