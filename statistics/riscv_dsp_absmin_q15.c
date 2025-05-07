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
#include "internal_isa.h"


/**
 * @ingroup statistics
 */

/**
 * @addtogroup AbsMin
 * @{
 */

/**
 * @brief           Minimum value of absolute values of a Q15 vector.
 * @param[in]       *src    points to the input vector.
 * @param[in]       size    size of the vectors.
 * @param[out]      *index  index of the minimum value.
 * @return minimum value.
 */

/* function description */
q15_t riscv_dsp_absmin_q15(const q15_t* FUNC_RESTRICT src, uint32_t size, uint32_t* FUNC_RESTRICT index)
{
    //First, check if input size is zero?
    if (size == 0)
    {
        *index = 0;
        return (q15_t)0x8000;
    }

    uint32_t min_index, i;
    i = min_index = 0U;

#if defined(ENA_DSP_ISA_32) && defined(ENA_NDS_V5_EXT)
    q15_t min_val, temp_val;
    uint32_t tmp_index = 0;
    suni_t vala;
    suni_t valb;
    vala.l0 = 0x7FFF7FFF;
    const q15_t* tmpsrc = src;

    i = size;
    // find the minimum value
    while (i >= 2)
    {
        valb = simd_load_16((q15_t*)src, 0);
        valb.l0 = NDS_DSP_KABS16(valb.l0);
        src += SIMD_NUM_Q15;
        vala.l0 = NDS_DSP_SMIN16(vala.l0, valb.l0);
        i -= SIMD_NUM_Q15;
    }
    min_val = vala.b16.h0;
    min_val = (min_val > vala.b16.h1) ? vala.b16.h1 : min_val;

    //find index
    while (1)
    {
        valb = simd_load_16((q15_t*)tmpsrc, 0);
        valb.l0 = NDS_DSP_KABS16(valb.l0);
        tmpsrc += SIMD_NUM_Q15;
        if (valb.b16.h0 == min_val)
            break;

        if (valb.b16.h1 == min_val)
        {
            tmp_index++;
            break;
        }
        tmp_index += SIMD_NUM_Q15;
    }
    min_index = tmp_index;

    //remainder
    while (i != 0)
    {
        temp_val = NDS_DSP_KABS16(*src++);
        min_index = (min_val > temp_val) ? (size - i) : min_index;
        min_val = (min_val > temp_val) ? temp_val : min_val;
        i--;
    }
#else
    //pureC
    q15_t min_val, temp_val;
    min_val = (*src > 0) ? *src : ((*src == (q15_t)0x8000) ? 0x7fff : -*src);
    src++;

    while (i < (size - 1))
    {
        temp_val = (*src > 0) ? *src : ((*src == (q15_t)0x8000) ? 0x7fff : -*src);
        src++;
        i++;
        if (min_val > temp_val)
        {
            min_index = i;
            min_val = temp_val;
        }
    }

#endif

    *index = min_index;
    return min_val;

}

/**
 * @} end of min.
 */
