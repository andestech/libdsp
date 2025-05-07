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
 * @defgroup biqd_casd_df1 Biquad Cascade Filters Direct Form I
 *
 * Biquad filter is a second-order recursive linear filter, containing two
 * poles and two zeros. The most straightforward implementation is the
 * Direct Form 1, which has the following difference equation:
 *
 * <pre>
 *      y[n] = b0 * x[n] + b1 * x[n-1] + b2 * x[n-2] + a1 * y[n-1]
 *              + a2 * y[n-2].
 * </pre>
 * A Direct Form I algorithm is used with 5 coefficients and 4 state
 * variables per stage. In this case the feedback coefficients
 * <code>a1</code> and <code>a2</code> must be negated.
 *
 * Higher order filters are realized as a cascade of second order sections.
 * The coefficients and state variables for a filter are stored together in
 * an instance data structure, which has the following instance structure :
 *
 * <pre>
 *      For the floating-point instance:
 *          typedef struct {
 *              uint32_t nstage;
 *              float32_t *state;
 *              float32_t *coeff;
 *          } riscv_dsp_biquad_df1_f32_t;
 *
 *      For the q15 and q31 instance:
 *          typedef struct {
 *              uint32_t nstage;
 *              (type)_t *state;
 *              (type)_t *coeff;
 *              int8_t   shift;
 *          } riscv_dsp_biquad_df1_(type)_t;
 * </pre>
 * where ntages refers to the number of second order stages used.
 *              Ex: A 10th order filter would be realized with
 *              <code>nstage=5</code> second order stages. A 7th order
 *              filter would be realized with <code>nstage = 4</code> second
 *              order stages with the coefficients for one of the stages
 *              configured as a first order filter (<code>b2=0</code> and
 *              <code>a2 = 0</code>).
 *        state points to state variables array of the length
 *              <code>4 * nstage</code>.
 *              Ex : Arranged as {x[n-1], x[n-2], y[n-1], y[n-2]}
 *        coeff points to the coefficients array of the length
 *              <code>5 * nstage</code>.
 *              Ex : Arranged as {b0, b1, b2, a1, a2}.
 *        shift shift to be applied.
 */

/**
 * @addtogroup biqd_casd_df1
 * @{
 */

/**
 * @brief Process of the Biquad Cascade floating-point filter.
 * @param[in]       *instance points to an instance of the biquad
 *                            cascade structure.
 * @param[in]       *src      points to the input block data.
 * @param[out]      *dst      points to the output block data.
 * @param[in]       size      number of the size per stage.
 * @return none.
 */

/* function description */
FUNC_ATTR_FFAST_MATH
void riscv_dsp_bq_df1_f32(const riscv_dsp_bq_df1_f32_t * FUNC_RESTRICT instance,
                          float32_t * FUNC_RESTRICT src, float32_t * FUNC_RESTRICT dst, uint32_t size)
{
    float32_t *in = src;
    float32_t *out = dst;
    float32_t *state = instance->state;
    float32_t *coeff = instance->coeff;
    float32_t acc0, acc1;
    float32_t b0, b1, b2, a1, a2;
    float32_t t0_xn, t0_xn1, t0_xn2, t0_yn1, t0_yn2;
    float32_t t1_xn;
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
            //acc0 = (b0 * t2_xn) + (b1 * t1_xn) + (b2 * t0_xn) + (a1 * acc1) + (a2 * acc0);
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

/**
 * @} end of biqd_casd_df1
 */
