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
 * @defgroup lfir Lattice FIR Filters
 *
 * Finite impulse response (FIR) filter is a filter whose impulse response
 * (or response to any finite length input) is of finite duration, because
 * it settles to zero in finite time. The lattice FIR filters are used in
 * a variety of adaptive filter applications. The filter structure is
 * feedforward and the net impulse response is finite length. The following
 * difference equations are implemented:
 *
 * This function operate on blocks of input data <code>src[n]</code> and
 * output data for the length <code>size</code> and each call to the
 * function processes.
 *
 * <pre>
 *      f0[n] = g0[n] = src[n]
 *      fm[n] = fm-1[n] + km * gm-1[n-1] for m = 1, 2, ...M
 *      gm[n] = km * fm-1[n] + gm-1[n-1] for m = 1, 2, ...M
 *      dst[n] = fM[n]
 * </pre>
 * where M is number of stages, Reflection Coefficients <code>km</code> are
 * stored in {k1, k2, ..., kM}. The <code>fm[n]</code> is the stage output.
 * The state variables <code>gm[n]</code> hold previous inputs and are
 * stored as {g0[n], g1[n], g2[n] ...gM-1[n]}.
 *
 * The coefficients and state variables for a filter are stored together in
 * an instance data structure which is defined as follow. The state
 * variables are updated after each block of data is processed and the
 * coefficients are untouched.
 *
 * <pre>
 *      struct {
 *          uint32_t stage;
 *          (type) *state;
 *          (type) *coeff;
 *      } riscv_dsp_lfir_(type)_t;
 *
 * where *state points to the state variable array of size
 *              <code>stage + size - 1</code>.
 *       Ex: {src[n-stage+1], src[n-stage], src[n-stage-1],
 *          src[n-stage-2]....src[0], src[1], ..., x[size-1]}
 *       *coeff points to the coefficient array of size
 *              <code>stage</code> and the coefficients are stored
 *              in time reversed order.
 *       Ex: {b[stage-1], b[stage-2], b[N-2], ..., b[1], b[0]}
 *       stage number of stages..
 * </pre>
 */

/**
 * @addtogroup lfir
 * @{
 */

/**
 * @brief Function for the floating-point lattice FIR filter.
 * @param[in]       *instance points to an instance of the lattice
 *                            FIR structure.
 * @param[in]       *src      points to the input block data.
 * @param[out]      *dst      points to the output block data.
 * @param[in]       size      number of the blocksize.
 * @return none.
 */

/* function description */
void riscv_dsp_lfir_f32(const riscv_dsp_lfir_f32_t * FUNC_RESTRICT instance,
                    float32_t * FUNC_RESTRICT src, float32_t * FUNC_RESTRICT dst, uint32_t size)
{
    /* process the whole block */
    while (size != 0)
    {
        float32_t *px;
        float32_t *pk;
        float32_t fcurr, fnext, gcurr, gnext;
        uint32_t stage;

        /* k1, k2, k3, ..., kM */
        pk = instance->coeff;
        px = instance->state;

        /* get f0(n) and g0(n-1) */
        fcurr = *src++;
        gcurr = *px;

        /* f1(n) = f0(n) + K1 * g0(n-1) */
        /* g1(n) = f0(n) * K1 + g0(n-1) */
        fnext = fcurr + ((*pk) * gcurr);
        gnext = (fcurr * (*pk++)) + gcurr;
        *px++ = fcurr;
        fcurr = fnext;

        stage = instance->stage - 1;

        /* stage loop */
        while (stage != 0)
        {
            gcurr = *px;
            *px++ = gnext;
            fnext = fcurr + ((*pk) * gcurr);
            gnext = (fcurr * (*pk++)) + gcurr;
            fcurr = fnext;
            stage--;
        }

        *dst++ = fcurr;
        size--;
    }
}

/**
 * @} end of lfir
 */
