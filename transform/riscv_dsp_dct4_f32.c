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
#include "internal_utils_math.h"
#include "internal_transform_math.h"

extern void riscv_dsp_dct_f32(float32_t *src, uint32_t m);

/**
 * @ingroup transform
 */

/**
 * @defgroup dct4 DCT Type IV Functions
 *
 * A discrete cosine transform (DCT) expresses a finite sequence of data
 * points in terms of a sum of cosine functions oscillating at different
 * frequencies. In particular, a DCT is a Fourier-related transform similar
 * to the discrete Fourier transform (DFT), but using only real numbers.
 * This function implements DCT Type IV Function:
 *
 * <pre>
 *            N-1
 *     y[k] = sum x[n] * cos(PI * (2 * k + 1) * (2 * n + 1) / (4 * N))
 *            n=0
 * </pre>
 *
 * Implementation of N such that
 *   1] N = 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, and 8192.
 *   2] m,  If not defined STATIC_VERSION, defined as log2(N), is less than
 *          RES_LOGN.
 */

/**
 * @addtogroup dct4
 * @{
 */

/**
 * @brief Function to implement the f32 DCT type IV function.
 * @param[in]       *src     points to the input block data.
 * @param[in]       m        If not defined STATIC_VERSION, the data buffer
 *                           is of size 2 * n while n = 2 ** m.
 * @return none.
 */

/* function description */

void FUNC_ATTR_NO_HWLOOP
riscv_dsp_dct4_f32(float32_t *src, uint32_t m)
{
    /* N > 4: (N / 2) * log2(N) + N / 4 multiplications/divisions */
    /*        (2 * N) * log2(N) - 9 * N / 4 + 1
     **                        additions/subtractions */
    /*  (only (3 * N / 2) * log2(N) - 5 * N / 4 + 1 if maintaining
     **      table of 1 / (2 * cosine)) */
    uint32_t n = (1 << m);
    uint32_t factor = 1 << (RES_LOGN - 2 - m);
    const float32_t *scale = &riscv_dsp_cos_table_f32_linear[(factor >> 1)];
    float32_t *ptre = &src[0], *ptro;

    /* preprocessing */
    ptro = &src[n];
    while (ptre != ptro)
    {
        float32_t data = *ptre;
        *ptre++ = (data * 2.0f * (*scale));
        scale += factor;
    }

    /* preprocessing */
    riscv_dsp_dct_f32(src, m);

    ptro = &src[n - 1];

    /* postprocessing */
    src[0] *= 0.5f;
    ptre = &src[0];
    while (ptre != ptro)
    {
        ptre[1] -= ptre[0];
        ptre++;
    }
}

void FUNC_ATTR_NO_HWLOOP
riscv_dsp_idct4_f32(float32_t *src, uint32_t m)
{

    float32_t *ptre = &src[0], *ptro;
    ptro = &src[1 << m];
    float32_t tmp = riscv_dsp_recip_table_f32[m - 2];
    riscv_dsp_dct4_f32(src, m);

    while (ptre != ptro)
    {
        *ptre++ *= tmp;
    }

}

/**
 * @} end of dct4
 */
