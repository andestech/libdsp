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

/* function description */
FUNC_ATTR_FFAST_MATH
void riscv_dsp_bq_df1_f16(const riscv_dsp_bq_df1_f16_t * FUNC_RESTRICT instance,
                          float16_t * FUNC_RESTRICT src, float16_t * FUNC_RESTRICT dst, uint32_t size)
{
    float16_t *in = src;
    float16_t *out = dst;
    float16_t *state = instance->state;
    float16_t *coeff = instance->coeff;
    float16_t acc0, acc1;
    float16_t b0, b1, b2, a1, a2;
    float16_t t0_xn, t0_xn1, t0_xn2, t0_yn1, t0_yn2;
    float16_t t1_xn;
    uint32_t i, stage = instance->nstage;


    while (stage != 0u)
    {
        b0 = *coeff++;
        b1 = *coeff++;
        b2 = *coeff++;
        a1 = *coeff++;
        a2 = *coeff++;

        t0_xn1 = state[0];
        t0_xn2 = state[1];
        t0_yn1 = state[2];
        t0_yn2 = state[3];

        /* for one stage */
        i = size >> 1;
        while (i != 0u)
        {
            t0_xn = *in++;
            acc0 = (b0 * t0_xn) + (b1 * t0_xn1) + (b2 * t0_xn2) + (a1 * t0_yn1) + (a2 * t0_yn2);
            t1_xn = *in++;
            acc1 = (b0 * t1_xn) + (b1 * t0_xn) + (b2 * t0_xn1) + (a1 * acc0) + (a2 * t0_yn1);
            *out++ = acc0;
            *out++ = acc1;

            t0_xn1 = t1_xn;
            t0_xn2 = t0_xn;
            t0_yn1 = acc1;
            t0_yn2 = acc0;
            i --;
        }
        i = size & 1;
        if (i != 0)
        {
            t0_xn = *in++;
            acc0 = (b0 * t0_xn) + (b1 * t0_xn1) + (b2 * t0_xn2) + (a1 * t0_yn1) + (a2 * t0_yn2);
            *out++ = acc0;
            t0_xn2 = t0_xn1;
            t0_xn1 = t0_xn;
            t0_yn2 = t0_yn1;
            t0_yn1 = acc0;
        }

        *state++ = t0_xn1;
        *state++ = t0_xn2;
        *state++ = t0_yn1;
        *state++ = t0_yn2;

        in = dst;
        out = dst;
        stage --;
    }
}
