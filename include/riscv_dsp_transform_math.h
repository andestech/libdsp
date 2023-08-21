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
/**
 * Backward compatibility
 */
#define riscv_dsp_cfft_rd2_f32  riscv_dsp_cfft_f32
#define riscv_dsp_cfft_rd2_f64  riscv_dsp_cfft_f64
#define riscv_dsp_cfft_rd2_q15  riscv_dsp_cfft_q15
#define riscv_dsp_cfft_rd2_q31  riscv_dsp_cfft_q31

#define riscv_dsp_cifft_rd2_f32  riscv_dsp_cifft_f32
#define riscv_dsp_cifft_rd2_f64  riscv_dsp_cifft_f64
#define riscv_dsp_cifft_rd2_q15  riscv_dsp_cifft_q15
#define riscv_dsp_cifft_rd2_q31  riscv_dsp_cifft_q31

#define riscv_dsp_cfft_rd4_f32  riscv_dsp_cfft_f32
#define riscv_dsp_cfft_rd4_f64  riscv_dsp_cfft_f64
#define riscv_dsp_cfft_rd4_q15  riscv_dsp_cfft_q15
#define riscv_dsp_cfft_rd4_q31  riscv_dsp_cfft_q31

#define riscv_dsp_cifft_rd4_f32  riscv_dsp_cifft_f32
#define riscv_dsp_cifft_rd4_f64  riscv_dsp_cifft_f64
#define riscv_dsp_cifft_rd4_q15  riscv_dsp_cifft_q15
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

/** @} trans */

#ifdef  __cplusplus
}
#endif
#endif // __RISCV_DSP32_TRANSFORM_MATH_H__
