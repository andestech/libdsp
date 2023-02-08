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
#include "internal_isa.h"

/**
 * @ingroup filtering
 */

/**
 * @addtogroup biqd_casd_df1
 * @{
 */

/**
 * @brief Process of the Biquad Cascade q15 filter.
 * @param[in]       *instance points to an instance of the biquad
 *                            cascade structure.
 * @param[in]       *src      points to the input block data.
 * @param[out]      *dst      points to the output block data.
 * @param[in]       size      number of the size per stage.
 * @return none.
 *
 * Function notes:
 * Both inputs are in 1.15 format and multiplications yield a 2.30 result.
 * The 2.30 intermediate results are accumulated in a 64-bit accumulator in
 * 34.30 format. The accumulator is then shifted by <code>shift</code> bits
 * to  truncate the result to 1.15 format by discarding the low 16 bits. The
 * output results is saturated to 1.15 format.
 *
 * If ENA_DSP_ISA and NDS_DSP_ASSUME_ALIGNED then the content of instance->coeff
 * should be arranged as following:
 *     {b0, 0, b1, b2, a0, a1, b0, 0, b1,...} //for simd load
 * otherwise:
 *     {b0, b1, b2, a0, a1, b0, b1,...}
 *
 */
/* function description */
#define OPT
void riscv_dsp_bq_df1_q15(const riscv_dsp_bq_df1_q15_t * FUNC_RESTRICT instance,
                    q15_t * FUNC_RESTRICT src, q15_t * FUNC_RESTRICT dst, uint32_t size)
{
    q15_t *in = src;
    q15_t *out = dst;
    q15_t *state = instance->state;
    q15_t *coeff = instance->coeff;
    q63_t acc;
    q15_t xn;
    q15_t b0;
    q15_t b1, b2, a1, a2;
    q15_t xn1, xn2, yn1, yn2;
    uint32_t i, stage = instance->nstage;
    q31_t temp_acc;
    int32_t lShift = (15 - (int32_t) instance->shift);
    int32_t uShift = (32 - lShift);
    q31_t acc_l, acc_h;

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

        i = size;
        while (i != 0u)
        {
            xn = *in++;

            acc = (q31_t) b0 * xn;
            acc += (q31_t) b1 * xn1;
            acc += (q31_t) b2 * xn2;
            acc += (q31_t) a1 * yn1;
            acc += (q31_t) a2 * yn2;
            acc_l = acc & 0xffffffff;
            acc_h = (acc >> 32) & 0xffffffff;
            temp_acc = (uint32_t) acc_l >> lShift | acc_h << uShift;
            temp_acc = NDS_ISA_SATS(temp_acc, 16u);

            xn2 = xn1;
            xn1 = xn;
            yn2 = yn1;
            yn1 = (q15_t) temp_acc;

            *out++ = (q15_t) temp_acc;
            i --;
        }
        *state++ = xn1;
        *state++ = xn2;
        *state++ = yn1;
        *state++ = yn2;
        in = dst;
        out = dst;
        stage--;
    }
}

/**
 * @} end of biqd_casd_df1
 */
