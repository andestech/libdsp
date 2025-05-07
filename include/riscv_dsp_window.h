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

#ifndef __RISCV_DSP_WINDOW_H__
#define __RISCV_DSP_WINDOW_H__

#ifdef  __cplusplus
extern "C"
{
#endif

#include "riscv_dsp_math_types.h"

/**
 * @defgroup window Window Functions
 * @brief Window Functions
 *
 * Window functions are classified into regular window functions and flat-top window functions.
 *
 * Regular window functions are used to detect tones or estimate the noise. They can be categorized into the following window types according to their peak sidelobe levels (PSL):
 *     - Bartlett window (26.5 dB)
 *     - Blackman Harris window (92 dB)
 *     - Hamming window (42.7 dB)
 *     - Hanning window (31.5 dB)
 *     - Nuttall3 window (46.7 dB)
 *     - Nuttall3a window (64.2 dB)
 *     - Nuttall3b window (71.5 dB)
 *     - Nuttall4 window (60.9 dB)
 *     - Nuttall4a window (82.6 dB)
 *     - Nuttall4b window (93.3 dB)
 *     - Nuttall4c window (98.1 dB)
 *     - Welch window (21.3 dB)
 *
 * Flat-top window functions are used to estimate the amplitude of a tone. They can be categorized into the following window types according to their PSL:
 *     - Hft90d window (90.2 dB)
 *     - Hft95 window (95.0 dB)
 *     - Hft116d window (116.8 dB)
 *     - Hft144d window (144.1 dB)
 *     - Hft169d window (169.5 dB)
 *     - Hft196d window (196.2 dB)
 *     - Hft223d window (223.0 dB)
 *     - Hft248d window (248.4 dB)
 *
 * These window types are referenced from Spectrum and spectral density estimation by the Discrete Fourier transform (DFT), including a comprehensive list of window functions and some new flat-top windows by G. Heinzel, A. Rudiger and R. Schilling, Max-Planck-Institut für Gravitationsphysik (Albert-Einstein-Institut), Teilinstitut Hannover.
 */

/**
 * @addtogroup window
 * @{
 */

/**
 * @defgroup win_welch Welch Window Functions
 *
 * These functions generate the Welch window with the following parameters:
 *  @par Parameters of the window
 *  | Parameter                             | Value              |
 *  | ------------------------------------: | -----------------: |
 *  | Peak sidelobe level                   |           21.3 dB  |
 *  | Normalized equivalent noise bandwidth |          1.2 bins  |
 *  | 3 dB bandwidth                        |       1.1535 bins  |
 *  | Flatness                              |        -2.2248 dB  |
 *  | Recommended overlap                   |            29.3 %  |
 *
 * Andes DSP library supports functions for the following data types: F32 and F64. These functions are described in the subsections below.
 */
/**
 * @addtogroup win_welch
 * @{
 */

/**
 * @brief Welch window function for F32
 * @param[out]   dst    Points to the output generated window
 * @param[in]    size   Number of samples in the window
 * @return none.
 */
void riscv_dsp_win_welch_f32(float32_t * dst, uint32_t size);

/**
 * @brief Welch window function for F64
 * @param[out]   dst    Points to the output generated window
 * @param[in]    size   Number of samples in the window
 * @return none.
 */
void riscv_dsp_win_welch_f64(float64_t * dst, uint32_t size);
/** @} win_welch */

/**
 * @defgroup win_bartlett Bartlett Window Functions
 *
 * These functions generate the Bartlett window with the following parameters:
 *  @par Parameters of the window
 *  | Parameter                             | Value              |
 *  | ------------------------------------: | -----------------: |
 *  | Peak sidelobe level                   |           26.5 dB  |
 *  | Normalized equivalent noise bandwidth |       1.3333 bins  |
 *  | 3 dB bandwidth                        |       1.2736 bins  |
 *  | Flatness                              |        -1.8242 dB  |
 *  | Recommended overlap                   |            50.0 %  |
 *
 * Andes DSP library supports functions for the following data types: F32 and F64. These functions are described in the subsections below.
 */
/**
 * @addtogroup win_bartlett
 * @{
 */

/**
 * @brief Bartlett window function for F32.
 * @param[out]   dst    Points to the output generated window
 * @param[in]    size   Number of samples in the window
 * @return none.
 */
void riscv_dsp_win_bartlett_f32(float32_t * dst, uint32_t size);

/**
 * @brief Bartlett window function for F64.
 * @param[out]   dst    Points to the output generated window
 * @param[in]    size   Number of samples in the window
 * @return none.
 */
void riscv_dsp_win_bartlett_f64(float64_t * dst, uint32_t size);
/** @} win_bartlett */

/**
 * @defgroup win_hamm Hamming Window Functions
 *
 * These functions generate the Hamming window with the following parameters:
 *  @par Parameters of the window
 *  | Parameter                             | Value              |
 *  | ------------------------------------: | -----------------: |
 *  | Peak sidelobe level                   |           42.7 dB  |
 *  | Normalized equivalent noise bandwidth |       1.3628 bins  |
 *  | 3 dB bandwidth                        |       1.3008 bins  |
 *  | Flatness                              |        -1.7514 dB  |
 *  | Recommended overlap                   |              50 %  |
 *
 * Andes DSP library supports functions for the following data types: F32 and F64. These functions are described in the subsections below.
 */
/**
 * @addtogroup win_hamm
 * @{
 */
/**
 * @brief Hamming window function for F32.
 * @param[out]   dst    Points to the output generated window
 * @param[in]    size   Number of samples in the window
 * @return none.
 */
void riscv_dsp_win_hamming_f32(float32_t * dst, uint32_t size);

/**
 * @brief Hamming window function for F64.
 * @param[out]   dst    Points to the output generated window
 * @param[in]    size   Number of samples in the window
 * @return none.
 */
void riscv_dsp_win_hamming_f64(float64_t * dst, uint32_t size);
/** @} win_hamm */

/**
 * @defgroup win_hann Hanning Window Functions
 *
 * These functions generate the Hanning window with the following parameters:
 *  @par Parameters of the window
 *  | Parameter                             | Value              |
 *  | ------------------------------------: | -----------------: |
 *  | Peak sidelobe level                   |           31.5 dB  |
 *  | Normalized equivalent noise bandwidth |          1.5 bins  |
 *  | 3 dB bandwidth                        |       1.4382 bins  |
 *  | Flatness                              |        -1.4236 dB  |
 *  | Recommended overlap                   |              50 %  |
 *
 * Andes DSP library supports functions for the following data types: F32 and F64. These functions are described in the subsections below.
 */
/**
 * @addtogroup win_hann
 * @{
 */
/**
 * @brief Hanning window function for F32.
 * @param[out]   dst    Points to the output generated window
 * @param[in]    size   Number of samples in the window
 * @return none.
 */
void riscv_dsp_win_hanning_f32(float32_t * dst, uint32_t size);

/**
 * @brief Hanning window function for F64.
 * @param[out]   dst    Points to the output generated window
 * @param[in]    size   Number of samples in the window
 * @return none.
 */
void riscv_dsp_win_hanning_f64(float64_t * dst, uint32_t size);
/** @} win_hann */

/**
 * @defgroup win_nuttall3 Nuttall3 Window Functions
 *
 * These functions generate the Nuttall3 window with the following parameters:
 *  @par Parameters of the window
 *  | Parameter                             | Value              |
 *  | ------------------------------------: | -----------------: |
 *  | Peak sidelobe level                   |           46.7 dB  |
 *  | Normalized equivalent noise bandwidth |       1.9444 bins  |
 *  | 3 dB bandwidth                        |       1.8496 bins  |
 *  | Flatness                              |        -0.8630 dB  |
 *  | Recommended overlap                   |            64.7 %  |
 *
 * Andes DSP library supports functions for the following data types: F32 and F64. These functions are described in the subsections below.
 */
/**
 * @addtogroup win_nuttall3
 * @{
 */
/**
 * @brief Nuttall3 window function for F32.
 * @param[out]   dst    Points to the output generated window
 * @param[in]    size   Number of samples in the window
 * @return none.
 */
void riscv_dsp_win_nuttall3_f32(float32_t * dst, uint32_t size);

/**
 * @brief Nuttall3 window function for F64.
 * @param[out]   dst    Points to the output generated window
 * @param[in]    size   Number of samples in the window
 * @return none.
 */
void riscv_dsp_win_nuttall3_f64(float64_t * dst, uint32_t size);
/** @} win_nuttall3 */

/**
 * @defgroup win_nuttall3a Nuttall3a Window Functions
 *
 * These functions generate the Nuttall3a window with the following parameters:
 *  @par Parameters of the window
 *  | Parameter                             | Value              |
 *  | ------------------------------------: | -----------------: |
 *  | Peak sidelobe level                   |           64.2 dB  |
 *  | Normalized equivalent noise bandwidth |       1.7721 bins  |
 *  | 3 dB bandwidth                        |       1.6828 bins  |
 *  | Flatness                              |        -1.0453 dB  |
 *  | Recommended overlap                   |            61.2 %  |
 *
 * Andes DSP library supports functions for the following data types: F32 and F64. These functions are described in the subsections below.
 */
/**
 * @addtogroup win_nuttall3a
 * @{
 */
/**
 * @brief Nuttall3a window function for F32.
 * @param[out]   dst    Points to the output generated window
 * @param[in]    size   Number of samples in the window
 * @return none.
 */
void riscv_dsp_win_nuttall3a_f32(float32_t * dst, uint32_t size);

/**
 * @brief Nuttall3a window function for F64.
 * @param[out]   dst    Points to the output generated window
 * @param[in]    size   Number of samples in the window
 * @return none.
 */
void riscv_dsp_win_nuttall3a_f64(float64_t * dst, uint32_t size);
/** @} win_nuttall3a */

/**
 * @defgroup win_nuttall3b Nuttall3b Window Functions
 *
 * These functions generate the Nuttall3b window with the following parameters:
 *  @par Parameters of the window
 *  | Parameter                             | Value              |
 *  | ------------------------------------: | -----------------: |
 *  | Peak sidelobe level                   |           71.5 dB  |
 *  | Normalized equivalent noise bandwidth |       1.7037 bins  |
 *  | 3 dB bandwidth                        |       1.6162 bins  |
 *  | Flatness                              |        -1.1352 dB  |
 *  | Recommended overlap                   |            59.8 %  |
 *
 * Andes DSP library supports functions for the following data types: F32 and F64. These functions are described in the subsections below.
 */
/**
 * @addtogroup win_nuttall3b
 * @{
 */
/**
 * @brief Nuttall3b window function for F32.
 * @param[out]   dst    Points to the output generated window
 * @param[in]    size   Number of samples in the window
 * @return none.
 */
void riscv_dsp_win_nuttall3b_f32(float32_t * dst, uint32_t size);

/**
 * @brief Nuttall3b window function for F64.
 * @param[out]   dst    Points to the output generated window
 * @param[in]    size   Number of samples in the window
 * @return none.
 */
void riscv_dsp_win_nuttall3b_f64(float64_t * dst, uint32_t size);
/** @} win_nuttall3b */

/**
 * @defgroup win_nuttall4 Nuttall4 Window Functions
 *
 * These functions generate the Nuttall4 window with the following parameters:
 *  @par Parameters of the window
 *  | Parameter                             | Value              |
 *  | ------------------------------------: | -----------------: |
 *  | Peak sidelobe level                   |           60.9 dB  |
 *  | Normalized equivalent noise bandwidth |         2.31 bins  |
 *  | 3 dB bandwidth                        |       2.1884 bins  |
 *  | Flatness                              |        -0.6184 dB  |
 *  | Recommended overlap                   |            70.5 %  |
 *
 * Andes DSP library supports functions for the following data types: F32 and F64. These functions are described in the subsections below.
 */
/**
 * @addtogroup win_nuttall4
 * @{
 */
/**
 * @brief Nuttall4 window function for F32.
 * @param[out]   dst    Points to the output generated window
 * @param[in]    size   Number of samples in the window
 * @return none.
 */
void riscv_dsp_win_nuttall4_f32(float32_t * dst, uint32_t size);

/**
 * @brief Nuttall4 window function for F64.
 * @param[out]   dst    Points to the output generated window
 * @param[in]    size   Number of samples in the window
 * @return none.
 */
void riscv_dsp_win_nuttall4_f64(float64_t * dst, uint32_t size);
/** @} win_nuttall4 */

/**
 * @defgroup win_nuttall4a Nuttall4a Window Functions
 *
 * These functions generate the Nuttall4a window with the following parameters:
 *  @par Parameters of the window
 *  | Parameter                             | Value              |
 *  | ------------------------------------: | -----------------: |
 *  | Peak sidelobe level                   |           82.6 dB  |
 *  | Normalized equivalent noise bandwidth |       2.1253 bins  |
 *  | 3 dB bandwidth                        |       2.0123 bins  |
 *  | Flatness                              |        -0.7321 dB  |
 *  | Recommended overlap                   |            68.0 %  |
 *
 * Andes DSP library supports functions for the following data types: F32 and F64. These functions are described in the subsections below.
 */
/**
 * @addtogroup win_nuttall4a
 * @{
 */
/**
 * @brief Nuttall4a window function for F32.
 * @param[out]   dst    Points to the output generated window
 * @param[in]    size   Number of samples in the window
 * @return none.
 */
void riscv_dsp_win_nuttall4a_f32(float32_t * dst, uint32_t size);

/**
 * @brief Nuttall4a window function for F64.
 * @param[out]   dst    Points to the output generated window
 * @param[in]    size   Number of samples in the window
 * @return none.
 */
void riscv_dsp_win_nuttall4a_f64(float64_t * dst, uint32_t size);
/** @} win_nuttall4a */

/**
 * @defgroup win_nuttall4b Nuttall4b Window Functions
 *
 * These functions generate the Nuttall4b window with the following parameters:
 *  @par Parameters of the window
 *  | Parameter                             | Value              |
 *  | ------------------------------------: | -----------------: |
 *  | Peak sidelobe level                   |           93.3 dB  |
 *  | Normalized equivalent noise bandwidth |       2.0212 bins  |
 *  | 3 dB bandwidth                        |       1.9122 bins  |
 *  | Flatness                              |        -0.8118 dB  |
 *  | Recommended overlap                   |            66.3 %  |
 *
 * Andes DSP library supports functions for the following data types: F32 and F64. These functions are described in the subsections below.
 */
/**
 * @addtogroup win_nuttall4b
 * @{
 */
/**
 * @brief Nuttall4b window function for F32.
 * @param[out]   dst    Points to the output generated window
 * @param[in]    size   Number of samples in the window
 * @return none.
 */
void riscv_dsp_win_nuttall4b_f32(float32_t * dst, uint32_t size);

/**
 * @brief Nuttall4b window function for F64.
 * @param[out]   dst    Points to the output generated window
 * @param[in]    size   Number of samples in the window
 * @return none.
 */
void riscv_dsp_win_nuttall4b_f64(float64_t * dst, uint32_t size);
/** @} win_nuttall4b */

/**
 * @defgroup win_nuttall4c Nuttall4c Window Functions
 *
 * These functions generate the Nuttall4c window with the following parameters:
 *  @par Parameters of the window
 *  | Parameter                             | Value              |
 *  | ------------------------------------: | -----------------: |
 *  | Peak sidelobe level                   |           98.1 dB  |
 *  | Normalized equivalent noise bandwidth |       1.9761 bins  |
 *  | 3 dB bandwidth                        |       1.8687 bins  |
 *  | Flatness                              |        -0.8506 dB  |
 *  | Recommended overlap                   |            65.6 %  |
 *
 * Andes DSP library supports functions for the following data types: F32 and F64. These functions are described in the subsections below.
 */
/**
 * @addtogroup win_nuttall4c
 * @{
 */
/**
 * @brief Nuttall4c window function for F32.
 * @param[out]   dst    Points to the output generated window
 * @param[in]    size   Number of samples in the window
 * @return none.
 */
void riscv_dsp_win_nuttall4c_f32(float32_t * dst, uint32_t size);

/**
 * @brief Nuttall4c window function for F64.
 * @param[out]   dst    Points to the output generated window
 * @param[in]    size   Number of samples in the window
 * @return none.
 */
void riscv_dsp_win_nuttall4c_f64(float64_t * dst, uint32_t size);
/** @} win_nuttall4c */

/**
 * @defgroup win_blackman Blackman Harris Window Functions
 *
 * These functions generate the Blackman Harris window with the following parameters:
 *  @par Parameters of the window
 *  | Parameter                             | Value              |
 *  | ------------------------------------: | -----------------: |
 *  | Peak sidelobe level                   |           92.0 dB  |
 *  | Normalized equivalent noise bandwidth |       2.0044 bins  |
 *  | 3 dB bandwidth                        |       1.8962 bins  |
 *  | Flatness                              |        -0.8256 dB  |
 *  | Recommended overlap                   |            66.1 %  |
 *
 * Andes DSP library supports functions for the following data types: F32 and F64. These functions are described in the subsections below.
 */
/**
 * @addtogroup win_blackman
 * @{
 */
/**
 * @brief Blackman Harris window function for F32.
 * @param[out]   dst    Points to the output generated window
 * @param[in]    size   Number of samples in the window
 * @return none.
 */
void riscv_dsp_win_blackman_harris_92db_f32(float32_t * dst, uint32_t size);

/**
 * @brief Blackman Harris window function for F64.
 * @param[out]   dst    Points to the output generated window
 * @param[in]    size   Number of samples in the window
 * @return none.
 */
void riscv_dsp_win_blackman_harris_92db_f64(float64_t * dst, uint32_t size);
/** @} win_blackman */

/**
 * @defgroup win_hft95 Hft95 Window Functions
 *
 * These functions generate the Hft95 window with the following parameters:
 *  @par Parameters of the window
 *  | Parameter                             | Value              |
 *  | ------------------------------------: | -----------------: |
 *  | Peak sidelobe level                   |           95.0 dB  |
 *  | Normalized equivalent noise bandwidth |       3.8112 bins  |
 *  | 3 dB bandwidth                        |       3.7590 bins  |
 *  | Flatness                              |         0.0044 dB  |
 *  | Recommended overlap                   |            75.6 %  |
 *
 * Andes DSP library supports functions for the following data types: F32 and F64. These functions are described in the subsections below.
 */
/**
 * @addtogroup win_hft95
 * @{
 */
/**
 * @brief Hft95 window function for F32.
 * @param[out]   dst    Points to the output generated window
 * @param[in]    size   Number of samples in the window
 * @return none.
 */
void riscv_dsp_win_hft95_f32(float32_t * dst, uint32_t size);

/**
 * @brief Hft95 window function for F64.
 * @param[out]   dst    Points to the output generated window
 * @param[in]    size   Number of samples in the window
 * @return none.
 */
void riscv_dsp_win_hft95_f64(float64_t * dst, uint32_t size);
/** @} win_hft95 */

/**
 * @defgroup win_hft90d Hft90d Window Functions
 *
 * These functions generate the Hft90d window with the following parameters:
 *  @par Parameters of the window
 *  | Parameter                             | Value              |
 *  | ------------------------------------: | -----------------: |
 *  | Peak sidelobe level                   |           90.2 dB  |
 *  | Normalized equivalent noise bandwidth |       3.8832 bins  |
 *  | 3 dB bandwidth                        |       3.8320 bins  |
 *  | Flatness                              |        -0.0039 dB  |
 *  | Recommended overlap                   |            76.0 %  |
 *
 * Andes DSP library supports functions for the following data types: F32 and F64. These functions are described in the subsections below.
 */
/**
 * @addtogroup win_hft90d
 * @{
 */
/**
 * @brief Hft90d window function for F32.
 * @param[out]   dst    Points to the output generated window
 * @param[in]    size   Number of samples in the window
 * @return none.
 */
void riscv_dsp_win_hft90d_f32(float32_t * dst, uint32_t size);

/**
 * @brief Hft90d window function for F64.
 * @param[out]   dst    Points to the output generated window
 * @param[in]    size   Number of samples in the window
 * @return none.
 */
void riscv_dsp_win_hft90d_f64(float64_t * dst, uint32_t size);
/** @} win_hft90d */

/**
 * @defgroup win_hft116d Hft116d Window Functions
 *
 * These functions generate the Hft116d window with the following parameters:
 *  @par Parameters of the window
 *  | Parameter                             | Value              |
 *  | ------------------------------------: | -----------------: |
 *  | Peak sidelobe level                   |          116.8 dB  |
 *  | Normalized equivalent noise bandwidth |       4.2186 bins  |
 *  | 3 dB bandwidth                        |       4.1579 bins  |
 *  | Flatness                              |        -0.0028 dB  |
 *  | Recommended overlap                   |            78.2 %  |
 *
 * Andes DSP library supports functions for the following data types: F32 and F64. These functions are described in the subsections below.
 */
/**
 * @addtogroup win_hft116d
 * @{
 */
/**
 * @brief Hft116d window function for F32.
 * @param[out]   dst    Points to the output generated window
 * @param[in]    size   Number of samples in the window
 * @return none.
 */
void riscv_dsp_win_hft116d_f32(float32_t * dst, uint32_t size);

/**
 * @brief Hft116d window function for F64.
 * @param[out]   dst    Points to the output generated window
 * @param[in]    size   Number of samples in the window
 * @return none.
 */
void riscv_dsp_win_hft116d_f64(float64_t * dst, uint32_t size);
/** @} win_hft116d */

/**
 * @defgroup win_hft144d Hft144d Window Functions
 *
 * These functions generate the Hft144d window with the following parameters:
 *  @par Parameters of the window
 *  | Parameter                             | Value              |
 *  | ------------------------------------: | -----------------: |
 *  | Peak sidelobe level                   |          144.1 dB  |
 *  | Normalized equivalent noise bandwidth |       4.5386 bins  |
 *  | 3 dB bandwidth                        |       4.4697 bins  |
 *  | Flatness                              |         0.0021 dB  |
 *  | Recommended overlap                   |            79.9 %  |
 *
 * Andes DSP library supports functions for the following data types: F32 and F64. These functions are described in the subsections below.
 */
/**
 * @addtogroup win_hft144d
 * @{
 */
/**
 * @brief Hft144d window function for F32.
 * @param[out]   dst    Points to the output generated window
 * @param[in]    size   Number of samples in the window
 * @return none.
 */
void riscv_dsp_win_hft144d_f32(float32_t * dst, uint32_t size);

/**
 * @brief Hft144d window function for F64.
 * @param[out]   dst    Points to the output generated window
 * @param[in]    size   Number of samples in the window
 * @return none.
 */
void riscv_dsp_win_hft144d_f64(float64_t * dst, uint32_t size);
/** @} win_hft144d */

/**
 * @defgroup win_hft169d Hft169d Window Functions
 *
 * These functions generate the Hft169d window with the following parameters:
 *  @par Parameters of the window
 *  | Parameter                             | Value              |
 *  | ------------------------------------: | -----------------: |
 *  | Peak sidelobe level                   |          169.5 dB  |
 *  | Normalized equivalent noise bandwidth |       4.8347 bins  |
 *  | 3 dB bandwidth                        |       4.7588 bins  |
 *  | Flatness                              |         0.0017 dB  |
 *  | Recommended overlap                   |            81.2 %  |
 *
 * Andes DSP library supports functions for the following data types: F32 and F64. These functions are described in the subsections below.
 */
/**
 * @addtogroup win_hft169d
 * @{
 */
/**
 * @brief Hft169d window function for F32.
 * @param[out]   dst    Points to the output generated window
 * @param[in]    size   Number of samples in the window
 * @return none.
 */
void riscv_dsp_win_hft169d_f32(float32_t * dst, uint32_t size);

/**
 * @brief Hft169d window function for F64.
 * @param[out]   dst    Points to the output generated window
 * @param[in]    size   Number of samples in the window
 * @return none.
 */
void riscv_dsp_win_hft169d_f64(float64_t * dst, uint32_t size);
/** @} win_hft169d */

/**
 * @defgroup win_hft196d Hft196d Window Functions
 *
 * These functions generate the Hft196d window with the following parameters:
 *  @par Parameters of the window
 *  | Parameter                             | Value              |
 *  | ------------------------------------: | -----------------: |
 *  | Peak sidelobe level                   |          196.2 dB  |
 *  | Normalized equivalent noise bandwidth |       5.1134 bins  |
 *  | 3 dB bandwidth                        |       5.0308 bins  |
 *  | Flatness                              |         0.0013 dB  |
 *  | Recommended overlap                   |            82.3 %  |
 *
 * Andes DSP library supports functions for the following data types: F32 and F64. These functions are described in the subsections below.
 */
/**
 * @addtogroup win_hft196d
 * @{
 */
/**
 * @brief Hft196d window function for F32.
 * @param[out]   dst    Points to the output generated window
 * @param[in]    size   Number of samples in the window
 * @return none.
 */
void riscv_dsp_win_hft196d_f32(float32_t * dst, uint32_t size);

/**
 * @brief Hft196d window function for F64.
 * @param[out]   dst    Points to the output generated window
 * @param[in]    size   Number of samples in the window
 * @return none.
 */
void riscv_dsp_win_hft196d_f64(float64_t * dst, uint32_t size);
/** @} win_hft196d */

/**
 * @defgroup win_hft223d Hft223d Window Functions
 *
 * These functions generate the Hft223d window with the following parameters:
 *  @par Parameters of the window
 *  | Parameter                             | Value              |
 *  | ------------------------------------: | -----------------: |
 *  | Peak sidelobe level                   |          223.0 dB  |
 *  | Normalized equivalent noise bandwidth |       5.3888 bins  |
 *  | 3 dB bandwidth                        |       5.3000 bins  |
 *  | Flatness                              |        -0.0011 dB  |
 *  | Recommended overlap                   |            83.3 %  |
 *
 * Andes DSP library supports functions for the following data types: F32 and F64. These functions are described in the subsections below.
 */
/**
 * @addtogroup win_hft223d
 * @{
 */
/**
 * @brief Hft223d window function for F32.
 * @param[out]   dst    Points to the output generated window
 * @param[in]    size   Number of samples in the window
 * @return none.
 */
void riscv_dsp_win_hft223d_f32(float32_t * dst, uint32_t size);

/**
 * @brief Hft223d window function for F64.
 * @param[out]   dst    Points to the output generated window
 * @param[in]    size   Number of samples in the window
 * @return none.
 */
void riscv_dsp_win_hft223d_f64(float64_t * dst, uint32_t size);
/** @} win_hft223d */

/**
 * @defgroup win_hft248d Hft248d Window Functions
 *
 * These functions generate the Hft248d window with the following parameters:
 * @par Parameters of the window
 * | Parameter                             | Value              |
 * | ------------------------------------: | -----------------: |
 * | Peak sidelobe level                   |          248.4 dB  |
 * | Normalized equivalent noise bandwidth |       5.6512 bins  |
 * | 3 dB bandwidth                        |       5.5567 bins  |
 * | Flatness                              |         0.0009 dB  |
 * | Recommended overlap                   |            84.1 %  |
 *
 * Andes DSP library supports functions for the following data types: F32 and F64. These functions are described in the subsections below.
 */
/**
 * @addtogroup win_hft248d
 * @{
 */
/**
 * @brief Hft248d window function for F32.
 * @param[out]   dst    Points to the output generated window
 * @param[in]    size   Number of samples in the window
 * @return none.
 */
void riscv_dsp_win_hft248d_f32(float32_t * dst, uint32_t size);

/**
 * @brief Hft248d window function for F64.
 * @param[out]   dst    Points to the output generated window
 * @param[in]    size   Number of samples in the window
 * @return none.
 */
void riscv_dsp_win_hft248d_f64(float64_t * dst, uint32_t size);

/** @} win_hft248d */

/** @} window */

#ifdef __cplusplus
}
#endif

#endif // __RISCV_DSP_WINDOW_H__
