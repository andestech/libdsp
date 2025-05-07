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
#include "riscv_dsp_transform_math.h"
#include "riscv_dsp_statistics_math.h"
#include "riscv_dsp_basic_math.h"
#include "riscv_dsp_complex_math.h"
#include "riscv_dsp_utils_math.h"
#include "riscv_dsp_matrix_math.h"

#include "internal_isa.h"

/* Constants for Q15 implementation */
#define LOG2TOLOG_Q15 0x02C5C860
#define MICRO_Q15 0x00000219
#define SHIFT_MELFILTER_SATURATION_Q15 10
#define ONE_Q15 0x7FFF

/**
 * @ingroup transform
 */

/**
 * @defgroup mfcc Functions
 *
 */

/**
 * @addtogroup mfcc
 * @{
 */

/**
 * @brief Function to implement the Q15 MFCC Functions
 * @param[in]       
 */
/* function description */
void riscv_dsp_mfcc_q15(const riscv_dsp_mfcc_q15_t * instance, q15_t *src, q15_t *dst, q31_t *buf)
{
    q15_t *coefs = instance->filter_coefs;
    q15_t maxValue;
    q31_t logExponent;
    uint32_t i, fftShift=0;
    q63_t result;

    /* Normalize */
    maxValue = riscv_dsp_absmax_val_q15(src, instance->fft_len);

    if ((maxValue != 0) && (maxValue != ONE_Q15))
    {
        q15_t quotient;
        int16_t shift;
        
        int status = riscv_dsp_divide_q15(ONE_Q15, maxValue, &quotient, &shift);
        if (status != 0)
        {
            return;
        }
        riscv_dsp_scale_q15(src, quotient, shift, src, instance->fft_len);
    }

    /* Multiply by window */
    riscv_dsp_mul_q15(src, instance->window_coefs, src, instance->fft_len);

    /* Compute spectrum magnitude */
    /* Default RFFT based implementation */
    riscv_dsp_rfft_q15(src, instance->log2_fft_len);
    src[instance->fft_len] = src[1];
    src[instance->fft_len + 1] = 0.0f;
    src[1] = 0.0f;

    riscv_dsp_shift_q15(src, 2, src, instance->fft_len);

    riscv_dsp_cmag_hp_q15(src, src, (instance->fft_len>>1) + 1);

    /* Apply MEL filters */
    for (i = 0; i < instance->n_mel_filters; i++)
    {
        result = riscv_dsp_dprod_q15(src + instance->filter_pos[i], coefs, instance->filter_len[i]);
        coefs += instance->filter_len[i];

        // q34.29 - fftShift
        result += MICRO_Q15;
        result >>= (SHIFT_MELFILTER_SATURATION_Q15);

        // q34.29 - fftShift - satShift
        buf[i] = NDS_ISA_SATS(result, 31);
    }

    if ((maxValue != 0) && (maxValue != ONE_Q15))
    {
        riscv_dsp_scale_q31(buf, maxValue<<16, 0, buf, instance->n_mel_filters);
    }

    /* Compute the log */
    for(uint32_t i=0;i<instance->n_mel_filters;i++)
    {
        buf[i] = riscv_dsp_log_q31(buf[i]);
    }

    // q5.26
    fftShift = instance->log2_fft_len - 1;
    logExponent = fftShift + 2 + SHIFT_MELFILTER_SATURATION_Q15;
    logExponent = logExponent * LOG2TOLOG_Q15;

    // q8.26
    riscv_dsp_offset_q31(buf, logExponent, buf, instance->n_mel_filters);
    riscv_dsp_shift_q31(buf, -19, buf, instance->n_mel_filters);

    for(i=0; i<instance->n_mel_filters; i++)
    {
        src[i] = NDS_ISA_SATS((q15_t)buf[i], 16);
    }

    // q8.7
    /* Multiply with the DCT matrix */
    riscv_dsp_mat_mul_mxv_q15(instance->dct_coefs, src, dst, instance->n_dct_out, instance->n_mel_filters);
}

/**
 * @} end of mfcc
 */
