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

/* Constants for Q31 implementation */
#define LOG2TOLOG_Q31 0x02C5C860
#define MICRO_Q31 0x08637BD0
#define SHIFT_MELFILTER_SATURATION_Q31 10
#define ONE_Q31 0x7FFFFFFFL

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
 * @brief Function to implement the Q31 MFCC Functions
 * @param[in]       
 */
/* function description */
void riscv_dsp_mfcc_q31(const riscv_dsp_mfcc_q31_t * instance, q31_t *src, q31_t *dst, q31_t *buf)
{
    
    q31_t *coefs = instance->filter_coefs;
    q31_t maxValue, logExponent;
    uint32_t i, fftShift=0;
    q63_t result;

    /* Normalize */
    maxValue = riscv_dsp_absmax_val_q31(src, instance->fft_len);

    if ((maxValue != 0) && (maxValue != ONE_Q31))
    {
        q31_t quotient;
        int16_t shift;
        
        int status = riscv_dsp_divide_q31(ONE_Q31, maxValue, &quotient, &shift);
        if (status != 0)
        {
            return;
        }
        riscv_dsp_scale_q31(src, quotient, shift, src, instance->fft_len);
    }

    /* Multiply by window */
    riscv_dsp_mul_q31(src, instance->window_coefs, src, instance->fft_len);

    /* Compute spectrum magnitude */
    /* Default RFFT based implementation */
    riscv_dsp_rfft_q31(src, instance->log2_fft_len);
    riscv_dsp_shift_q31(src, 2, src, instance->fft_len);

    riscv_dsp_cmag_q31(src, src, (instance->fft_len>>1) + 1);

    /* Apply MEL filters */
    for (i = 0; i < instance->n_mel_filters; i++)
    {
        result = riscv_dsp_dprod_q31(src + instance->filter_pos[i], coefs, instance->filter_len[i]);
        coefs += instance->filter_len[i];

        // q16.48 - fftShift
        result += MICRO_Q31;
        result >>= (SHIFT_MELFILTER_SATURATION_Q31 + 18);

        // q16.29 - fftShift - satShift
        buf[i] = NDS_ISA_SATS(result, 31);
    }

    if ((maxValue != 0) && (maxValue != ONE_Q31))
    {
        riscv_dsp_scale_q31(buf, maxValue, 0, buf, instance->n_mel_filters);
    }

    /* Compute the log */
    for(uint32_t i=0;i<instance->n_mel_filters;i++)
    {
        buf[i] = riscv_dsp_log_q31(buf[i]);
    }

    // q5.26
    fftShift = instance->log2_fft_len - 1;
    logExponent = fftShift + 2 + SHIFT_MELFILTER_SATURATION_Q31;
    logExponent = logExponent * LOG2TOLOG_Q31;

    // q5.26
    riscv_dsp_offset_q31(buf, logExponent, buf, instance->n_mel_filters);
    riscv_dsp_shift_q31(buf, -3, buf, instance->n_mel_filters);
    
    // q8.23
    /* Multiply with the DCT matrix */
    riscv_dsp_mat_mul_mxv_q31(instance->dct_coefs, buf, dst, instance->n_dct_out, instance->n_mel_filters);
}

/**
 * @} end of mfcc
 */
