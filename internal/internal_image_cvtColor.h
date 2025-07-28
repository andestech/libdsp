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

#ifndef __INTERNAL_IMAGE_CVTCOLOR_H__
#define __INTERNAL_IMAGE_CVTCOLOR_H__

#include "stdlib.h"

typedef enum
{
    OUTPUT_BGR = 0,
    OUTPUT_RGB = 2

} rbg_output;

typedef enum
{
    UV_INPUT = 0,
    VU_INPUT = 1

} nv12_to_rgb_input;

typedef enum
{
    U_PLANE_FIRST = 0,
    V_PLANE_FIRST = 1
} iyuv_to_rgb_input;

#include "internal_image_cvtColor_template_pureC.h"

#include "internal_isa.h"

#include "internal_nds_types.h"
#include <math.h>
#include "riscv_dsp_image.h"

static inline void vec_img_nv12torgb(riscv_dsp_img_u8_t img_src, riscv_dsp_img_u8_t img_dst, nv12_to_rgb_input u_idx, rbg_output b_idx)
{
    uint32_t tmp_i = 0,  tmp_j = 0;
    uint32_t src_w = img_src.width;
    uint32_t src_h = img_src.height;
    uint32_t uv_offset_idx = 0 ;
    uint32_t dst_ch = img_dst.channels;
    uint32_t uv_height_offset = src_h*2/3;
    uint8_t *p_y_start = img_src.data;
    uint8_t *p_uv_start = p_y_start + uv_height_offset*src_w;
    uint8_t *p_y_offset = NULL, *p_uv_offset = NULL;
    uint8_t *p_row = img_dst.data;
    uint8_t u, v, y0, y1;

    for( tmp_i = 0 ; tmp_i < uv_height_offset ; tmp_i ++)
    {
        p_y_offset = &p_y_start[tmp_i*src_w];
        uv_offset_idx = tmp_i >> 1;
        p_uv_offset = &p_uv_start[uv_offset_idx*src_w];
        for(tmp_j = 0 ; tmp_j < src_w/2 ; tmp_j ++)
        {
            u = p_uv_offset[0+u_idx];
            v = p_uv_offset[1-u_idx];
            y0 = p_y_offset[0];
            y1 = p_y_offset[1];
            vec_cvtYuv42xxp2RGB8_C(u, v, y0, y1, p_row, dst_ch, b_idx);
            p_y_offset += 2;
            p_uv_offset += 2;
            p_row += 2*dst_ch;
        }
    }
}

static inline void vec_img_iyuvtorgb(riscv_dsp_img_u8_t img_src, riscv_dsp_img_u8_t img_dst, iyuv_to_rgb_input u_idx, rbg_output b_idx)
{
    uint32_t tmp_i = 0,  tmp_j = 0;
    uint32_t src_w = img_src.width;
    uint32_t src_h = img_src.height;
    uint32_t uv_offset_idx = 0 ;
    uint32_t dst_ch = img_dst.channels;
    uint32_t u_height_offset = src_h*2/3;
    uint32_t v_height_offset = src_h*1/3;
    uint32_t uv_height_offset = u_height_offset;
    uint8_t *p_y_start = img_src.data;
    uint8_t *p_u_start = p_y_start + u_height_offset*src_w;
    uint8_t *p_v_start = p_u_start + (v_height_offset*src_w)/2;
    uint8_t *p_y_offset = NULL, *p_u_offset = NULL, *p_v_offset = NULL;
    uint8_t *p_row = img_dst.data;
    uint8_t u, v, y0, y1;
    if(u_idx == V_PLANE_FIRST)
    {
        v_height_offset = src_h*2/3;
        u_height_offset = src_h*1/3;
        uv_height_offset = v_height_offset;
        p_v_start = p_y_start + v_height_offset*src_w;
        p_u_start = p_v_start + (u_height_offset*src_w)/2;
    }
    for( tmp_i = 0 ; tmp_i < uv_height_offset ; tmp_i ++)
    {
        p_y_offset = &p_y_start[tmp_i*src_w];
        uv_offset_idx = tmp_i >> 1;
        p_u_offset = &p_u_start[uv_offset_idx*src_w/2];
        p_v_offset = &p_v_start[uv_offset_idx*src_w/2];
        for(tmp_j = 0 ; tmp_j < src_w/2 ; tmp_j ++)
        {
            u = p_u_offset[tmp_j];
            v = p_v_offset[tmp_j];
            y0 = p_y_offset[0];
            y1 = p_y_offset[1];
            vec_cvtYuv42xxp2RGB8_C(u, v, y0, y1, p_row, dst_ch, b_idx);
            p_y_offset += 2;
            p_row += 2*dst_ch;
        }
    }
}

#endif
