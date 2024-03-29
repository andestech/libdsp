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
 * riscv_dsp_tan_table_q31_2048[k] = tan(2 * PI * k / 2048)
 * where 0 <= k <= 512.
 */

const q31_t riscv_dsp_tan_table_q31_2048[513] =
{
    0x00000000, 0x00000c91, 0x00001922, 0x000025b3,
    0x00003245, 0x00003ed6, 0x00004b68, 0x000057fa,
    0x0000648d, 0x00007120, 0x00007db4, 0x00008a48,
    0x000096dd, 0x0000a373, 0x0000b00a, 0x0000bca1,
    0x0000c939, 0x0000d5d2, 0x0000e26d, 0x0000ef08,
    0x0000fba5, 0x00010842, 0x000114e1, 0x00012182,
    0x00012e24, 0x00013ac7, 0x0001476c, 0x00015412,
    0x000160ba, 0x00016d64, 0x00017a0f, 0x000186bc,
    0x0001936c, 0x0001a01d, 0x0001acd0, 0x0001b985,
    0x0001c63d, 0x0001d2f7, 0x0001dfb3, 0x0001ec71,
    0x0001f932, 0x000205f5, 0x000212ba, 0x00021f83,
    0x00022c4e, 0x0002391b, 0x000245ec, 0x000252bf,
    0x00025f96, 0x00026c6f, 0x0002794b, 0x0002862b,
    0x0002930d, 0x00029ff3, 0x0002acdc, 0x0002b9c9,
    0x0002c6b9, 0x0002d3ad, 0x0002e0a4, 0x0002ed9f,
    0x0002fa9e, 0x000307a0, 0x000314a6, 0x000321b0,
    0x00032ebf, 0x00033bd1, 0x000348e8, 0x00035602,
    0x00036321, 0x00037045, 0x00037d6c, 0x00038a99,
    0x000397ca, 0x0003a4ff, 0x0003b239, 0x0003bf79,
    0x0003ccbc, 0x0003da05, 0x0003e753, 0x0003f4a6,
    0x000401ff, 0x00040f5c, 0x00041cbf, 0x00042a27,
    0x00043795, 0x00044508, 0x00045281, 0x00046000,
    0x00046d84, 0x00047b0e, 0x0004889f, 0x00049635,
    0x0004a3d1, 0x0004b174, 0x0004bf1d, 0x0004cccc,
    0x0004da82, 0x0004e83e, 0x0004f601, 0x000503cb,
    0x0005119b, 0x00051f73, 0x00052d51, 0x00053b36,
    0x00054923, 0x00055716, 0x00056511, 0x00057314,
    0x0005811e, 0x00058f2f, 0x00059d49, 0x0005ab6a,
    0x0005b992, 0x0005c7c3, 0x0005d5fc, 0x0005e43d,
    0x0005f287, 0x000600d8, 0x00060f32, 0x00061d95,
    0x00062c00, 0x00063a74, 0x000648f1, 0x00065777,
    0x00066606, 0x0006749e, 0x00068340, 0x000691ea,
    0x0006a09e, 0x0006af5c, 0x0006be24, 0x0006ccf5,
    0x0006dbd0, 0x0006eab5, 0x0006f9a4, 0x0007089e,
    0x000717a2, 0x000726b0, 0x000735c9, 0x000744ed,
    0x0007541b, 0x00076355, 0x00077299, 0x000781e9,
    0x00079144, 0x0007a0aa, 0x0007b01c, 0x0007bf99,
    0x0007cf23, 0x0007deb8, 0x0007ee59, 0x0007fe07,
    0x00080dc1, 0x00081d87, 0x00082d5a, 0x00083d3a,
    0x00084d26, 0x00085d20, 0x00086d26, 0x00087d3a,
    0x00088d5c, 0x00089d8a, 0x0008adc7, 0x0008be12,
    0x0008ce6a, 0x0008ded1, 0x0008ef46, 0x0008ffc9,
    0x0009105b, 0x000920fc, 0x000931ab, 0x0009426a,
    0x00095338, 0x00096416, 0x00097503, 0x00098600,
    0x0009970c, 0x0009a829, 0x0009b956, 0x0009ca94,
    0x0009dbe2, 0x0009ed40, 0x0009feb0, 0x000a1031,
    0x000a21c3, 0x000a3367, 0x000a451d, 0x000a56e4,
    0x000a68be, 0x000a7aa9, 0x000a8ca7, 0x000a9eb8,
    0x000ab0dc, 0x000ac313, 0x000ad55d, 0x000ae7bb,
    0x000afa2c, 0x000b0cb1, 0x000b1f4b, 0x000b31f8,
    0x000b44bb, 0x000b5792, 0x000b6a7e, 0x000b7d7f,
    0x000b9096, 0x000ba3c3, 0x000bb705, 0x000bca5e,
    0x000bddcd, 0x000bf153, 0x000c04f0, 0x000c18a4,
    0x000c2c6f, 0x000c4052, 0x000c544d, 0x000c6861,
    0x000c7c8d, 0x000c90d1, 0x000ca52f, 0x000cb9a6,
    0x000cce37, 0x000ce2e2, 0x000cf7a7, 0x000d0c86,
    0x000d2180, 0x000d3695, 0x000d4bc6, 0x000d6113,
    0x000d767b, 0x000d8c00, 0x000da1a1, 0x000db760,
    0x000dcd3c, 0x000de336, 0x000df94d, 0x000e0f83,
    0x000e25d8, 0x000e3c4c, 0x000e52df, 0x000e6992,
    0x000e8066, 0x000e975a, 0x000eae6f, 0x000ec5a5,
    0x000edcfd, 0x000ef478, 0x000f0c14, 0x000f23d4,
    0x000f3bb7, 0x000f53be, 0x000f6bea, 0x000f843a,
    0x000f9caf, 0x000fb54a, 0x000fce0a, 0x000fe6f2,
    0x00100000, 0x00101936, 0x00103294, 0x00104c1a,
    0x001065c9, 0x00107fa2, 0x001099a4, 0x0010b3d2,
    0x0010ce2a, 0x0010e8ae, 0x0011035e, 0x00111e3a,
    0x00113944, 0x0011547c, 0x00116fe3, 0x00118b78,
    0x0011a73d, 0x0011c333, 0x0011df59, 0x0011fbb1,
    0x0012183b, 0x001234f8, 0x001251e9, 0x00126f0d,
    0x00128c67, 0x0012a9f6, 0x0012c7bc, 0x0012e5b9,
    0x001303ed, 0x0013225b, 0x00134101, 0x00135fe2,
    0x00137efe, 0x00139e55, 0x0013bde9, 0x0013ddba,
    0x0013fdca, 0x00141e19, 0x00143ea8, 0x00145f78,
    0x0014808a, 0x0014a1df, 0x0014c377, 0x0014e554,
    0x00150777, 0x001529e1, 0x00154c92, 0x00156f8d,
    0x001592d1, 0x0015b660, 0x0015da3c, 0x0015fe64,
    0x001622db, 0x001647a2, 0x00166cb9, 0x00169223,
    0x0016b7e0, 0x0016ddf1, 0x00170458, 0x00172b16,
    0x0017522d, 0x0017799d, 0x0017a169, 0x0017c992,
    0x0017f219, 0x00181aff, 0x00184447, 0x00186df2,
    0x00189801, 0x0018c276, 0x0018ed53, 0x00191899,
    0x0019444a, 0x00197069, 0x00199cf6, 0x0019c9f3,
    0x0019f763, 0x001a2548, 0x001a53a2, 0x001a8276,
    0x001ab1c3, 0x001ae18e, 0x001b11d7, 0x001b42a1,
    0x001b73ee, 0x001ba5c1, 0x001bd81c, 0x001c0b01,
    0x001c3e74, 0x001c7275, 0x001ca709, 0x001cdc31,
    0x001d11f1, 0x001d484b, 0x001d7f42, 0x001db6d9,
    0x001def14, 0x001e27f4, 0x001e617e, 0x001e9bb5,
    0x001ed69b, 0x001f1235, 0x001f4e86, 0x001f8b91,
    0x001fc95b, 0x002007e6, 0x00204738, 0x00208753,
    0x0020c83c, 0x002109f8, 0x00214c8a, 0x00218ff7,
    0x0021d444, 0x00221975, 0x00225f8f, 0x0022a697,
    0x0022ee93, 0x00233787, 0x00238179, 0x0023cc6f,
    0x0024186e, 0x0024657c, 0x0024b39f, 0x002502de,
    0x0025533f, 0x0025a4c8, 0x0025f781, 0x00264b71,
    0x0026a09e, 0x0026f711, 0x00274ed2, 0x0027a7e7,
    0x0028025a, 0x00285e33, 0x0028bb7a, 0x00291a3a,
    0x00297a7b, 0x0029dc47, 0x002a3fa9, 0x002aa4a9,
    0x002b0b55, 0x002b73b5, 0x002bddd7, 0x002c49c6,
    0x002cb78e, 0x002d273c, 0x002d98dd, 0x002e0c80,
    0x002e8232, 0x002efa02, 0x002f7400, 0x002ff03b,
    0x00306ec5, 0x0030efae, 0x00317308, 0x0031f8e5,
    0x0032815a, 0x00330c7a, 0x00339a5b, 0x00342b11,
    0x0034beb4, 0x0035555b, 0x0035ef1f, 0x00368c1a,
    0x00372c66, 0x0037d020, 0x00387765, 0x00392253,
    0x0039d10b, 0x003a83ad, 0x003b3a5d, 0x003bf540,
    0x003cb47c, 0x003d7839, 0x003e40a1, 0x003f0de0,
    0x003fe026, 0x0040b7a3, 0x0041948a, 0x00427711,
    0x00435f72, 0x00444de8, 0x004542b2, 0x00463e13,
    0x00474051, 0x004849b7, 0x00495a92, 0x004a7337,
    0x004b93fc, 0x004cbd40, 0x004def65, 0x004f2ad3,
    0x00506ffc, 0x0051bf54, 0x0053195a, 0x00547e95,
    0x0055ef94, 0x00576cf0, 0x0058f74d, 0x005a8f5b,
    0x005c35d4, 0x005deb83, 0x005fb140, 0x006187f3,
    0x00637097, 0x00656c3a, 0x00677c00, 0x0069a124,
    0x006bdcfd, 0x006e30fd, 0x00709eb9, 0x007327e5,
    0x0075ce63, 0x0078943e, 0x007b7bb2, 0x007e8736,
    0x0081b97e, 0x00851585, 0x00889e96, 0x008c585a,
    0x009046df, 0x00946eaa, 0x0098d4c9, 0x009d7ee7,
    0x00a27363, 0x00a7b970, 0x00ad5937, 0x00b35bfe,
    0x00b9cc63, 0x00c0b692, 0x00c8289a, 0x00d032c7,
    0x00d8e81e, 0x00e25ef3, 0x00ecb1a8, 0x00f7ffa7,
    0x01046ea0, 0x01122c35, 0x0121702a, 0x01327f64,
    0x0145afff, 0x015b6f0e, 0x017448cf, 0x0190f4a6,
    0x01b26719, 0x01d9ed98, 0x02095af4, 0x02435175,
    0x028bc48b, 0x02e8e999, 0x0365199b, 0x0412f4be,
    0x0517bb5a, 0x06ca58e9, 0x0a2f8fd6, 0x145f2c3d,
    0x7fffffff
};
