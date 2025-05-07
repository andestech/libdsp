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
#include "internal_filtering_math.h"

/**
 * @addtogroup biqd_casd_df2T
 * @{
 */

/**
 * @brief Process of the direct transposed form II Biquad Cascade floating-point filter.
 * @param[in]       *instance points to an instance of the biquad
 *                            cascade structure.
 * @param[in]       *src      points to the input block data.
 * @param[out]      *dst      points to the output block data.
 * @param[in]       size      number of the size per stage.
 * @return none.
 */

/* function description */
FUNC_ATTR_FFAST_MATH
void riscv_dsp_bq_stereo_df2T_f32(const riscv_dsp_bq_stereo_df2T_f32_t * FUNC_RESTRICT instance,
                                  float32_t * FUNC_RESTRICT src, float32_t * FUNC_RESTRICT dst, uint32_t size)
{
    float32_t *in = src;
    float32_t *out = dst;
    float32_t *state = instance->state;
    float32_t *coeff = instance->coeff;
    float32_t acc0, acc1;
    float32_t b0, b1, b2, a1, a2;
    float32_t xn0, xn1;
    float32_t d00, d01, d10, d11;
    uint32_t i, stage = instance->nstage;

    while (stage != 0u)
    {
        b0 = *coeff++;
        b1 = *coeff++;
        b2 = *coeff++;
        a1 = *coeff++;
        a2 = *coeff++;

        d00 = state[0];
        d10 = state[1];
        d01 = state[2];
        d11 = state[3];

        /* for one stage */
        i = size;
        while (i != 0u)
        {
            xn0 = *in++;
            xn1 = *in++;

            acc0 = b0 * xn0 + d00;
            acc1 = b0 * xn1 + d01;

            *out++ = acc0;
            *out++ = acc1;

            d00 = b1 * xn0 + a1 * acc0 + d10;
            d01 = b1 * xn1 + a1 * acc1 + d11;
            d10 = b2 * xn0 + a2 * acc0;
            d11 = b2 * xn1 + a2 * acc1;

            i --;
        }

        *state++ = d00;
        *state++ = d10;
        *state++ = d01;
        *state++ = d11;

        in = dst;
        out = dst;
        stage --;
    }
}

/**
 * @} end of biqd_casd_df2T
 */
