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
 * @defgroup upsplfir Upsampling FIR Filter
 *
 * This functions implement the Upsampling Fir Filter by combine an FIR
 * filter together with a upsampler. They are used in multirate systems for
 * increasing the sample rate of a signal without introducing high frequency
 * images.
 *
 * When upsampling by a factor of <code>L</code>, the signal should be
 * filtered by a lowpass filter with a normalized cutoff frequency of
 * <code>1/L</code> in order to prevent aliasing distortion. The upsampler
 * inserts <code>L-1</code> zeros between each sample.
 *
 * The functions operate on blocks of input and output data. <code>src</code>
 * points to an array for the length <code>size</code> values and the output
 * <code>dst</code> points to an array of the length <code>size * L</code>
 * values. The functions use a polyphase filter structure:
 *
 * <pre>
 *      dst[n] =  b[0] * src[n] + b[L]   * src[n - 1] + ...
 *                  + b[L * (plen - 1)] * src[n - plen + 1]
 *      dst[n + 1] =  b[1] * src[n] + b[L + 1]   * src[n - 1] + ...
 *                      + b[L * (plen - 1) + 1] * src[n - plen + 1]
 *          ...
 *      dst[n + (L - 1)] =  b[L - 1] * src[n] + b[2 * L - 1]   * src[n - 1]
 *                  + ... + b[L * (plen - 1) + (L - 1)] * src[n - plen + 1]
 *      wher b[n] is the filter coefficients.
 * </pre>
 *
 * The coefficients and state variables for a filter are stored together in
 * an instance data structure which is defined as follow. The state
 * variables are updated after each block of data is processed and the
 * coefficients are untouched.
 *
 * <pre>
 *      struct {
 *          uint32_t L;
 *          uint32_t plen;
 *          (type) *coeff;
 *          (type) *state;
 *      } riscv_dsp_upsplfir_(type)_t;
 *
 * where *state points to the state variable array of size
 *              <code>plen + size - 1</code>.
 *       *coeff points to the coefficient array of size <code>L*plen</code>.
 *         plen length of the polyphase filter.
 *            L is the upsampling factor.
 * </pre>
 */

/**
 * @addtogroup upsplfir
 * @{
 */

/**
 * @brief Function for the floating-point Upsampling FIR filter.
 * @param[in]       *instance points to an instance of the Upsampling FIR
 *                            structure.
 * @param[in]       *src      points to the input block data.
 * @param[out]      *dst      points to the output block data.
 * @param[in]       size      number of the size.
 * @return none.
 */

/* function description */
void FUNC_ATTR_NO_HWLOOP
riscv_dsp_upsplfir_f32(const riscv_dsp_upsplfir_f32_t * FUNC_RESTRICT instance, float32_t * FUNC_RESTRICT src,
                   float32_t * FUNC_RESTRICT dst, uint32_t size)
{
    float32_t *state = instance->state;
    float32_t *scurr, *px, *pb;
    float32_t sum;
    uint32_t plen = instance->plen;
    uint32_t tapcnt, blkcnt;
    int32_t i;

    scurr = instance->state + (plen - 1u);

    blkcnt = size;
    while (blkcnt != 0u)
    {
        *scurr++ = *src++;

        i = (int32_t) instance->L - 1;
        while (i != 0)
        {
            sum = 0.0f;
            px = state;
            pb = instance->coeff + i;

            tapcnt = plen;
            while (tapcnt != 0u)
            {
                sum += *px++ **pb;
                pb += instance->L;
                tapcnt--;
            }

            *dst++ = sum;
            i--;
        }
        sum = 0.0f;
        px = state;
        pb = instance->coeff;

        tapcnt = plen;
        while (tapcnt != 0u)
        {
            sum += *px++ **pb;
            pb += instance->L;
            tapcnt--;
        }

        *dst++ = sum;

        state = state + 1;
        blkcnt--;
    }

    scurr = instance->state;

    i = (plen - 1u);
    while (i != 0u)
    {
        *scurr++ = *state++;
        i --;
    }
}

/**
 * @} end of upsplfir
 */
