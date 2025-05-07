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

#ifndef __RISCV_DSP_INTERP_H__
#define __RISCV_DSP_INTERP_H__

#ifdef  __cplusplus
extern "C"
{
#endif

#include "riscv_dsp_math_types.h"

/**
 * @defgroup interp Interpolation Functions
 */
/**
 * @addtogroup interp
 * @{
 */

// linear interpolation Functions
/**
 * @defgroup linear_interp Linear Interpolation Functions
 * @brief Linear Interpolation Functions
 *
 * Linear interpolation is a method of curve fitting using linear polynomials. Linear interpolation works by effectively drawing a straight line between two neighboring samples and returning the appropriate point along that line.
 *
 * Andes DSP library supports the linear interpolation functions for single- and half-precision floating-point, Q31, Q15 and Q7 data types. These functions are introduced in the subsections below.
 */

/**
 * @addtogroup linear_interp
 * @{
 */
/**
 * @brief Instance structure for the f32 linear interpolate function.
 */
typedef struct
{
    uint32_t tbl_size;             /**< number of table size */
    float32_t first_x;             /**< first x of table */
    float32_t x_spacing;           /**< x-axis spacing */
    const float32_t * tbl_data;    /**< pointer of the table data */
} riscv_dsp_linear_interp_f32_t;

/**
 * @brief Linear interpolate function of f32 data types
 * @param[in]       *instance pointer of the instance structure
 * @param[in]       *src      pointer of the input vector
 * @param[out]      *dst pointer of the output vector.
 * @param[in]       size number of elements in a vector
 * @return Always zero.
 */
int riscv_dsp_linear_interp_f32(riscv_dsp_linear_interp_f32_t *instance, float32_t *src, float32_t *dst, uint32_t size);

#if defined (__riscv_zfh)
/**
 * @brief Instance structure for the f16 linear interpolate function.
 */
typedef struct
{
    uint32_t tbl_size;             /**< number of table size */
    float16_t first_x;             /**< first x of table */
    float16_t x_spacing;           /**< x-axis spacing */
    const float16_t * tbl_data;    /**< pointer of the table data */
} riscv_dsp_linear_interp_f16_t;

/**
 * @brief Linear interpolate function of f16 data types
 * @param[in]       *instance pointer of the instance structure
 * @param[in]       *src      pointer of the input vector
 * @param[out]      *dst pointer of the output vector.
 * @param[in]       size number of elements in a vector
 * @return Always zero.
 */
int riscv_dsp_linear_interp_f16(riscv_dsp_linear_interp_f16_t *instance, float16_t *src, float16_t *dst, uint32_t size);
#endif

/**
 * @brief Instance structure for the q15 linear interpolate function.
 */
typedef struct
{
    uint32_t tbl_size;         /**< number of table size */
    const q15_t * tbl_data;    /**< pointer of the table data */
} riscv_dsp_linear_interp_q15_t;

/**
 * @brief Linear interpolate function of q15 data types
 * @param[in]       *instance pointer of the instance structure
 * @param[in]       *src      pointer of the input vector
 * @param[out]      *dst pointer of the output vector.
 * @param[in]       size number of elements in a vector
 * @return Always zero.
 */
int riscv_dsp_linear_interp_q15(riscv_dsp_linear_interp_q15_t * instance, q31_t * src, q15_t * dst, uint32_t size);

/**
 * @brief Instance structure for the q31 linear interpolate function.
 */
typedef struct
{
    uint32_t tbl_size;         /**< number of table size */
    const q31_t * tbl_data;    /**< pointer of the table data */
} riscv_dsp_linear_interp_q31_t;

/**
 * @brief Linear interpolate function of q31 data types
 * @param[in]       *instance pointer of the instance structure
 * @param[in]       *src      pointer of the input vector
 * @param[out]      *dst pointer of the output vector.
 * @param[in]       size number of elements in a vector
 * @return Always zero.
 */
int riscv_dsp_linear_interp_q31(riscv_dsp_linear_interp_q31_t * instance, q31_t * src, q31_t * dst, uint32_t size);

/**
 * @brief Instance structure for the q7 linear interpolate function.
 */
typedef struct
{
    uint32_t tbl_size;         /**< number of table size */
    const q7_t * tbl_data;    /**< pointer of the table data */
} riscv_dsp_linear_interp_q7_t;

/**
 * @brief Linear interpolate function of q7 data types
 * @param[in]       *instance pointer of the instance structure
 * @param[in]       *src      pointer of the input vector
 * @param[out]      *dst pointer of the output vector.
 * @param[in]       size number of elements in a vector
 * @return Always zero.
 */
int riscv_dsp_linear_interp_q7(riscv_dsp_linear_interp_q7_t * instance, q31_t * src, q7_t * dst, uint32_t size);
/** @} linear_interp */

// Bilinear interpolation Functions
/**
 * @defgroup bilinear_interp Bilinear Interpolation Functions
 * @brief Bilinear Interpolation Functions
 *
 * Bilinear interpolation is an extension of linear interpolation applied to a two-dimensional grid. The underlying function f(x, y) is sampled on a regular grid and the interpolation process determines values between the grid points. Bilinear interpolation is equivalent to two-step linear interpolation, first in the x-dimension and then in the y-dimension.
 *
 * Andes DSP library supports the bilinear interpolation functions for single- and half-precision floating-point, Q31, Q15 and Q7 data types. These functions are introduced in the subsections below.
 */

/**
 * @addtogroup bilinear_interp
 * @{
 */
/**
 * @brief Instance structure of the f32 bilinear interpolation
 */
typedef struct
{
    uint32_t row;   /**< number of rows in the data table. */
    uint32_t col;   /**< number of columns in the data table. */
    const float32_t * tbl_data;   /**< points to the data table. */
}riscv_dsp_bilinear_interp_f32_t;

/**
 * @brief Bilinear interpolate function of f32 data types
 * @param[in]       *instance pointer of the instance structure
 * @param[in]       *src_x      pointer of the input x coordinate vector
 * @param[in]       *src_y      pointer of the input y coordinate vector
 * @param[out]      *dst pointer of the output vector.
 * @param[in]       size number of elements in a vector
 * @return Always zero.
 */
int riscv_dsp_bilinear_interp_f32(riscv_dsp_bilinear_interp_f32_t * instance, float32_t * src_x, float32_t * src_y, float32_t * dst, uint32_t size);

/**
 * @brief Instance structure of the q31 bilinear interpolation
 */
typedef struct
{
    uint32_t row;   /**< number of rows in the data table. */
    uint32_t col;   /**< number of columns in the data table. */
    const q31_t * tbl_data;   /**< points to the data table. */
}riscv_dsp_bilinear_interp_q31_t;

/**
 * @brief Bilinear interpolate function of q31 data types
 * @param[in]       *instance pointer of the instance structure
 * @param[in]       *src_x      pointer of the input x coordinate vector
 * @param[in]       *src_y      pointer of the input y coordinate vector
 * @param[out]      *dst pointer of the output vector.
 * @param[in]       size number of elements in a vector
 * @return Always zero.
 */
int riscv_dsp_bilinear_interp_q31(riscv_dsp_bilinear_interp_q31_t * instance, q31_t * src_x, q31_t * src_y, q31_t * dst, uint32_t size);

/**
 * @brief Instance structure of the q15 bilinear interpolation
 */
typedef struct
{
    uint32_t row;   /**< number of rows in the data table. */
    uint32_t col;   /**< number of columns in the data table. */
    const q15_t * tbl_data;   /**< points to the data table. */
}riscv_dsp_bilinear_interp_q15_t;

/**
 * @brief Bilinear interpolate function of q15 data types
 * @param[in]       *instance pointer of the instance structure
 * @param[in]       *src_x      pointer of the input x coordinate vector
 * @param[in]       *src_y      pointer of the input y coordinate vector
 * @param[out]      *dst pointer of the output vector.
 * @param[in]       size number of elements in a vector
 * @return Always zero.
 */
int riscv_dsp_bilinear_interp_q15(riscv_dsp_bilinear_interp_q15_t * instance, q31_t * src_x, q31_t * src_y, q15_t * dst, uint32_t size);

/**
 * @brief Instance structure of the q7 bilinear interpolation
 */
typedef struct
{
    uint32_t row;   /**< number of rows in the data table. */
    uint32_t col;   /**< number of columns in the data table. */
    const q7_t * tbl_data;   /**< points to the data table. */
}riscv_dsp_bilinear_interp_q7_t;

/**
 * @brief Bilinear interpolate function of q7 data types
 * @param[in]       *instance pointer of the instance structure
 * @param[in]       *src_x      pointer of the input x coordinate vector
 * @param[in]       *src_y      pointer of the input y coordinate vector
 * @param[out]      *dst pointer of the output vector.
 * @param[in]       size number of elements in a vector
 * @return Always zero.
 */
int riscv_dsp_bilinear_interp_q7(riscv_dsp_bilinear_interp_q7_t * instance, q31_t * src_x, q31_t * src_y, q7_t * dst, uint32_t size);
#if defined (__riscv_zfh)
/**
 * @brief Instance structure of the f16 bilinear interpolation
 */
typedef struct
{
    uint32_t row;   /**< number of rows in the data table. */
    uint32_t col;   /**< number of columns in the data table. */
    const float16_t * tbl_data;   /**< points to the data table. */
}riscv_dsp_bilinear_interp_f16_t;

/**
 * @brief Bilinear interpolate function of f16 data types
 * @param[in]       *instance pointer of the instance structure
 * @param[in]       *src_x      pointer of the input x coordinate vector
 * @param[in]       *src_y      pointer of the input y coordinate vector
 * @param[out]      *dst pointer of the output vector.
 * @param[in]       size number of elements in a vector
 * @return Always zero.
 */
int riscv_dsp_bilinear_interp_f16(riscv_dsp_bilinear_interp_f16_t * instance, float16_t * src_x, float16_t * src_y, float16_t * dst, uint32_t size);
#endif
/** @} bilinear_interp */

// Cubic Spline Interpolation Functions
/**
 * @defgroup spline_interp Cubic Spline Interpolation Functions
 * @brief Cubic Spline Interpolation Functions
 *
 * Spline interpolation is a method of interpolation where the interpolant is a piecewise-defined polynomial called "spline". You can use the function riscv_dsp_spline_interp_init_f32 to calculate the coefficient of spline and call the function riscv_dsp_spline_interp_f32 to get the interpolation result of the input vector.
 *
 * Andes DSP library only supports the spline interpolation functions for the F32 data type. The function is introduced in the subsections below.
 */

/**
 * @addtogroup spline_interp
 * @{
 */
/**
 * @brief Struct for specifying cubic spline type
 */
typedef enum
{
  RISCV_DSP_SPLINE_NATURAL = 0,           /**< Natural spline */
  RISCV_DSP_SPLINE_PARABOLIC_RUNOUT = 1   /**< Parabolic runout spline */
} riscv_dsp_spline_bound_cond;

/**
 * @brief Instance structure for the f32 cubic spline interpolation.
 */
typedef struct
{
  riscv_dsp_spline_bound_cond cond;   /**< Type (boundary conditions) */
  const float32_t * x;                /**< x values */
  const float32_t * y;                /**< y values */
  uint32_t num_x;                     /**< Number of known data points */
  float32_t * coeffs;                 /**< Coefficients buffer (b,c, and d) */
} riscv_dsp_spline_interp_f32_t;

/**
 * @brief Cubic spline interpolation initialize function
 * @param[in, out]    *instance pointer of the instance structure
 * @param[in]       cond        type of the boundary condition
 * @param[in]       *srcx      pointer of the x coordinate known data vector
 * @param[in]       *srcy      pointer of the y coordinate known data vector
 * @param[in]       nsize      number of known date points in a vector
 * @param[in]       *coeffs    pointer of the coefficient vector
 * @param[in]       *tmpbuf    pointer of the temporary buffer
 * @return none.
 */
void riscv_dsp_spline_interp_init_f32(riscv_dsp_spline_interp_f32_t * instance,
                                      riscv_dsp_spline_bound_cond cond,
                                      const float32_t * srcx,
                                      const float32_t * srcy,
                                      uint32_t nsize,
                                      float32_t * coeffs,
                                      float32_t * tmpbuf);

/**
 * @brief Cubic spline interpolation function
 * @param[in]       *instance pointer of the instance structure
 * @param[in]       *src      pointer of the input vector
 * @param[out]      *dst pointer of the output vector.
 * @param[in]       size number of elements in a vector
 * @return none.
 */
void riscv_dsp_spline_interp_f32(riscv_dsp_spline_interp_f32_t * instance, const float32_t * src, float32_t * dst, uint32_t size);

/** @} spline_interp */
/** @} interp */

#ifdef  __cplusplus
}
#endif
#endif // __RISCV_DSP_INTERP_H__
