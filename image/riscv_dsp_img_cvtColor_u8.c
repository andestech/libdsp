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
#include "riscv_dsp_image.h"

#include "internal_image_cvtColor.h"
q31_t riscv_dsp_img_cvtColor_u8(riscv_dsp_img_u8_t img_src, riscv_dsp_img_u8_t img_dst, riscv_dsp_img_color_cvt type)
{
    rbg_output b_idx;
    switch(type)
    {
    case RISCV_DSP_COLOR_YUV2RGB_NV12:
    case RISCV_DSP_COLOR_YUV2RGBA_NV12:
    {
        nv12_to_rgb_input u_idx;
        u_idx = UV_INPUT;
        b_idx = OUTPUT_RGB;
        vec_img_nv12torgb(img_src, img_dst, u_idx, b_idx);
        return NDSV_OK;
    }
    case RISCV_DSP_COLOR_YUV2RGB_NV21:
    case RISCV_DSP_COLOR_YUV2RGBA_NV21:
    {
        nv12_to_rgb_input u_idx;
        u_idx = VU_INPUT;
        b_idx = OUTPUT_RGB;
        vec_img_nv12torgb(img_src, img_dst, u_idx, b_idx);
        return NDSV_OK;
    }
    case RISCV_DSP_COLOR_YUV2BGR_NV12:
    case RISCV_DSP_COLOR_YUV2BGRA_NV12:
    {
        nv12_to_rgb_input u_idx;
        u_idx = UV_INPUT;
        b_idx = OUTPUT_BGR;
        vec_img_nv12torgb(img_src, img_dst, u_idx, b_idx);
        return NDSV_OK;
    }
    case RISCV_DSP_COLOR_YUV2BGR_NV21:
    case RISCV_DSP_COLOR_YUV2BGRA_NV21:
    {
        nv12_to_rgb_input u_idx;
        u_idx = VU_INPUT;
        b_idx = OUTPUT_BGR;
        vec_img_nv12torgb(img_src, img_dst, u_idx, b_idx);
        return NDSV_OK;
    }
    case RISCV_DSP_COLOR_YUV2RGB_IYUV:
    case RISCV_DSP_COLOR_YUV2RGBA_IYUV:
    {
        iyuv_to_rgb_input u_idx;
        u_idx = U_PLANE_FIRST;
        b_idx = OUTPUT_RGB;
        vec_img_iyuvtorgb(img_src, img_dst, u_idx, b_idx);
        return NDSV_OK;
    }
    case RISCV_DSP_COLOR_YUV2BGR_IYUV:
    case RISCV_DSP_COLOR_YUV2BGRA_IYUV:
    {
        iyuv_to_rgb_input u_idx;
        u_idx = U_PLANE_FIRST;
        b_idx = OUTPUT_BGR;
        vec_img_iyuvtorgb(img_src, img_dst, u_idx, b_idx);
        return NDSV_OK;
    }
    case RISCV_DSP_COLOR_YUV2RGB_YV12:
    case RISCV_DSP_COLOR_YUV2RGBA_YV12:
    {
        iyuv_to_rgb_input u_idx;
        u_idx = V_PLANE_FIRST;
        b_idx = OUTPUT_RGB;
        vec_img_iyuvtorgb(img_src, img_dst, u_idx, b_idx);
        return NDSV_OK;
    }
    case RISCV_DSP_COLOR_YUV2BGR_YV12:
    case RISCV_DSP_COLOR_YUV2BGRA_YV12:
    {
        iyuv_to_rgb_input u_idx;
        u_idx = V_PLANE_FIRST;
        b_idx = OUTPUT_BGR;
        vec_img_iyuvtorgb(img_src, img_dst, u_idx, b_idx);
        return NDSV_OK;
    }
    default:
    {
        return NDSV_FAIL;
    }
    }
}
