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
 * @ingroup filtering
 */

/**
 * @addtogroup biqd_casd_df1
 * @{
 */

/**
 * @brief Process of the Biquad Cascade q31 filter.
 * @param[in]       *instance points to an instance of the biquad
 *                            cascade structure.
 * @param[in]       *src      points to the input block data.
 * @param[out]      *dst      points to the output block data.
 * @param[in]       size      number of the size per stage.
 * @return none.
 *
 * Function notes:
 * Both inputs are in 1.31 format and multiplications yield a 2.62 result.
 * The 2.30 intermediate results are accumulated in a 64-bit accumulator in
 * 34.30 format. In order to avoid overflows completely the input signal
 * must be scaled down by 2 bits and lie in the range [-0.25 +0.25). After
 * all 5 multiply-accumulates are performed, the accumulator is shifted by
 * <code>shift</code> bits and the result truncated to 1.31 format by
 * discarding the low 32 bits.
 */

/* function description */
void riscv_dsp_bq_df1_fast_q31(const riscv_dsp_bq_df1_q31_t * FUNC_RESTRICT instance,
                         q31_t * FUNC_RESTRICT src, q31_t * FUNC_RESTRICT dst, uint32_t size)
{
    q31_t *in = src;
    q31_t *out = dst;
    q31_t *state = instance->state;
    q31_t *coeff = instance->coeff;
    q31_t acc;
    q31_t b0;
    q31_t xn;
    q31_t xn1, xn2, yn1, yn2;
    q31_t b1, b2, a1, a2;
    uint32_t i, stage = instance->nstage;
    uint32_t uShift = ((uint32_t) instance->shift + 1u);

    while (stage != 0u)
    {
        b0 = *coeff++;
        b1 = *coeff++;
        b2 = *coeff++;
        a1 = *coeff++;
        a2 = *coeff++;

        xn1 = state[0];
        xn2 = state[1];
        yn1 = state[2];
        yn2 = state[3];

        /* for one stage */
        i = size;
        while (i != 0u)
        {
            xn = *in++;
            acc = (q31_t) (((q63_t) b0 * xn + 0x80000000LL ) >> 32);
            acc = (q31_t) (((((q63_t) acc) << 32) + ((q63_t) b1 * xn1) + 0x80000000LL ) >> 32);
            acc = (q31_t) (((((q63_t) acc) << 32) + ((q63_t) b2 * xn2) + 0x80000000LL ) >> 32);
            acc = (q31_t) (((((q63_t) acc) << 32) + ((q63_t) a1 * yn1) + 0x80000000LL ) >> 32);
            acc = (q31_t) (((((q63_t) acc) << 32) + ((q63_t) a2 * yn2) + 0x80000000LL ) >> 32);
            acc = acc << uShift;
            xn2 = xn1;
            xn1 = xn;
            yn2 = yn1;
            yn1 = acc;
            *out++ = acc;
            i --;
        }
        *state++ = xn1;
        *state++ = xn2;
        *state++ = yn1;
        *state++ = yn2;
        in = dst;
        out = dst;
        stage --;
    }
}

/**
 * @} end of biqd_casd_df1
 */
