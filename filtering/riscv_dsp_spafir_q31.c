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
 * @addtogroup spafir
 * @{
 */

/**
 * @brief Function for the q31 Sparse FIR filter.
 * @param[in]       *instance points to an instance of the Sparse FIR
 *                            structure.
 * @param[in]       *src      points to the input block data.
 * @param[out]      *dst      points to the output block data.
 * @param[in]       *buf     points to a temporary buffer of length size.
 * @param[in]       size      number of the size.
 * @return none.
 *
 * Function notes:
 * Both coeicients and state variables are represented in 1.31 format
 * and multiplications truncated to a 2.30 result. The function is
 * implemented using an internal 32-bit accumulator. In order to avoid
 * overflows the input signal or coeicients  must be scaled down by
 * log2(coe_size) bits.
 */

/* function description */
void riscv_dsp_spafir_q31(riscv_dsp_spafir_q31_t * FUNC_RESTRICT instance, q31_t * FUNC_RESTRICT src,
                          q31_t * FUNC_RESTRICT dst, q31_t * FUNC_RESTRICT buf, uint32_t size)
{
    q31_t *state = instance->state;
    q31_t *coeff = instance->coeff;
    q31_t *px;
    q31_t *py = state;
    q31_t *pb = buf;
    q31_t *pOut;
    int32_t *nezdelay = instance->nezdelay;
    uint32_t delsize = instance->delay + size;
    uint16_t coeff_size = instance->coeff_size;
    int32_t rinx;
    uint32_t tapcnt, blkcnt;
    q31_t coe = *coeff++;
    q31_t in;

    dsp_cirwrite_f32((int32_t *) py, delsize, &instance->index, 1,
                      (int32_t *) src, 1, size);

    rinx = (int32_t) (instance->index - size) - *nezdelay++;
    if (rinx < 0)
    {
        rinx += (int32_t) delsize;
    }

    py = state;
    dsp_cirread_f32((int32_t *) py, delsize, &rinx, 1,
                     (int32_t *) pb, (int32_t *) pb, size, 1, size);
    px = pb;
    pOut = dst;

    blkcnt = size;
    while (blkcnt != 0u)
    {
    *pOut++ = (q31_t) (((q63_t) * px++ * coe) >> 32);
        blkcnt--;
    }

    coe = *coeff++;
    rinx = (int32_t) (instance->index - size) - *nezdelay++;
    if (rinx < 0 )
    {
        rinx += (int32_t) delsize;
    }

    tapcnt = (uint32_t) coeff_size - 1u;
    while (tapcnt != 0u)
    {
        py = state;
        dsp_cirread_f32((int32_t *) py, delsize, &rinx, 1,
                         (int32_t *) pb, (int32_t *) pb, size, 1, size);
        px = pb;
        pOut = dst;
        blkcnt = size;
        while (blkcnt != 0u)
        {
            //*pOut += ((q63_t) * px++ * coe) >> 32;
            q63_t out = *pOut;
            out += ((q63_t) * px++ * coe) >> 32;
            *pOut++ = (q31_t) (out);
            blkcnt--;
        }

        coe = *coeff++;
        rinx = (int32_t) (instance->index - size) - *nezdelay++;
        if (rinx < 0)
        {
            rinx += (int32_t) delsize;
        }
        tapcnt--;
    }

    pOut = dst;
    blkcnt = size;
    while (blkcnt != 0u)
    {
        in = *pOut << 1;
        *pOut++ = in;
        blkcnt--;
    }
}

/**
 * @} end of spafir
 */
