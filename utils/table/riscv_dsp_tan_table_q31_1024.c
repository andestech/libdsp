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

#include "internal_math_types.h"

/**
 * tangent look-up table:
 * tan(x) where 0 <= x <= PI / 2
 * riscv_dsp_tan_table_q31_1024[k] = tan(2 * PI * k / 1024)
 * where 0 <= k <= 256.
 */

const q31_t riscv_dsp_tan_table_q31_1024[257] =
{
    0x00000000, 0x00003244, 0x00006489, 0x000096d0,
    0x0000c91a, 0x0000fb68, 0x00012dbb, 0x00016013,
    0x00019272, 0x0001c4da, 0x0001f749, 0x000229c3,
    0x00025c47, 0x00028ed7, 0x0002c174, 0x0002f41e,
    0x000326d7, 0x000359a0, 0x00038c7a, 0x0003bf65,
    0x0003f263, 0x00042575, 0x0004589b, 0x00048bd8,
    0x0004bf2b, 0x0004f296, 0x0005261b, 0x000559b9,
    0x00058d72, 0x0005c148, 0x0005f53b, 0x0006294d,
    0x00065d7d, 0x000691cf, 0x0006c642, 0x0006fad9,
    0x00072f93, 0x00076473, 0x00079979, 0x0007cea7,
    0x000803fd, 0x0008397e, 0x00086f2a, 0x0008a502,
    0x0008db08, 0x0009113d, 0x000947a3, 0x00097e3a,
    0x0009b504, 0x0009ec03, 0x000a2337, 0x000a5aa2,
    0x000a9245, 0x000aca23, 0x000b023c, 0x000b3a91,
    0x000b7325, 0x000babf8, 0x000be50d, 0x000c1e65,
    0x000c5801, 0x000c91e3, 0x000ccc0c, 0x000d067f,
    0x000d413d, 0x000d7c47, 0x000db7a0, 0x000df349,
    0x000e2f44, 0x000e6b92, 0x000ea836, 0x000ee532,
    0x000f2287, 0x000f6038, 0x000f9e45, 0x000fdcb3,
    0x00101b82, 0x00105ab4, 0x00109a4c, 0x0010da4d,
    0x00111ab7, 0x00115b8e, 0x00119cd4, 0x0011de8b,
    0x001220b6, 0x00126357, 0x0012a671, 0x0012ea06,
    0x00132e19, 0x001372ac, 0x0013b7c3, 0x0013fd60,
    0x00144387, 0x00148a39, 0x0014d17b, 0x0015194f,
    0x001561b8, 0x0015aaba, 0x0015f458, 0x00163e95,
    0x00168975, 0x0016d4fb, 0x0017212c, 0x00176e0a,
    0x0017bb9a, 0x001809df, 0x001858de, 0x0018a89b,
    0x0018f919, 0x00194a5e, 0x00199c6e, 0x0019ef4d,
    0x001a4300, 0x001a978c, 0x001aecf6, 0x001b4343,
    0x001b9a78, 0x001bf29a, 0x001c4bb0, 0x001ca5bf,
    0x001d00cc, 0x001d5cde, 0x001db9fb, 0x001e1829,
    0x001e776f, 0x001ed7d3, 0x001f395e, 0x001f9c15,
    0x00200000, 0x00206527, 0x0020cb92, 0x00213349,
    0x00219c54, 0x002206bb, 0x00227289, 0x0022dfc6,
    0x00234e7b, 0x0023beb2, 0x00243076, 0x0024a3d1,
    0x002518ce, 0x00258f78, 0x002607db, 0x00268202,
    0x0026fdfb, 0x00277bd2, 0x0027fb95, 0x00287d50,
    0x00290114, 0x002986ee, 0x002a0eee, 0x002a9925,
    0x002b25a2, 0x002bb477, 0x002c45b7, 0x002cd973,
    0x002d6fbf, 0x002e08b0, 0x002ea459, 0x002f42d3,
    0x002fe432, 0x0030888f, 0x00313002, 0x0031daa6,
    0x00328895, 0x003339eb, 0x0033eec6, 0x0034a745,
    0x00356387, 0x003623ae, 0x0036e7dc, 0x0037b038,
    0x00387ce7, 0x00394e11, 0x003a23e2, 0x003afe84,
    0x003bde27, 0x003cc2fc, 0x003dad37, 0x003e9d0c,
    0x003f92b5, 0x00408e6f, 0x00419078, 0x00429914,
    0x0043a887, 0x0044bf1e, 0x0045dd26, 0x004702f2,
    0x004830db, 0x0049673e, 0x004aa67d, 0x004bef02,
    0x004d413d, 0x004e9da3, 0x005004b4, 0x005176f5,
    0x0052f4f6, 0x00547f51, 0x005616a9, 0x0057bbae,
    0x00596f1b, 0x005b31ba, 0x005d0464, 0x005ee800,
    0x0060dd8a, 0x0062e60f, 0x006502b5, 0x006734b6,
    0x00697d68, 0x006bde3e, 0x006e58cd, 0x0070eeca,
    0x0073a216, 0x007674bb, 0x007968f8, 0x007c8141,
    0x007fc04c, 0x00832914, 0x0086bee4, 0x008a8564,
    0x008e80a2, 0x0092b524, 0x009727f9, 0x009bdeca,
    0x00a0dff7, 0x00a632b4, 0x00abdf27, 0x00b1ee9b,
    0x00b86ba9, 0x00bf6280, 0x00c6e12e, 0x00cef800,
    0x00d7b9fa, 0x00e13d71, 0x00eb9cc7, 0x00f6f764,
    0x010372fb, 0x01113d2d, 0x01208dbe, 0x0131a992,
    0x0144e6c6, 0x015ab26d, 0x017398c5, 0x01905133,
    0x01b1d03c, 0x01d96350, 0x0208dd40, 0x0242e055,
    0x028b5ffe, 0x02e8919e, 0x0364ce33, 0x0412b5e8,
    0x05178915, 0x06ca3336, 0x0a2f76b4, 0x145f1fac,
    0x7fffffff
};
