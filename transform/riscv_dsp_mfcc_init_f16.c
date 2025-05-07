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
#include "internal_utils_math.h"

/**
  @ingroup transform
 */

/**
  @addtogroup mfcc
  @{
 */

/**
 * @param[in,out]  instance     
 */

void riscv_dsp_mfcc_init_f16(riscv_dsp_mfcc_f16_t * instance,
                            uint32_t fft_len,
                            uint32_t n_mel_filters,
                            uint32_t n_dct_out,
                            float16_t *dct_coefs,
                            uint32_t *filter_pos,
                            uint32_t *filter_len,
                            float16_t *filter_coefs,
                            float16_t *window_coefs)
{
    instance->fft_len = fft_len;
    instance->n_mel_filters = n_mel_filters;
    instance->n_dct_out = n_dct_out;
    instance->dct_coefs = dct_coefs;
    instance->filter_pos = filter_pos;
    instance->filter_len = filter_len;
    instance->filter_coefs = filter_coefs;
    instance->window_coefs = window_coefs;
    instance->log2_fft_len = dsp_log2_32b(fft_len);
}

/**
  @} end of Transform group
 */
