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
 * @brief Function to implement the half floating-point MFCC Functions
 * @param[in]       
 */

/* function description */
void riscv_dsp_mfcc_f16(const riscv_dsp_mfcc_f16_t * instance, float16_t *src, float16_t *dst, float16_t *buf)
{
    float16_t maxValue, result;
    uint32_t i;
    float16_t *coefs = instance->filter_coefs;

    /* Normalize */
    maxValue = riscv_dsp_absmax_val_f16(src, instance->fft_len);

    if (maxValue != 0.0f)
    {
        riscv_dsp_scale_f16(src, 1.0f / maxValue, src, instance->fft_len);
    }

    /* Multiply by window */
    riscv_dsp_mul_f16(src, instance->window_coefs, src, instance->fft_len);

    /* Compute spectrum magnitude */
    /* Default RFFT based implementation */

    riscv_dsp_rfft_f16(src, instance->log2_fft_len);

    riscv_dsp_cmag_f16(src, src, (instance->fft_len>>1) + 1);

    if (maxValue != 0.0f)
    {
        riscv_dsp_scale_f16(src, maxValue, src, (instance->fft_len>>1) + 1);
    }

    /* Apply MEL filters */
    for (i = 0; i < instance->n_mel_filters; i++)
    {
        result = riscv_dsp_dprod_f16(src + instance->filter_pos[i], coefs, instance->filter_len[i]);

        coefs += instance->filter_len[i];
        buf[i] = result;
    }

    /* Compute the log */
    riscv_dsp_offset_f16(buf, 1.0e-4f, buf, instance->n_mel_filters);

    // riscv_dsp_vlog_f16(buf, buf, instance->n_mel_filters);
    for(uint32_t i=0;i<instance->n_mel_filters;i++)
    {
        buf[i] = riscv_dsp_log_f16(buf[i]);
    }

    /* Multiply with the DCT matrix */
    riscv_dsp_mat_mul_mxv_f16(instance->dct_coefs, buf, dst, instance->n_dct_out, instance->n_mel_filters);
}

/**
 * @} end of mfcc
 */
