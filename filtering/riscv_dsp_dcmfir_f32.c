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
 * @ingroup filtering
 */

/**
 * @defgroup dcmfir Decimator FIR Filter
 *
 * This functions implement the Decimator Fir Filter by combine an FIR
 * filter together with a decimator. They are used in multirate systems for
 * reducing the sample rate of a signal without introducing aliasing
 * distortion.
 *
 * When decimating by a factor of <code>M</code>, the signal should be
 * prefiltered by a lowpass filter with a normalized cutoff frequency of
 * <code>1/M</code> in order to prevent aliasing distortion. Instead of
 * calculating all of the FIR filter outputs and discarding <code>M-1</code>
 * out of every <code>M</code>, only the samples output by the decimator are
 * computed.
 *
 * The functions operate on blocks of input and output data. <code>src</code>
 * points to an array for the length <code>size</code> values and the output
 * <code>dst</code> points to an array of the length <code>size/M</code>
 * values. In order to have an interger number of output samples
 * <code>size</code> must always be a multiple of the decimation factor
 * <code>M</code>.
 *
 * <pre>
 *      dst[n] =  f[0] * src[n] + f[1] * src[n-1] + f[2] * src[n-2]
 *              + ...+ f[coeff_size - 1] * src[n - coeff_size + 1]
 *      wher f[n] is the filter coefficients.
 * </pre>
 *
 * The coefficients and state variables for a filter are stored together in
 * an instance data structure which is defined as follow. The state
 * variables are updated after each block of data is processed and the
 * coefficients are untouched.
 *
 * <pre>
 *      struct {
 *          uint32_t M;
 *          uint32_t coeff_size;
 *          (type) *coeff;
 *          (type) *state;
 *      } riscv_dsp_dcmfir_(type)_t;
 *
 * where *state points to the state variable array of size
 *              <code>coeff_size + size - 1</code>.
 *          Ex: {src[n-coeff_size+1], src[n-coeff_size], src[n-coeff_size-1],
 *              src[n-coeff_size-2]....src[0], src[1], ..., x[size-1]}
 *       *coeff points to the coefficient array of size
 *              <code>coeff_size</code> and the coefficients are stored
 *              in time reversed order.
 *          Ex: {b[coeff_size-1], b[coeff_size-2], ..., b[1], b[0]}
 *   coeff_size number of filter coefficients.
 *            M is the decimation factor.
 * </pre>
 */

/**
 * @addtogroup dcmfir
 * @{
 */

/**
 * @brief Function for the floating-point Decimator FIR filter.
 * @param[in]       *instance points to an instance of the Decimator FIR
 *                            structure.
 * @param[in]       *src      points to the input block data.
 * @param[out]      *dst      points to the output block data.
 * @param[in]       size      number of the size.
 * @return none.
 */

/* function description */
void FUNC_ATTR_NO_HWLOOP
riscv_dsp_dcmfir_f32(const riscv_dsp_dcmfir_f32_t * FUNC_RESTRICT instance, float32_t * FUNC_RESTRICT src,
               float32_t * FUNC_RESTRICT dst, uint32_t size)
{
    float32_t *state = instance->state;
    float32_t *scurr, *px, *pb;
    float32_t sum;
    float32_t x0, c0;
    float32_t x1, c1;
    uint32_t csize = instance->coeff_size;
    uint32_t i, tapcnt, blkcnt, bsize = size / instance->M;

    scurr = instance->state + (csize - 1u);

    blkcnt = bsize;
    while (blkcnt != 0u)
    {
        //LOOP_HINT_NO_HWLOOP();
        i = instance->M;
        while (i)
        {
            *scurr++ = *src++;
            i--;
        }

        sum = 0.0f;
        px = state;
        pb = instance->coeff;

        tapcnt = csize >> 1;
        while (tapcnt != 0u)
        {
            c0 = *pb++;
            x0 = *px++;
            c1 = *pb++;
            x1 = *px++;
            sum += (x0 * c0);
            sum += (x1 * c1);
            tapcnt--;
        }
        tapcnt = csize & 1;
        if (tapcnt != 0)
        {
            c0 = *pb++;
            x0 = *px++;
            sum += (x0 * c0);
        }

        state = state + instance->M;
        *dst++ = sum;
        blkcnt--;
    }

    scurr = instance->state;

    i = (csize - 1u);
    while (i != 0u)
    {
        //LOOP_HINT_NO_HWLOOP();
        *scurr++ = *state++;
        i--;
    }
}

/**
 * @} end of dcmfir
 */
