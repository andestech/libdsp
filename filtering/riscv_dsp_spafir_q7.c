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
#include "internal_isa.h"
/**
 * @ingroup filtering
 */

/**
 * @addtogroup spafir
 * @{
 */

/**
 * @brief Function for the q7 Sparse FIR filter.
 * @param[in]       *instance points to an instance of the Sparse FIR
 *                            structure.
 * @param[in]       *src      points to the input block data.
 * @param[out]      *dst      points to the output block data.
 * @param[in]       *buf1     points to a temporary buffer of length size.
 * @param[in]       *buf2     points to a temporary buffer of length size.
 * @param[in]       size      number of the size.
 * @return none.
 *
 * Function notes:
 * Both coefficients and state variables are represented in 1.7 format
 * and multiplications yield a 2.14 result and these are added to a
 * 32-bit accumulator. After all multiply-accumulates are performed, the
 * 2.14 accumulator is accumulator in 18.14 format and then converted to
 * 18.7 format by discarding the low 7 bit.
 */
/* function description */
void riscv_dsp_spafir_q7(riscv_dsp_spafir_q7_t * FUNC_RESTRICT instance, q7_t * FUNC_RESTRICT src,
                         q7_t * FUNC_RESTRICT dst, q7_t * FUNC_RESTRICT buf1, q31_t * FUNC_RESTRICT buf2, uint32_t size)
{
    q7_t *state = instance->state;
    q7_t *pIn = src, *pOut = dst;
    q7_t *coeff = instance->coeff;
    q7_t *px, *pb = buf1, *py = state;
    int32_t *nezdelay = instance->nezdelay;
    uint32_t delsize = instance->delay + size;
    uint16_t coeff_size = instance->coeff_size;
    int32_t rinx;
    uint32_t tapcnt, blkcnt;
    q7_t coe;
    q31_t *pScr2 = buf2;

    dsp_cirwrite_q7(py, delsize, &instance->index, 1, pIn, 1, size);
    tapcnt = coeff_size;
    rinx = (instance->index - size) - *nezdelay++;

    if (rinx < 0)
    {
        rinx += (int32_t) delsize;
    }

    py = state;
    dsp_cirread_q7(py, delsize, &rinx, 1, pb, pb, size, 1, size);
    px = pb;
    buf2 = pScr2;

    coe = *coeff++;
    blkcnt = size;
    while (blkcnt != 0u)
    {
        *buf2++ = ((q31_t) * px++ * coe);
        blkcnt--;
    }
    coe = *coeff++;
    rinx = (instance->index - size) - *nezdelay++;

    if (rinx < 0)
    {
        rinx += (int32_t) delsize;
    }


    tapcnt = (uint32_t) coeff_size - 1u;
    while (tapcnt != 0u)
    {
        py = state;
        dsp_cirread_q7(py, delsize, &rinx, 1, pb, pb, size, 1, size);
        px = pb;
        buf2 = pScr2;

        blkcnt = size;
        while (blkcnt != 0u)
        {
            *buf2 += (q31_t) * px++ * coe;
            buf2++;
            blkcnt--;
        }
        coe = *coeff++;

        rinx = (instance->index - size) - *nezdelay++;
        if (rinx < 0)
        {
            rinx += (int32_t) delsize;
        }
        tapcnt--;
    }

    blkcnt = size;
    while (blkcnt != 0u)
    {
        *pOut++ = (q7_t) NDS_ISA_SATS(*pScr2++ >> 7, 8u);
        blkcnt--;
    }
}

/**
 * @} end of spafir
 */
