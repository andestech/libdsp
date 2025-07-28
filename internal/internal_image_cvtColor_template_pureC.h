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

#ifndef __INTERNAL_IMAGE_CVTCOLOR_TEMPLATE_PUREC_H__
#define __INTERNAL_IMAGE_CVTCOLOR_TEMPLATE_PUREC_H__

#include "internal_nds_types.h"

#include "internal_isa.h"
#define NDSLIB_SATU NDS_ISA_SAT

static const int ITUR_BT_601_CY = 1220542;    // 1.164 in Q20
static const int ITUR_BT_601_CUB = 2116026;   // 2.018 in Q20
static const int ITUR_BT_601_CUG = -409993;   // -0.391 in Q20
static const int ITUR_BT_601_CVG = -852492;   // -0.813 in Q20
static const int ITUR_BT_601_CVR = 1673527;   //1.596 in Q20
static const int ITUR_BT_601_SHIFT = 20;
#define MAX(a, b) ((a < b) ? b : a)
#define MIN(a, b) ((a < b) ? a : b)
static inline void vec_cvtYuv42xxp2RGB8_C(const uint8_t u, const uint8_t v,
                                    const uint8_t y0, const uint8_t y1, uint8_t* dst, uint32_t dcn, uint32_t bIdx)
{
    //int32_t uu, vv, tmp_y0, tmp_y1, y, tmp_result;
    int32_t uu, vv, tmp_y0, tmp_y1, y;
    int32_t ruv, guv, buv;
    uint8_t r0, g0, b0, a0;
    uint8_t r1, g1, b1, a1;
    uu = (int32_t)u - 128;
    vv = (int32_t)v - 128;
    ruv = (1 << (ITUR_BT_601_SHIFT - 1)) + ITUR_BT_601_CVR * vv;
    guv = (1 << (ITUR_BT_601_SHIFT - 1)) + ITUR_BT_601_CVG * vv + ITUR_BT_601_CUG * uu;
    buv = (1 << (ITUR_BT_601_SHIFT - 1)) + ITUR_BT_601_CUB * uu;

    tmp_y0 = (int32_t)y0;
    y = MAX((tmp_y0 - 16), 0) * ITUR_BT_601_CY;

    r0 = (uint8_t)NDSLIB_SATU(((y+ruv) >> ITUR_BT_601_SHIFT), 8u);
    g0 = (uint8_t)NDSLIB_SATU(((y+guv) >> ITUR_BT_601_SHIFT), 8u);
    b0 = (uint8_t)NDSLIB_SATU(((y+buv) >> ITUR_BT_601_SHIFT), 8u);
    a0 = 0xff;

    tmp_y1 = (int32_t)y1;
    y = MAX((tmp_y1 -16), 0)*ITUR_BT_601_CY;
    r1 = (uint8_t)NDSLIB_SATU(((y+ruv) >> ITUR_BT_601_SHIFT), 8u);
    g1 = (uint8_t)NDSLIB_SATU(((y+guv) >> ITUR_BT_601_SHIFT), 8u);
    b1 = (uint8_t)NDSLIB_SATU(((y+buv) >> ITUR_BT_601_SHIFT), 8u);
    a1 = 0xff;

    dst[2-bIdx] = r0;
    dst[1]      = g0;
    dst[bIdx]   = b0;
    if(dcn == 4)
    {
        dst[3] = a0;
    }
    dst[dcn+2-bIdx] = r1;
    dst[dcn+1]      = g1;
    dst[dcn+0+bIdx] = b1;
    if(dcn == 4)
    {
        dst[7] = a1;
    }

}
#endif
