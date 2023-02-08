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
extern int32_t riscv_dsp_cfft_f64(float64_t *src, uint32_t m);
extern int32_t riscv_dsp_cifft_f64(float64_t *src, uint32_t m);

/* function description */
int32_t riscv_dsp_rfft_f64(float64_t *src, uint32_t m)
{
    register unsigned int i, j;
    float64_t c, s, xt1, xt2, xt3, yt1, yt2, yt3;
    unsigned int n;
    n = 1 << m;

    /* Main loop */
#if FFT_LOGN > RES_LOGN
    float64_t p = TWO_PI / n;
#endif /* FFT_LOGN > RES_LOGN */

    riscv_dsp_cfft_f64(src, m - 1);

    /* post-processing */
    float64_t *ptr = src;

    /* for rFFT */
    i = j = n >> 2;
    while (i != 0)
    {
#if FFT_LOGN > RES_LOGN
        GET_COS_SIN_VALUES(i, c, s, p, f64);
#else
        GET_COS_SIN_VALUES(i, c, s, m, f64);
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

int32_t riscv_dsp_rifft_f64(float64_t *src, uint32_t m)
{
    register unsigned int i, j;
    float64_t c, s, xt1, xt2, xt3, yt1, yt2, yt3;
    unsigned int n;
    n = 1 << m;

    /* Main loop */
#if FFT_LOGN > RES_LOGN
    float64_t p = TWO_PI / n;
#endif /* FFT_LOGN > RES_LOGN */

    /* for rIFFT, pre-processing */
    float64_t *ptr =  src;

    i = j = n >> 2;
    while (i != 0)
    {
#if FFT_LOGN > RES_LOGN
        GET_COS_SIN_VALUES(i, c, s, p, f64);
#else
        GET_COS_SIN_VALUES(i, c, s, m, f64);
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

    riscv_dsp_cifft_f64(src, m - 1);

    return 0;
}

/**
 * @} end of rfft
 */
