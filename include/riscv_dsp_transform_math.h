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
#ifndef __RISCV_DSP_TRANSFORM_MATH_H__
#define __RISCV_DSP_TRANSFORM_MATH_H__

#ifdef  __cplusplus
extern "C"
{
#endif

#include "riscv_dsp_math_types.h"

/**
 * @defgroup trans Transform Functions
 * @brief Transform Functions
 *
 * In Andes DSP library, the resolution is set as 1024 (the logarithm of 1024 to base 2 is 10) for
 * default configuration. This default value affects the resolution of cosine and sine lookup tables
 * (i.e. the number of elements in tables). It also affects transform functions since they use these
 * tables to look up cosine or sine values. Thus, some suggested values for the input vector size are
 * provided for transform functions in this chapter. Please find them from the function parameter m
 * or the compilation option FFT_LOGN in examples. Using input vector size values that are not
 * suggested, though still workable, may be at the risk of worse accuracy.
 *
 * Note that overflow may occur in Q31 and Q15 transform functions. To avoid the problem, you
 * may need to perform an arithmetic right shift operation before calling these functions. The shift
 * amount is different according to the input format, which may vary depending on the input size.
 * For details about input formats, please refer to the note of each Q31 and Q15 transform function
 * in subsections of this chapter.
 */
/**
 * @addtogroup trans
 * @{
 */

// CFFT/CIFFT
/// single-precision floating-point Complex FFT/IFFT Function
/**
 * @defgroup cfft Complex FFT Functions
 * @brief Complex FFT Functions
 *
 * Functions in this section internally call radix-2 or radix-4 CFFT/CIFFT functions
 * depending on the parameter m, which represents the base 2 logarithm
 * value of the input sample number. If the value of m is a multiple of two such as 4 or 6, the radix-4
 * functions are used to provide a faster performance. Otherwise, radix-2 functions are used.
 *
 * Andes DSP library supports distinct CFFT and CIFFT functions for the following data types:
 * single-precision floating-point, Q31 and Q15. These functions are introduced in the subsections below. For Q31
 * and Q15 Radix-4 CFFT and CIFFT functions, you may need to do an arithmetic right shift before
 * calling them.
 */
/**
 * @addtogroup cfft
 * @{
 */
/**
 * @brief cfft of f32 vectors.
 * @param[in, out] src   pointer of the input vector. After the function is executed, the
 *                       output will be stored in the input vector.
 * @param[in]        m   base 2 logarithm value of the sample number and it can be set from 3 to 10
 *
 * @b Example
 *     <pre>
 * Given 128 samples (that is, FFT_LOGN = 7), the example of single-precision floating-point CFFT and
 * CIFFT is as follows:
 *      \#define FFT_LOGN 7
 *      float32_t src[2* (1 << FFT_LOGN)] = {…};
 *      int32_t ret;
 *      riscv_dsp_cfft_f32(src, FFT_LOGN);
 *      riscv_dsp_cifft_f32(src, FFT_LOGN);
 *
 * This example also serves as a reference for examples of F16, F64, Q31 and Q15 CFFT and
 * CIFFT functions.
 *     </pre>
 */
void riscv_dsp_cfft_f32(float32_t *src, uint32_t m);

/**
 * @brief cfft of f64 vectors.
 * @param[in, out] src   pointer of the input vector. After the function is executed, the
 *                       output will be stored in the input vector.
 * @param[in]        m   base 2 logarithm value of the sample number and it can be set from 3 to 10
 */
void riscv_dsp_cfft_f64(float64_t *src, uint32_t m);

/**
 * @brief cifft of f32 vectors.
 * @param[in, out] src   pointer of the input vector. After the function is executed, the
 *                       output will be stored in the input vector.
 * @param[in]        m   base 2 logarithm value of the sample number and it can be set from 3 to 10
 */
void riscv_dsp_cifft_f32(float32_t *src, uint32_t m);

/**
 * @brief cifft of f64 vectors.
 * @param[in, out] src   pointer of the input vector. After the function is executed, the
 *                       output will be stored in the input vector.
 * @param[in]        m   base 2 logarithm value of the sample number and it can be set from 3 to 10
 */
void riscv_dsp_cifft_f64(float64_t *src, uint32_t m);

#if defined (__riscv_zfh)
/**
 * @brief cfft of f16 vectors.
 * @param[in, out] src   pointer of the input vector. After the function is executed, the
 *                       output will be stored in the input vector.
 * @param[in]        m   base 2 logarithm value of the sample number and it can be set from 3 to 10
 */
void riscv_dsp_cfft_f16(float16_t *src, uint32_t m);

/**
 * @brief cifft of f16 vectors.
 * @param[in, out] src   pointer of the input vector. After the function is executed, the
 *                       output will be stored in the input vector.
 * @param[in]        m   base 2 logarithm value of the sample number and it can be set from 3 to 10
 */
void riscv_dsp_cifft_f16(float16_t *src, uint32_t m);
#endif

/// Q15 Complex FFT/IFFT Function
/**
 * @brief cfft of q15 vectors.
 * @param[in, out] src   pointer of the input vector. After the function is executed, the
 *                       output will be stored in the input vector.
 * @param[in]        m   base 2 logarithm value of the sample number and it can be set from 3 to 10
 *
 * @b Note:
 *
 * The input and output formats are listed below. To satisfy the input format corresponding to
 * your input size, you may need to perform an arithmetic shift operation before calling this
 * function.
 *
 * @image html cfft_rd2.gif ""
 */
void riscv_dsp_cfft_q15(q15_t *src, uint32_t m);
/**
 * @brief cifft of q15 vectors.
 * @param[in, out] src   pointer of the input vector. After the function is executed, the
 *                       output will be stored in the input vector.
 * @param[in]        m   base 2 logarithm value of the sample number and it can be set from 3 to 10
 *
 * @b Note:
 *
 * The input and output formats are listed below. To satisfy the input format corresponding to
 * your input size, you may need to perform an arithmetic shift operation before calling this
 * function.
 *
 * @image html cifft_rd2.gif ""
 */
void riscv_dsp_cifft_q15(q15_t *src, uint32_t m);

/// Q31 Complex FFT/IFFT Function
/**
 * @brief cfft of q31 vectors.
 * @param[in, out] src   pointer of the input vector. After the function is executed, the
 *                       output will be stored in the input vector.
 * @param[in]        m   base 2 logarithm value of the sample number and it can be set from 3 to 10
 *
 * @b Note:
 *
 * The input and output formats are listed below. To satisfy the input format corresponding to
 * your input size, you may need to perform an arithmetic shift operation before calling this
 * function.
 *
 * @image html cfft_rd2_q31.gif ""
 */
void riscv_dsp_cfft_q31(q31_t *src, uint32_t m);
/**
 * @brief cifft of q31 vectors.
 * @param[in, out] src   pointer of the input vector. After the function is executed, the
 *                       output will be stored in the input vector.
 * @param[in]        m   base 2 logarithm value of the sample number and it can be set from 3 to 10
 *
 * @b Note:
 *
 * The input and output formats are listed below. To satisfy the input format corresponding to
 * your input size, you may need to perform an arithmetic shift operation before calling this
 * function.
 *
 * @image html cfft_rd2_q31.gif ""
 */
void riscv_dsp_cifft_q31(q31_t *src, uint32_t m);

/** Backward compatibility of cfft_rd2  */
#define riscv_dsp_cfft_rd2_f32  riscv_dsp_cfft_f32
/** Backward compatibility of cfft_rd2  */
#define riscv_dsp_cfft_rd2_f64  riscv_dsp_cfft_f64
/** Backward compatibility of cfft_rd2  */
#define riscv_dsp_cfft_rd2_q15  riscv_dsp_cfft_q15
/** Backward compatibility of cfft_rd2  */
#define riscv_dsp_cfft_rd2_q31  riscv_dsp_cfft_q31

/** Backward compatibility of cifft_rd2  */
#define riscv_dsp_cifft_rd2_f32  riscv_dsp_cifft_f32
/** Backward compatibility of cifft_rd2  */
#define riscv_dsp_cifft_rd2_f64  riscv_dsp_cifft_f64
/** Backward compatibility of cifft_rd2  */
#define riscv_dsp_cifft_rd2_q15  riscv_dsp_cifft_q15
/** Backward compatibility of cifft_rd2  */
#define riscv_dsp_cifft_rd2_q31  riscv_dsp_cifft_q31

/** Backward compatibility of cfft_rd4  */
#define riscv_dsp_cfft_rd4_f32  riscv_dsp_cfft_f32
/** Backward compatibility of cfft_rd4  */
#define riscv_dsp_cfft_rd4_f64  riscv_dsp_cfft_f64
/** Backward compatibility of cfft_rd4  */
#define riscv_dsp_cfft_rd4_q15  riscv_dsp_cfft_q15
/** Backward compatibility of cfft_rd4  */
#define riscv_dsp_cfft_rd4_q31  riscv_dsp_cfft_q31

/** Backward compatibility of cifft_rd4  */
#define riscv_dsp_cifft_rd4_f32  riscv_dsp_cifft_f32
/** Backward compatibility of cifft_rd4  */
#define riscv_dsp_cifft_rd4_f64  riscv_dsp_cifft_f64
/** Backward compatibility of cifft_rd4  */
#define riscv_dsp_cifft_rd4_q15  riscv_dsp_cifft_q15
/** Backward compatibility of cifft_rd4  */
#define riscv_dsp_cifft_rd4_q31  riscv_dsp_cifft_q31

/** @} cfft */

// Real-FFT
/// single-precision floating-point Real FFT/IFFT Function
/**
 * @defgroup rfft Real FFT Functions
 * @brief Real FFT Functions
 *
 * Real fast Fourier transform (RFFT) and inverse fast Fourier transform (RIFFT) functions
 * transform signals composed of real values from time domain to frequency domain.
 *
 * The RFFT algorithm treats the N real data in the input as N/2 complex and performs a CFFT.
 * After the CFFT is computed, the real value at N/2 of the frequency spectrum is stored at the first
 * imaginary position. The output layout of the RFFT is thus arranged as [r[0], r[N/2], r[1],
 * i[1], ..., r[N/2 - 1], i[N/2 - 1]] where r is real, i is imaginary and N is the size of the input
 * data. As no additional array is created during the transformation, the size of the output is
 * identical to that of the input.
 *
 * As to the RIFFT, its input data has the same format as the output of the RFFT and its output
 * data after computation is converted from the frequency spectrum to the time domain.
 *
 * Andes DSP library supports distinct RFFT and RIFFT functions for the following data types:
 * single-precision floating-point, Q31 and Q15. These functions are introduced in the subsections below. For Q31
 * and Q15 RFFT and RIFFT functions, you may need to do an arithmetic right shift before calling
 * them.
 */
/**
 * @addtogroup rfft
 * @{
 */
/**
 * @brief rfft of f32 vectors.
 * @param[in, out] src   pointer of the input vector. After the function is executed, the
 *                       output will be stored in the input vector.
 * @param[in]        m   base 2 logarithm value of the sample number and it can be set from 4 to 10
 * @return 0 success; -1 failure
 *
 * @b Example
 *     <pre>
 * Given 128 samples (that is, FFT_LOGN = 7), the example of single-precision floating-point RFFT and RIFFT
 * is as follows:
 *      \#define FFT_LOGN 7
 *      float32_t src[(1 << FFT_LOGN)] = {…};
 *      int32_t ret;
 *      ret = riscv_dsp_rfft_f32(src, FFT_LOGN);
 *      if (ret == 0)
 *          Success
 *      else
 *          Fail
 *      ret = riscv_dsp_rifft_f32(src, FFT_LOGN);
 *      if (ret == 0)
 *          Success
 *      else
 *          Fail
 *
 * This example also serves as a reference for examples of Q31 or Q15 RFFT and RIFFT
 * functions.
 *     </pre>
 */
int32_t riscv_dsp_rfft_f32(float32_t *src, uint32_t m);
/**
 * @brief rfft of f64 vectors.
 * @param[in, out] src   pointer of the input vector. After the function is executed, the
 *                       output will be stored in the input vector.
 * @param[in]        m   base 2 logarithm value of the sample number and it can be set from 4 to 10
 * @return 0 success; -1 failure
 */
int32_t riscv_dsp_rfft_f64(float64_t *src, uint32_t m);
/**
 * @brief rifft of f32 vectors.
 * @param[in, out] src   pointer of the input vector. After the function is executed, the
 *                       output will be stored in the input vector.
 * @param[in]        m   base 2 logarithm value of the sample number and it can be set from 4 to 10
 * @return 0 success; -1 failure
 */
int32_t riscv_dsp_rifft_f32(float32_t *src, uint32_t m);

/**
 * @brief rifft of f64 vectors.
 * @param[in, out] src   pointer of the input vector. After the function is executed, the
 *                       output will be stored in the input vector.
 * @param[in]        m   base 2 logarithm value of the sample number and it can be set from 4 to 10
 * @return 0 success; -1 failure
 */
int32_t riscv_dsp_rifft_f64(float64_t *src, uint32_t m);

#if defined (__riscv_zfh)
/**
 * @brief rfft of f16 vectors.
 * @param[in, out] src   pointer of the input vector. After the function is executed, the
 *                       output will be stored in the input vector.
 * @param[in]        m   base 2 logarithm value of the sample number and it can be set from 4 to 10
 * @return 0 success; -1 failure
 */
int32_t riscv_dsp_rfft_f16(float16_t *src, uint32_t m);

/**
 * @brief rifft of f16 vectors.
 * @param[in, out] src   pointer of the input vector. After the function is executed, the
 *                       output will be stored in the input vector.
 * @param[in]        m   base 2 logarithm value of the sample number and it can be set from 4 to 10
 * @return 0 success; -1 failure
 */
int32_t riscv_dsp_rifft_f16(float16_t *src, uint32_t m);
#endif

/// Q15 Real FFT/IFFT Function
/**
 * @brief rfft of q15 vectors.
 * @param[in, out] src   pointer of the input vector. After the function is executed, the
 *                       output will be stored in the input vector.
 * @param[in]        m   base 2 logarithm value of the sample number and it can be set from 4 to 10
 * @return 0 success; -1 failure
 *
 * @b Note:
 *
 * The input and output formats are listed below. To satisfy the input format corresponding to
 * your input size, you may need to perform an arithmetic shift operation before calling this
 * function.
 *
 * @image html rfft_q15.gif ""
 */
int32_t riscv_dsp_rfft_q15(q15_t *src, uint32_t m);
/**
 * @brief rifft of q15 vectors.
 * @param[in, out] src   pointer of the input vector. After the function is executed, the
 *                       output will be stored in the input vector.
 * @param[in]        m   base 2 logarithm value of the sample number and it can be set from 4 to 10
 * @return 0 success; -1 failure
 *
 * @b Note:
 *
 * The input and output formats are listed below. To satisfy the input format corresponding to
 * your input size, you may need to perform an arithmetic shift operation before calling this
 * function.
 *
 * @image html rifft_q15.gif ""
 */
int32_t riscv_dsp_rifft_q15(q15_t *src, uint32_t m);

/// Q31 Real FFT/IFFT Function
/**
 * @brief rfft of q31 vectors.
 * @param[in, out] src   pointer of the input vector. After the function is executed, the
 *                       output will be stored in the input vector.
 * @param[in]        m   base 2 logarithm value of the sample number and it can be set from 4 to 10
 * @return 0 success; -1 failure
 *
 * @b Note:
 *
 * The input and output formats are listed below. To satisfy the input format corresponding to
 * your input size, you may need to perform an arithmetic shift operation before calling this
 * function.
 *
 * @image html rfft_q31.gif ""
 */
int32_t riscv_dsp_rfft_q31(q31_t *src, uint32_t m);
/**
 * @brief rifft of q31 vectors.
 * @param[in, out] src   pointer of the input vector. After the function is executed, the
 *                       output will be stored in the input vector.
 * @param[in]        m   base 2 logarithm value of the sample number and it can be set from 4 to 10
 * @return 0 success; -1 failure
 *
 * @b Note:
 *
 * The input and output formats are listed below. To satisfy the input format corresponding to
 * your input size, you may need to perform an arithmetic shift operation before calling this
 * function.
 *
 * @image html rifft_q31.gif ""
 */
int32_t riscv_dsp_rifft_q31(q31_t *src, uint32_t m);
/** @} rfft */

// DCT/IDCT Type II
/// single-precision floating-point DCT/IDCT Type II Function
/**
 * @defgroup dct DCT Type II Functions
 * @brief DCT Type II Functions
 *
 * Discrete cosine transform (DCT) type II functions implement DCT with the following equation:
 *
 * @image html dct.gif ""
 *
 * Andes DSP library supports distinct DCT type II and IDCT functions for the following data types:
 * single-precision floating-point, Q31 and Q15. These functions are introduced in the subsections below. For Q31
 * and Q15 DCT type II and IDCT functions, you may need to do an arithmetic right shift before
 * calling them.
 */
/**
 * @addtogroup dct
 * @{
 */
/**
 * @param[in, out] src   pointer of the input vector. After the function is executed, the
 *                       output will be stored in the input vector.
 * @param[in]        m   base 2 logarithm value of the sample number and it can be set from 3 to 8
 * @return none
 *
 * @b Example
 *  <pre>
 * Given 256 samples (that is, FFT_LOGN = 8), the example of single-precision floating-point (DCT) type II and
 * IDCT is as follows:
 *      \#define FFT_LOGN 8
 *      float32_t src[(1 << FFT_LOGN)] = {…};
 *      riscv_dsp_dct_f32(src, FFT_LOGN);
 *      riscv_dsp_idct_f32(src, FFT_LOGN);
 * This example also serves as a reference for examples of Q31 or Q15 DCT type II and IDCT
 * functions.
 *  </pre>
 */
void riscv_dsp_dct_f32(float32_t *src, uint32_t m);
/**
 * @param[in, out] src   pointer of the input vector. After the function is executed, the
 *                       output will be stored in the input vector.
 * @param[in]        m   base 2 logarithm value of the sample number and it can be set from 3 to 8
 * @return none
 */
void riscv_dsp_idct_f32(float32_t *src, uint32_t m);

/// Q15 DCT/IDCT Type II Function
/**
 * @param[in, out] src   pointer of the input vector. After the function is executed, the
 *                       output will be stored in the input vector.
 * @param[in]        m   base 2 logarithm value of the sample number and it can be set from 3 to 8
 * @return none
 *
 * @b Note:
 *
 * The input and output formats are listed below. To satisfy the input format corresponding to
 * your input size, you may need to perform an arithmetic shift operation before calling this
 * function.
 *
 * @image html dct_q15.gif ""
 */
void riscv_dsp_dct_q15(q15_t *src, uint32_t m);
/**
 * @param[in, out] src   pointer of the input vector. After the function is executed, the
 *                       output will be stored in the input vector.
 * @param[in]        m   base 2 logarithm value of the sample number and it can be set from 3 to 8
 * @return none
 *
 * @b Note:
 *
 * The input and output formats are listed below. To satisfy the input format corresponding to
 * your input size, you may need to perform an arithmetic shift operation before calling this
 * function.
 *
 * @image html idct_q15.gif ""
 */
void riscv_dsp_idct_q15(q15_t *src, uint32_t m);

/// Q31 DCT/IDCT Type II Function
/**
 * @param[in, out] src   pointer of the input vector. After the function is executed, the
 *                       output will be stored in the input vector.
 * @param[in]        m   base 2 logarithm value of the sample number and it can be set from 3 to 8
 * @return none
 *
 * @b Note:
 *
 * The input and output formats are listed below. To satisfy the input format corresponding to
 * your input size, you may need to perform an arithmetic shift operation before calling this
 * function.
 *
 * @image html dct_q31.gif ""
 */
void riscv_dsp_dct_q31(q31_t *src, uint32_t m);
/**
 * @param[in, out] src   pointer of the input vector. After the function is executed, the
 *                       output will be stored in the input vector.
 * @param[in]        m   base 2 logarithm value of the sample number and it can be set from 3 to 8
 * @return none
 *
 * @b Note:
 *
 * The input and output formats are listed below. To satisfy the input format corresponding to
 * your input size, you may need to perform an arithmetic shift operation before calling this
 * function.
 *
 * @image html idct_q31.gif ""
 */
void riscv_dsp_idct_q31(q31_t *src, uint32_t m);
/** @} dct */

// DCT Type IV
/// single-precision floating-point DCT/IDCT Type IV Function
/**
 * @defgroup dct4 DCT Type IV Functions
 * @brief DCT Type IV Functions
 *
 * Discrete cosine transform (DCT) type IV functions implement DCT transform with the following
 * equation:
 *
 * @image html dct4.gif ""
 *
 * Andes DSP library supports distinct DCT and IDCT type IV functions for the following data types:
 * single-precision floating-point, Q31 and Q15. These functions are introduced in the subsections below. For Q31
 * and Q15 DCT and IDCT type IV functions, you may need to do an arithmetic right shift before
 * calling them.
 */
/**
 * @addtogroup dct4
 * @{
 */
/**
 * @param[in, out] src   pointer of the input vector. After the function is executed, the
 *                       output will be stored in the input vector.
 * @param[in]        m   base 2 logarithm value of the sample number and it can be set from 3 to 7
 * @return none
 *
 * @b Example
 *  <pre>
 * Given 128 samples (that is, FFT_LOGN = 7), the example of single-precision floating-point DCT or IDCT type
 * IV transform is as follows:
 *      \#define FFT_LOGN 7
 *      float32_t src[(1 << FFT_LOGN)] = {…};
 *      riscv_dsp_dct4_f32(src, FFT_LOGN);
 *      riscv_dsp_idct4_f32(src, FFT_LOGN);
 * This example also serves as a reference for examples of Q31 or Q15 DCT type IV and IDCT
 * functions.
 *  </pre>
 */
void riscv_dsp_dct4_f32(float32_t *src, uint32_t m);
/**
 * @param[in, out] src   pointer of the input vector. After the function is executed, the
 *                       output will be stored in the input vector.
 * @param[in]        m   base 2 logarithm value of the sample number and it can be set from 3 to 7
 * @return none
 */
void riscv_dsp_idct4_f32(float32_t *src, uint32_t m);

/// Q15 DCT/IDCT Type IV Function
/**
 * @param[in, out] src   pointer of the input vector. After the function is executed, the
 *                       output will be stored in the input vector.
 * @param[in]        m   base 2 logarithm value of the sample number and it can be set from 3 to 7
 * @return none
 *
 * @b Note:
 *
 * The input and output formats are listed below. To satisfy the input format corresponding to
 * your input size, you may need to perform an arithmetic shift operation before calling this
 * function.
 *
 * @image html dct4_q15.gif ""
 */
void riscv_dsp_dct4_q15(q15_t *src, uint32_t m);
/**
 * @param[in, out] src   pointer of the input vector. After the function is executed, the
 *                       output will be stored in the input vector.
 * @param[in]        m   base 2 logarithm value of the sample number and it can be set from 3 to 7
 * @return none
 *
 * @b Note:
 *
 * The input and output formats are listed below. To satisfy the input format corresponding to
 * your input size, you may need to perform an arithmetic shift operation before calling this
 * function.
 *
 * @image html idct4_q15.gif ""
 */
void riscv_dsp_idct4_q15(q15_t *src, uint32_t m);

/// Q31 DCT/IDCT Type IV Function
/**
 * @param[in, out] src   pointer of the input vector. After the function is executed, the
 *                       output will be stored in the input vector.
 * @param[in]        m   base 2 logarithm value of the sample number and it can be set from 3 to 7
 * @return none
 *
 * @b Note:
 *
 * The input and output formats are listed below. To satisfy the input format corresponding to
 * your input size, you may need to perform an arithmetic shift operation before calling this
 * function.
 *
 * @image html dct4_q31.gif ""
 */
void riscv_dsp_dct4_q31(q31_t *src, uint32_t m);
/**
 * @param[in, out] src   pointer of the input vector. After the function is executed, the
 *                       output will be stored in the input vector.
 * @param[in]        m   base 2 logarithm value of the sample number and it can be set from 3 to 7
 * @return none
 *
 * @b Note:
 *
 * The input and output formats are listed below. To satisfy the input format corresponding to
 * your input size, you may need to perform an arithmetic shift operation before calling this
 * function.
 *
 * @image html idct4_q31.gif ""
 */
void riscv_dsp_idct4_q31(q31_t *src, uint32_t m);
/** @} dct4 */

// MFCC
/// single-precision floating-point MFCC Function
/**
 * @defgroup MFCC Functions
 * @brief MFCC Functions
 *
 * MFCCs are coefficients that collectively make up a mel-frequency cepstrum (MFC). They are commonly derived from a type of cepstral representation of the audio clip in the following process.
 * 1. Take the Fourier transform of (a windowed excerpt of) a signal.
 * 2. Map the powers of the spectrum obtained above onto the mel scale, using triangular or cosine overlapping windows.
 * 3. Take the logs of the powers at each of the mel frequencies.
 * 4. Take the discrete cosine transform of the list of mel log powers, as if it were a signal.
 * 5. The MFCCs are the amplitudes of the resulting spectrum.
 *
 * Before calling an MFCC function (e.g., riscv_dsp_mfcc_f32), ensure you initialize its structure with a corresponding initialization function (e.g., riscv_dsp_mfcc_init_f32) first so that correct results can be guaranteed.
 *
 * Andes DSP library supports the following data types: single- and half-precision floating-point, Q31 and Q15. These functions are introduced in the subsections below.
 */
/**
 * @addtogroup mfcc
 * @{
 */

/**
 * @brief Instance structure for single-precision floating-point MFCC Function
 */
typedef struct
{
    uint32_t fft_len; /**< FFT length */
    uint32_t n_mel_filters; /**< Number of Mel filters */
    uint32_t n_dct_out; /**< Number of DCT outputs */
    float32_t *dct_coefs; /**< Internal DCT coefficients */
    uint32_t *filter_pos; /**< Internal Mel filter positions in spectrum */
    uint32_t *filter_len; /**< Internal Mel filter  lengths */
    float32_t *filter_coefs; /**< Internal Mel filter coefficients */
    float32_t *window_coefs; /**< Windowing coefficients */
    uint32_t log2_fft_len; /**< log2 of FFT length */
} riscv_dsp_mfcc_f32_t;

/**
 * @brief Function to initialize F32 MFCC.
 * @param[out]      *instance pointer of the instance structure
 * @param[in]       fft_len          RFFT length
 * @param[in]       n_mel_filters    number of Mel filters
 * @param[in]       n_dct_out        number of DCT outputs
 * @param[in]       *dct_coefs       pointer of the DCT coefficients vector
 * @param[in]       *filter_pos      pointer of the Mel filter positions vector
 * @param[in]       *filter_len      pointer of the Mel filter lengths vector
 * @param[in]       *filter_coefs    pointer of the Mel filter coefficients vector
 * @param[in]       **window_coefs   pointer of the window coefficients vector
 * @return none.
 */
void riscv_dsp_mfcc_init_f32(riscv_dsp_mfcc_f32_t * instance,
                            uint32_t fft_len,
                            uint32_t n_mel_filters,
                            uint32_t n_dct_out,
                            float32_t *dct_coefs,
                            uint32_t *filter_pos,
                            uint32_t *filter_len,
                            float32_t *filter_coefs,
                            float32_t *window_coefs);

/**
 * @brief MFCC function for F32.
 * @param[in]      *instance     pointer of the instance structure
 * @param[in]      *src          pointer of the input vector
 * @param[out]     *dst          pointer of the output vector
 * @param[in]      *buf          pointer of the temporary buffer
 */
void riscv_dsp_mfcc_f32(const riscv_dsp_mfcc_f32_t * instance, float32_t *src, float32_t *dst, float32_t *buf);

/**
 * @brief Instance structure for Q31 MFCC Function
 */
typedef struct
{
    uint32_t fft_len; /**< FFT length */
    uint32_t n_mel_filters; /**< Number of Mel filters */
    uint32_t n_dct_out; /**< Number of DCT outputs */
    q31_t *dct_coefs; /**< Internal DCT coefficients */
    uint32_t *filter_pos; /**< Internal Mel filter positions in spectrum */
    uint32_t *filter_len; /**< Internal Mel filter  lengths */
    q31_t *filter_coefs; /**< Internal Mel filter coefficients */
    q31_t *window_coefs; /**< Windowing coefficients */
    uint32_t log2_fft_len; /**< log2 of FFT length */
} riscv_dsp_mfcc_q31_t;

/**
 * @brief Function to initialize Q31 MFCC.
 * @param[out]      *instance pointer of the instance structure
 * @param[in]       fft_len          RFFT length
 * @param[in]       n_mel_filters    number of Mel filters
 * @param[in]       n_dct_out        number of DCT outputs
 * @param[in]       *dct_coefs       pointer of the DCT coefficients vector
 * @param[in]       *filter_pos      pointer of the Mel filter positions vector
 * @param[in]       *filter_len      pointer of the Mel filter lengths vector
 * @param[in]       *filter_coefs    pointer of the Mel filter coefficients vector
 * @param[in]       **window_coefs   pointer of the window coefficients vector
 * @return none.
 */
void riscv_dsp_mfcc_init_q31(riscv_dsp_mfcc_q31_t * instance,
                            uint32_t fft_len,
                            uint32_t n_mel_filters,
                            uint32_t n_dct_out,
                            q31_t *dct_coefs,
                            uint32_t *filter_pos,
                            uint32_t *filter_len,
                            q31_t *filter_coefs,
                            q31_t *window_coefs);

/**
 * @brief MFCC function for Q31.
 * @param[in]      *instance     pointer of the instance structure
 * @param[in]      *src          pointer of the input vector
 * @param[out]     *dst          pointer of the output vector
 * @param[in]      *buf          pointer of the temporary buffer
 */
void riscv_dsp_mfcc_q31(const riscv_dsp_mfcc_q31_t * instance, q31_t *src, q31_t *dst, q31_t *buf);

/**
 * @brief Instance structure for Q15 MFCC Function
 */
typedef struct
{
    uint32_t fft_len; /**< FFT length */
    uint32_t n_mel_filters; /**< Number of Mel filters */
    uint32_t n_dct_out; /**< Number of DCT outputs */
    q15_t *dct_coefs; /**< Internal DCT coefficients */
    uint32_t *filter_pos; /**< Internal Mel filter positions in spectrum */
    uint32_t *filter_len; /**< Internal Mel filter  lengths */
    q15_t *filter_coefs; /**< Internal Mel filter coefficients */
    q15_t *window_coefs; /**< Windowing coefficients */
    uint32_t log2_fft_len; /**< log2 of FFT length */
} riscv_dsp_mfcc_q15_t;

/**
 * @brief Function to initialize Q15 MFCC.
 * @param[out]      *instance pointer of the instance structure
 * @param[in]       fft_len          RFFT length
 * @param[in]       n_mel_filters    number of Mel filters
 * @param[in]       n_dct_out        number of DCT outputs
 * @param[in]       *dct_coefs       pointer of the DCT coefficients vector
 * @param[in]       *filter_pos      pointer of the Mel filter positions vector
 * @param[in]       *filter_len      pointer of the Mel filter lengths vector
 * @param[in]       *filter_coefs    pointer of the Mel filter coefficients vector
 * @param[in]       **window_coefs   pointer of the window coefficients vector
 * @return none.
 */
void riscv_dsp_mfcc_init_q15(riscv_dsp_mfcc_q15_t * instance,
                            uint32_t fft_len,
                            uint32_t n_mel_filters,
                            uint32_t n_dct_out,
                            q15_t *dct_coefs,
                            uint32_t *filter_pos,
                            uint32_t *filter_len,
                            q15_t *filter_coefs,
                            q15_t *window_coefs);

/**
 * @brief MFCC function for Q15.
 * @param[in]      *instance     pointer of the instance structure
 * @param[in]      *src          pointer of the input vector
 * @param[out]     *dst          pointer of the output vector
 * @param[in]      *buf          pointer of the temporary buffer
 */
void riscv_dsp_mfcc_q15(const riscv_dsp_mfcc_q15_t * instance, q15_t *src, q15_t *dst, q31_t *buf);

#if defined (__riscv_zfh)
/**
 * @brief Instance structure for single-precision floating-point MFCC Function
 */
typedef struct
{
    uint32_t fft_len; /**< FFT length */
    uint32_t n_mel_filters; /**< Number of Mel filters */
    uint32_t n_dct_out; /**< Number of DCT outputs */
    float16_t *dct_coefs; /**< Internal DCT coefficients */
    uint32_t *filter_pos; /**< Internal Mel filter positions in spectrum */
    uint32_t *filter_len; /**< Internal Mel filter  lengths */
    float16_t *filter_coefs; /**< Internal Mel filter coefficients */
    float16_t *window_coefs; /**< Windowing coefficients */
    uint32_t log2_fft_len; /**< log2 of FFT length */
} riscv_dsp_mfcc_f16_t;

/**
 * @brief Function to initialize F16 MFCC.
 * @param[out]      *instance pointer of the instance structure
 * @param[in]       fft_len          RFFT length
 * @param[in]       n_mel_filters    number of Mel filters
 * @param[in]       n_dct_out        number of DCT outputs
 * @param[in]       *dct_coefs       pointer of the DCT coefficients vector
 * @param[in]       *filter_pos      pointer of the Mel filter positions vector
 * @param[in]       *filter_len      pointer of the Mel filter lengths vector
 * @param[in]       *filter_coefs    pointer of the Mel filter coefficients vector
 * @param[in]       **window_coefs   pointer of the window coefficients vector
 * @return none.
 */
void riscv_dsp_mfcc_init_f16(riscv_dsp_mfcc_f16_t * instance,
                            uint32_t fft_len,
                            uint32_t n_mel_filters,
                            uint32_t n_dct_out,
                            float16_t *dct_coefs,
                            uint32_t *filter_pos,
                            uint32_t *filter_len,
                            float16_t *filter_coefs,
                            float16_t *window_coefs);

/**
 * @brief MFCC function for F16.
 * @param[in]      *instance     pointer of the instance structure
 * @param[in]      *src          pointer of the input vector
 * @param[out]     *dst          pointer of the output vector
 * @param[in]      *buf          pointer of the temporary buffer
 */
void riscv_dsp_mfcc_f16(const riscv_dsp_mfcc_f16_t * instance, float16_t *src, float16_t *dst, float16_t *buf);

#endif

/** @} mfcc */

/** @} trans */

#ifdef  __cplusplus
}
#endif
#endif // __RISCV_DSP32_TRANSFORM_MATH_H__
