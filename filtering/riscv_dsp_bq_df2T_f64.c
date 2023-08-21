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
void riscv_dsp_bq_df2T_f64(const riscv_dsp_bq_df2T_f64_t * FUNC_RESTRICT instance,
                     float64_t * FUNC_RESTRICT src, float64_t * FUNC_RESTRICT dst, uint32_t size)
{
    float64_t *in = src;
    float64_t *out = dst;
    float64_t *state = instance->state;
    float64_t *coeff = instance->coeff;
    float64_t acc, acc2;
    float64_t b0, b1, b2, a1, a2;
    float64_t xn, xn2;
    float64_t d0, d1;
    float64_t tmp1;
    uint32_t i, stage = instance->nstage;
    uint32_t hsize = size >> 1;
    uint32_t rest = size & 1;

    while (stage != 0u)
    {
        b0 = *coeff++;
        b1 = *coeff++;
        b2 = *coeff++;
        a1 = *coeff++;
        a2 = *coeff++;

        d0 = state[0];
        d1 = state[1];

        /* for one stage */
        i = hsize;
        while (i != 0u)
        {
            xn = *in++;
            xn2 = *in++;
            acc = (b0 * xn) + d0;
            tmp1 = (a1 * acc) + d1;
            d0 = (b1 * xn) + tmp1;
            acc2 = (b0 * xn2) + d0;
            d1 = (b2 * xn) + (a2 * acc);
            tmp1 = (a1 * acc2) + d1;
            d0 = (b1 * xn2) + tmp1;
            d1 = (b2 * xn2) + (a2 * acc2);
            *out++ = acc;
            *out++ = acc2;
            i--;
        }
        if (rest != 0)
        {
            xn = *in++;
            acc = (b0 * xn) + d0;
            *out++ = acc;
            tmp1 = (a1 * acc) + d1;
            d0 = (b1 * xn) + tmp1;
            d1 = (b2 * xn) + (a2 * acc);
        }

        *state++ = d0;
        *state++ = d1;

        in = dst;
        out = dst;
        stage --;
    }
}

/**
 * @} end of biqd_casd_df2T
 */
