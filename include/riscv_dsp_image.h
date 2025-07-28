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
#ifndef __RISCV_DSP_IMAGE_H__
#define __RISCV_DSP_IMAGE_H__

/** @file*/

#ifdef  __cplusplus
extern "C"
{
#endif

#include "riscv_dsp_math_types.h"

/**
 * @defgroup groupImage Image Functions
 * @brief image process Functions
 *
 * This set of functions computes the image data for data type U8.
 * These functions are introduced in the subsections below.
 */
/**
 * @addtogroup groupImage
 * @{
 */

/**
 * @defgroup ImageResize Image Resize Functions
 *
 * @brief The function resizes images using the bilinear interpolation algorithm.
 *
 * The function resizes images using the bilinear interpolation algorithm. The input pixel is arranged in RGB or RGBA format
 *  RGB:   [R[0], G[0], B[0], ...].
 *  RGBA:  [R[0], G[0], B[0], A[0], ...].
 */

 /**
  * @addtogroup ImageResize
  * @{
  */

/**
 * @brief Image resize U8 function
 *
 * @param[out] dst pointer of the output vector
 * @param[in] dst_width number of width in the output vector
 * @param[in] dst_height number of height in the output vector
 * @param[in] src pointer of the input vector
 * @param[in] src_width number of width in the input vector
 * @param[in] src_height number of height in the input vector
 * @param[in] src_stride stride of the input vector
 */
void riscv_dsp_img_resize_bilinear_rgb_u8(u8_t* dst,
                                      u32_t dst_width,
                                      u32_t dst_height,
                                      u8_t* src,
                                      u32_t src_width,
                                      u32_t src_height,
                                      u32_t src_stride);
/** @} */

/**
 * @defgroup image_cvt Image Convert Functions
 *
 * @brief Image Convert Functions
 *
 * This function converts an image from one color space to another. Andes vector library supports the following conversion code as riscv_vec_img_color_cvt of enum type.
 *
 * typedef enum {
 *    RISCV_DSP_COLOR_YUV2RGB_NV12 = 90,
 *    RISCV_DSP_COLOR_YUV2BGR_NV12 = 91,
 *    RISCV_DSP_COLOR_YUV2RGBA_NV12 = 94,
 *    RISCV_DSP_COLOR_YUV2BGRA_NV12 = 95,
 *    RISCV_DSP_COLOR_YUV2RGB_YV12 = 98,
 *    RISCV_DSP_COLOR_YUV2BGR_YV12 = 99,
 *    RISCV_DSP_COLOR_YUV2RGB_IYUV = 100,
 *    RISCV_DSP_COLOR_YUV2BGR_IYUV = 101,
 *    RISCV_DSP_COLOR_YUV2RGBA_YV12 = 102,
 *    RISCV_DSP_COLOR_YUV2BGRA_YV12 = 103,
 *    RISCV_DSP_COLOR_YUV2RGBA_IYUV = 104,
 *    RISCV_DSP_COLOR_YUV2BGRA_IYUV = 105
 * } riscv_dsp_img_color_cvt;
 */

 /**
  * @addtogroup image_cvt
  * @{
  */
/**
 * @brief structure of the u8 image
 */
typedef struct
{
        u8_t *data;     /**<Points to the image data*/
        u32_t height;   /**<Height of the image*/
        u32_t width;    /**<Width of the image*/
        u32_t channels; /**<Channels of the image*/
} riscv_dsp_img_u8_t;

/**
 * @brief Conversion code of riscv_vec_img_color_cvt
 */
typedef enum {
       RISCV_DSP_COLOR_YUV2RGB_NV12  = 90,
       RISCV_DSP_COLOR_YUV2BGR_NV12  = 91,
       RISCV_DSP_COLOR_YUV2RGB_NV21  = 92,
       RISCV_DSP_COLOR_YUV2BGR_NV21  = 93,
       RISCV_DSP_COLOR_YUV2RGBA_NV12 = 94,
       RISCV_DSP_COLOR_YUV2BGRA_NV12 = 95,
       RISCV_DSP_COLOR_YUV2RGBA_NV21 = 96,
       RISCV_DSP_COLOR_YUV2BGRA_NV21 = 97,
       RISCV_DSP_COLOR_YUV2RGB_YV12 = 98,
       RISCV_DSP_COLOR_YUV2BGR_YV12 = 99,
       RISCV_DSP_COLOR_YUV2RGB_IYUV = 100,
       RISCV_DSP_COLOR_YUV2BGR_IYUV = 101,
       RISCV_DSP_COLOR_YUV2RGBA_YV12 = 102,
       RISCV_DSP_COLOR_YUV2BGRA_YV12 = 103,
       RISCV_DSP_COLOR_YUV2RGBA_IYUV = 104,
       RISCV_DSP_COLOR_YUV2BGRA_IYUV = 105
} riscv_dsp_img_color_cvt;

/**
 * @brief Image color conversion function for U8.
 *
 * @param[in] img_src           Points to the input image structure
 * @param[out] img_dst          Points to the output image structure
 * @param[in] type              Image color conversion code
 * @return -1 if conversion fails, 0 if successful.
 */
q31_t riscv_dsp_img_cvtColor_u8(riscv_dsp_img_u8_t img_src, riscv_dsp_img_u8_t img_dst, riscv_dsp_img_color_cvt type);

/** @} image_cvt */
/** @} */

#ifdef  __cplusplus
}
#endif

#endif // __RISCV_DSP_IMAGE_H__
