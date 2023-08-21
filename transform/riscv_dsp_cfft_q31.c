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

/**
 * @ingroup transform
 */

/**
 * @addtogroup cfft
 * @{
 */

/**
 * @brief Function to implement the q31 Complex FFT Functions
 * @param[in]       *src     points to the input block data src[2 * k] and
 *                           src[2 * k + 1] contain the real and imaginary
 *                           parts of the k-th data.
 * @param[in]       m        If not defined STATIC_VERSION, the data buffer
 *                           is of size 2 * n while n = 2 ** m.
 * @return none
 */

extern int32_t riscv_dsp_cfft_rd2_q31(q31_t *src, uint32_t m);
extern int32_t riscv_dsp_cfft_rd4_q31(q31_t *src, uint32_t m);
extern int32_t riscv_dsp_cifft_rd2_q31(q31_t *src, uint32_t m);
extern int32_t riscv_dsp_cifft_rd4_q31(q31_t *src, uint32_t m);

void riscv_dsp_cfft_q31(q31_t *src, uint32_t m)
{
    switch (m)
    {
    case 4:
    case 6:
    case 8:
    case 10:
    case 12:
    case 14:
        riscv_dsp_cfft_rd4_q31(src, m);
        break;
    default :
        riscv_dsp_cfft_rd2_q31(src, m);
        break;
    }
}

/**
 * @brief Function to implement the q31 Complex IFFT Functions
 * @param[in]       *src     points to the input block data src[2 * k] and
 *                           src[2 * k + 1] contain the real and imaginary
 *                           parts of the k-th data.
 * @param[in]       m        If not defined STATIC_VERSION, the data buffer
 *                           is of size 2 * n while n = 2 ** m.
 * @return none
 */

void riscv_dsp_cifft_q31(q31_t *src, uint32_t m)
{

    switch (m)
    {
    case 4:
    case 6:
    case 8:
    case 10:
    case 12:
    case 14:
        riscv_dsp_cifft_rd4_q31(src, m);
        break;
    default :
        riscv_dsp_cifft_rd2_q31(src, m);
        break;
    }
}
/**
 * @} end of cfft
 */
