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
 * @defgroup liir Lattice Infinite impulse response(IIR) Filters
 *
 * The lattice IIR filters are used in a variety of adaptive filter
 * applications. The filter structure has feedforward and feedback components
 * and the net impulse response is infinite length. The following
 * difference equations are implemented:
 *
 * This function operate on blocks of input data <code>src[n]</code> and
 * output data for the length <code>size</code> and each call to the
 * function processes.
 *
 * <pre>
 *      fN[n] = src[n]
 *      fm-1[n] = fm[n] - rm * gm-1[n-1] for m = N, N-1, ...1
 *      gm[n] = rm * fm-1[n] + gm-1[n-1] for m = N, N-1, ...1
 *      dst[n] = vN * gN(n) + vN-1 * gN-1(n) + ...+ v0 * g0(n)
 * </pre>
 * where N is number of stages, Reflection Coefficients <code>rm</code> are
 * stored in {rN, rN-1, ....r1} of the length <code>nstage</code>. The
 * <code>vm[n]</code> is the ladder coefficients of the length
 * <code>nstage + 1</code>. The state variables array <code>gm[n]</code> has
 * the length <code>nstage + size</code> sizes.
 *
 * The coefficients and state variables for a filter are stored together in
 * an instance data structure which is defined as follow. The state
 * variables are updated after each block of data is processed and the
 * coefficients are untouched.
 *
 * <pre>
 *      struct {
 *          uint32_t nstage;
 *          (type) *state;
 *          (type) *rcoeff;
 *          (type) *lcoeff;
 *      } riscv_dsp_liir_(type)_t;
 *
 * where *state  points to the state variable array of size
 *               <code>nstage + size</code>.
 *       *rcoeff points to the reflection coefficient array of size
 *               <code>nstage</code> and the coefficients are stored
 *               in time reversed order.
 *          Ex:  {r[nstage-1], r[nstage-2], ..., r[1]}
 *       *lcoeff points to the ladder coefficient array of size
 *               <code>nstage + 1</code>.
 *          Ex:  {v[nstage-1], v[nstage-2], ..., v[1], v[0]}
 *        nstage number of stages in the filter.
 * </pre>
 */

/**
 * @addtogroup liir
 * @{
 */

/**
 * @brief Function for the floating-point lattice IIR filter.
 * @param[in]       *instance points to an instance of the lattice
 *                            IIR structure.
 * @param[in]       *src      points to the input block data.
 * @param[out]      *dst      points to the output block data.
 * @param[in]       size      number of the blocksize.
 * @return none.
 */

/* function description */
void riscv_dsp_liir_f32(const riscv_dsp_liir_f32_t * FUNC_RESTRICT instance,
                    float32_t * FUNC_RESTRICT src, float32_t * FUNC_RESTRICT dst, uint32_t size)
{
#ifdef ENA_HIGHER_PERFORMANCE_LIIR_F32
    float32_t fnext1, gcurr1, gnext;
    float32_t *px2;
    float32_t k1, k2;
    float32_t v1, v2, v3, v4;
    float32_t gcurr2;
    float32_t fnext2;
#else
    float32_t fcurr, gcurr = 0;
#endif
    float32_t acc;
    uint32_t blkcnt, tapcnt;
    float32_t *pst, *ref, *lad;
    uint32_t nstage = instance->nstage;
    float32_t *state, *stateCurnt;

    state = &instance->state[0];
    blkcnt = size;

#ifdef ENA_HIGHER_PERFORMANCE_LIIR_F32
    while (blkcnt != 0u)
    {
        fnext2 = *src++;
        lad = &instance->lcoeff[0];
        ref = &instance->rcoeff[0];

        pst = state;
        px2 = state;
        acc = 0.0f;

        tapcnt = (nstage) >> 2;

        while (tapcnt > 0u)
        {
            gcurr1 = *pst;
            k1 = *ref;

            fnext1 = fnext2 - (k1 * gcurr1);
            v1 = *lad;

            k2 = *(ref + 1u);
            gcurr2 = *(pst + 1u);
            v2 = *(lad + 1u);
            fnext2 = fnext1 - (k2 * gcurr2);
            gnext = gcurr1 + (k1 * fnext1);

            k1 = *(ref + 2u);
            *px2++ = gnext;
            gcurr1 = *(pst + 2u);
            acc += (gnext * v1);

            fnext1 = fnext2 - (k1 * gcurr1);
            gnext = gcurr2 + (k2 * fnext2);

            gcurr2 = *(pst + 3u);

            acc += (gnext * v2);

            k2 = *(ref + 3u);

            *px2++ = gnext;

            fnext2 = fnext1 - (k2 * gcurr2);
            gnext = gcurr1 + (k1 * fnext1);

            v3 = *(lad + 2u);

            acc += (gnext * v3);

            *px2++ = gnext;
            ref += 4u;

            gnext = (fnext2 * k2) + gcurr2;
            v4 = *(lad + 3u);

            acc += (gnext * v4);
            *px2++ = gnext;

            pst += 4u;
            lad += 4u;
            tapcnt--;
        }

        tapcnt = (nstage) % 0x4u;
        while (tapcnt != 0u)
        {
            gcurr1 = *pst++;
            fnext1 = fnext2 - ((*ref) * gcurr1);
            gnext = (fnext1 * (*ref++)) + gcurr1;
            acc += (gnext * (*lad++));
            *px2++ = gnext;
            fnext2 = fnext1;
            tapcnt--;
        }
        acc += (fnext2 * (*lad));
        *px2++ = fnext2;
        *dst++ = acc;
        state++;
        blkcnt--;
    }
#else
    while (blkcnt != 0u)
    {
        fcurr = *src++;
        pst = state;
        acc = 0.0f;
        ref = &instance->rcoeff[0];
        lad = &instance->lcoeff[0];

        tapcnt = nstage;
        /* stage loop */
        while (tapcnt != 0u)
        {
            gcurr = *pst;
            float32_t val_ref, val_lad;

            val_ref = *ref;
            val_lad = *lad;

            fcurr = fcurr - gcurr * val_ref;
            gcurr = gcurr + fcurr * val_ref;

            acc += (gcurr * val_lad);
            lad++;
            ref++;
            *pst = gcurr;
            pst++;
            tapcnt--;
        }
        acc += (fcurr * (*lad));
        lad++;
        *pst = fcurr;
        *dst++ = acc;
        state++;
        blkcnt--;
    }
#endif

    stateCurnt = &instance->state[0];
    state = &instance->state[size];
    tapcnt = nstage;
    while (tapcnt != 0u)
    {
        *stateCurnt++ = *state++;
        tapcnt--;
    }
}

/**
 * @} end of liir
 */
