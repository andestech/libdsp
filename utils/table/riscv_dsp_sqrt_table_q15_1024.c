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
 * sqrtgent look-up table:
 * riscv_dsp_sqrt_table_q15_1024[k] = sqrt(k / 1024)
 * where 0 <= k <= 1024.
 **/

const q15_t riscv_dsp_sqrt_table_q15_1024[1025] =
{
    0x0200, 0x0400, 0x05a8, 0x06ee, 0x0800, 0x08f2, 0x09cc, 0x0a95,
    0x0b50, 0x0c00, 0x0ca6, 0x0d44, 0x0ddb, 0x0e6c, 0x0ef7, 0x0f7e,
    0x1000, 0x107e, 0x10f8, 0x1170, 0x11e3, 0x1255, 0x12c3, 0x132f,
    0x1399, 0x1400, 0x1465, 0x14c9, 0x152a, 0x158a, 0x15e9, 0x1645,
    0x16a1, 0x16fa, 0x1753, 0x17aa, 0x1800, 0x1855, 0x18a8, 0x18fb,
    0x194c, 0x199d, 0x19ec, 0x1a3b, 0x1a88, 0x1ad5, 0x1b21, 0x1b6c,
    0x1bb6, 0x1c00, 0x1c49, 0x1c91, 0x1cd8, 0x1d1f, 0x1d65, 0x1daa,
    0x1def, 0x1e33, 0x1e77, 0x1eb9, 0x1efc, 0x1f3e, 0x1f7f, 0x1fc0,
    0x2000, 0x2040, 0x207f, 0x20be, 0x20fc, 0x213a, 0x2177, 0x21b4,
    0x21f1, 0x222d, 0x2269, 0x22a4, 0x22df, 0x231a, 0x2354, 0x238e,
    0x23c7, 0x2400, 0x2439, 0x2471, 0x24a9, 0x24e1, 0x2518, 0x254f,
    0x2586, 0x25bc, 0x25f3, 0x2628, 0x265e, 0x2693, 0x26c8, 0x26fd,
    0x2731, 0x2765, 0x2799, 0x27cd, 0x2800, 0x2833, 0x2866, 0x2898,
    0x28cb, 0x28fd, 0x292f, 0x2960, 0x2992, 0x29c3, 0x29f4, 0x2a25,
    0x2a55, 0x2a85, 0x2ab5, 0x2ae5, 0x2b15, 0x2b44, 0x2b73, 0x2ba3,
    0x2bd1, 0x2c00, 0x2c2e, 0x2c5d, 0x2c8b, 0x2cb9, 0x2ce6, 0x2d14,
    0x2d41, 0x2d6e, 0x2d9b, 0x2dc8, 0x2df5, 0x2e21, 0x2e4e, 0x2e7a,
    0x2ea6, 0x2ed2, 0x2efd, 0x2f29, 0x2f54, 0x2f7f, 0x2faa, 0x2fd5,
    0x3000, 0x302b, 0x3055, 0x307f, 0x30a9, 0x30d4, 0x30fd, 0x3127,
    0x3151, 0x317a, 0x31a4, 0x31cd, 0x31f6, 0x321f, 0x3247, 0x3270,
    0x3299, 0x32c1, 0x32e9, 0x3312, 0x333a, 0x3362, 0x3389, 0x33b1,
    0x33d9, 0x3400, 0x3427, 0x344f, 0x3476, 0x349d, 0x34c3, 0x34ea,
    0x3511, 0x3537, 0x355e, 0x3584, 0x35aa, 0x35d1, 0x35f7, 0x361c,
    0x3642, 0x3668, 0x368d, 0x36b3, 0x36d8, 0x36fe, 0x3723, 0x3748,
    0x376d, 0x3792, 0x37b7, 0x37db, 0x3800, 0x3825, 0x3849, 0x386d,
    0x3892, 0x38b6, 0x38da, 0x38fe, 0x3922, 0x3945, 0x3969, 0x398d,
    0x39b0, 0x39d4, 0x39f7, 0x3a1a, 0x3a3e, 0x3a61, 0x3a84, 0x3aa7,
    0x3aca, 0x3aec, 0x3b0f, 0x3b32, 0x3b54, 0x3b77, 0x3b99, 0x3bbc,
    0x3bde, 0x3c00, 0x3c22, 0x3c44, 0x3c66, 0x3c88, 0x3caa, 0x3ccb,
    0x3ced, 0x3d0f, 0x3d30, 0x3d52, 0x3d73, 0x3d94, 0x3db6, 0x3dd7,
    0x3df8, 0x3e19, 0x3e3a, 0x3e5b, 0x3e7b, 0x3e9c, 0x3ebd, 0x3edd,
    0x3efe, 0x3f1e, 0x3f3f, 0x3f5f, 0x3f7f, 0x3fa0, 0x3fc0, 0x3fe0,
    0x4000, 0x4020, 0x4040, 0x4060, 0x4080, 0x409f, 0x40bf, 0x40de,
    0x40fe, 0x411e, 0x413d, 0x415c, 0x417c, 0x419b, 0x41ba, 0x41d9,
    0x41f8, 0x4217, 0x4236, 0x4255, 0x4274, 0x4293, 0x42b1, 0x42d0,
    0x42ef, 0x430d, 0x432c, 0x434a, 0x4369, 0x4387, 0x43a5, 0x43c4,
    0x43e2, 0x4400, 0x441e, 0x443c, 0x445a, 0x4478, 0x4496, 0x44b4,
    0x44d2, 0x44ef, 0x450d, 0x452b, 0x4548, 0x4566, 0x4583, 0x45a1,
    0x45be, 0x45db, 0x45f9, 0x4616, 0x4633, 0x4650, 0x466d, 0x468a,
    0x46a7, 0x46c4, 0x46e1, 0x46fe, 0x471b, 0x4738, 0x4755, 0x4771,
    0x478e, 0x47aa, 0x47c7, 0x47e4, 0x4800, 0x481c, 0x4839, 0x4855,
    0x4871, 0x488e, 0x48aa, 0x48c6, 0x48e2, 0x48fe, 0x491a, 0x4936,
    0x4952, 0x496e, 0x498a, 0x49a6, 0x49c2, 0x49dd, 0x49f9, 0x4a15,
    0x4a30, 0x4a4c, 0x4a68, 0x4a83, 0x4a9e, 0x4aba, 0x4ad5, 0x4af1,
    0x4b0c, 0x4b27, 0x4b42, 0x4b5e, 0x4b79, 0x4b94, 0x4baf, 0x4bca,
    0x4be5, 0x4c00, 0x4c1b, 0x4c36, 0x4c51, 0x4c6b, 0x4c86, 0x4ca1,
    0x4cbc, 0x4cd6, 0x4cf1, 0x4d0c, 0x4d26, 0x4d41, 0x4d5b, 0x4d76,
    0x4d90, 0x4daa, 0x4dc5, 0x4ddf, 0x4df9, 0x4e14, 0x4e2e, 0x4e48,
    0x4e62, 0x4e7c, 0x4e96, 0x4eb0, 0x4eca, 0x4ee4, 0x4efe, 0x4f18,
    0x4f32, 0x4f4c, 0x4f66, 0x4f80, 0x4f99, 0x4fb3, 0x4fcd, 0x4fe6,
    0x5000, 0x501a, 0x5033, 0x504d, 0x5066, 0x5080, 0x5099, 0x50b2,
    0x50cc, 0x50e5, 0x50fe, 0x5118, 0x5131, 0x514a, 0x5163, 0x517c,
    0x5196, 0x51af, 0x51c8, 0x51e1, 0x51fa, 0x5213, 0x522c, 0x5245,
    0x525d, 0x5276, 0x528f, 0x52a8, 0x52c1, 0x52d9, 0x52f2, 0x530b,
    0x5323, 0x533c, 0x5355, 0x536d, 0x5386, 0x539e, 0x53b7, 0x53cf,
    0x53e8, 0x5400, 0x5418, 0x5431, 0x5449, 0x5461, 0x547a, 0x5492,
    0x54aa, 0x54c2, 0x54da, 0x54f2, 0x550b, 0x5523, 0x553b, 0x5553,
    0x556b, 0x5583, 0x559b, 0x55b2, 0x55ca, 0x55e2, 0x55fa, 0x5612,
    0x562a, 0x5641, 0x5659, 0x5671, 0x5689, 0x56a0, 0x56b8, 0x56cf,
    0x56e7, 0x56ff, 0x5716, 0x572e, 0x5745, 0x575c, 0x5774, 0x578b,
    0x57a3, 0x57ba, 0x57d1, 0x57e9, 0x5800, 0x5817, 0x582e, 0x5846,
    0x585d, 0x5874, 0x588b, 0x58a2, 0x58b9, 0x58d0, 0x58e8, 0x58ff,
    0x5916, 0x592d, 0x5943, 0x595a, 0x5971, 0x5988, 0x599f, 0x59b6,
    0x59cd, 0x59e4, 0x59fa, 0x5a11, 0x5a28, 0x5a3e, 0x5a55, 0x5a6c,
    0x5a82, 0x5a99, 0x5ab0, 0x5ac6, 0x5add, 0x5af3, 0x5b0a, 0x5b20,
    0x5b37, 0x5b4d, 0x5b64, 0x5b7a, 0x5b90, 0x5ba7, 0x5bbd, 0x5bd3,
    0x5bea, 0x5c00, 0x5c16, 0x5c2c, 0x5c43, 0x5c59, 0x5c6f, 0x5c85,
    0x5c9b, 0x5cb1, 0x5cc8, 0x5cde, 0x5cf4, 0x5d0a, 0x5d20, 0x5d36,
    0x5d4c, 0x5d62, 0x5d77, 0x5d8d, 0x5da3, 0x5db9, 0x5dcf, 0x5de5,
    0x5dfb, 0x5e10, 0x5e26, 0x5e3c, 0x5e52, 0x5e67, 0x5e7d, 0x5e93,
    0x5ea8, 0x5ebe, 0x5ed3, 0x5ee9, 0x5eff, 0x5f14, 0x5f2a, 0x5f3f,
    0x5f55, 0x5f6a, 0x5f80, 0x5f95, 0x5fab, 0x5fc0, 0x5fd5, 0x5feb,
    0x6000, 0x6015, 0x602b, 0x6040, 0x6055, 0x606a, 0x6080, 0x6095,
    0x60aa, 0x60bf, 0x60d4, 0x60ea, 0x60ff, 0x6114, 0x6129, 0x613e,
    0x6153, 0x6168, 0x617d, 0x6192, 0x61a7, 0x61bc, 0x61d1, 0x61e6,
    0x61fb, 0x6210, 0x6225, 0x6239, 0x624e, 0x6263, 0x6278, 0x628d,
    0x62a1, 0x62b6, 0x62cb, 0x62e0, 0x62f4, 0x6309, 0x631e, 0x6332,
    0x6347, 0x635c, 0x6370, 0x6385, 0x6399, 0x63ae, 0x63c2, 0x63d7,
    0x63ec, 0x6400, 0x6414, 0x6429, 0x643d, 0x6452, 0x6466, 0x647b,
    0x648f, 0x64a3, 0x64b8, 0x64cc, 0x64e0, 0x64f5, 0x6509, 0x651d,
    0x6531, 0x6546, 0x655a, 0x656e, 0x6582, 0x6596, 0x65ab, 0x65bf,
    0x65d3, 0x65e7, 0x65fb, 0x660f, 0x6623, 0x6637, 0x664b, 0x665f,
    0x6673, 0x6687, 0x669b, 0x66af, 0x66c3, 0x66d7, 0x66eb, 0x66ff,
    0x6713, 0x6726, 0x673a, 0x674e, 0x6762, 0x6776, 0x678a, 0x679d,
    0x67b1, 0x67c5, 0x67d9, 0x67ec, 0x6800, 0x6814, 0x6827, 0x683b,
    0x684f, 0x6862, 0x6876, 0x6889, 0x689d, 0x68b1, 0x68c4, 0x68d8,
    0x68eb, 0x68ff, 0x6912, 0x6926, 0x6939, 0x694d, 0x6960, 0x6974,
    0x6987, 0x699a, 0x69ae, 0x69c1, 0x69d4, 0x69e8, 0x69fb, 0x6a0e,
    0x6a22, 0x6a35, 0x6a48, 0x6a5c, 0x6a6f, 0x6a82, 0x6a95, 0x6aa9,
    0x6abc, 0x6acf, 0x6ae2, 0x6af5, 0x6b08, 0x6b1c, 0x6b2f, 0x6b42,
    0x6b55, 0x6b68, 0x6b7b, 0x6b8e, 0x6ba1, 0x6bb4, 0x6bc7, 0x6bda,
    0x6bed, 0x6c00, 0x6c13, 0x6c26, 0x6c39, 0x6c4c, 0x6c5f, 0x6c72,
    0x6c84, 0x6c97, 0x6caa, 0x6cbd, 0x6cd0, 0x6ce3, 0x6cf5, 0x6d08,
    0x6d1b, 0x6d2e, 0x6d41, 0x6d53, 0x6d66, 0x6d79, 0x6d8b, 0x6d9e,
    0x6db1, 0x6dc3, 0x6dd6, 0x6de9, 0x6dfb, 0x6e0e, 0x6e21, 0x6e33,
    0x6e46, 0x6e58, 0x6e6b, 0x6e7d, 0x6e90, 0x6ea2, 0x6eb5, 0x6ec7,
    0x6eda, 0x6eec, 0x6eff, 0x6f11, 0x6f24, 0x6f36, 0x6f49, 0x6f5b,
    0x6f6d, 0x6f80, 0x6f92, 0x6fa4, 0x6fb7, 0x6fc9, 0x6fdb, 0x6fee,
    0x7000, 0x7012, 0x7025, 0x7037, 0x7049, 0x705b, 0x706e, 0x7080,
    0x7092, 0x70a4, 0x70b6, 0x70c8, 0x70db, 0x70ed, 0x70ff, 0x7111,
    0x7123, 0x7135, 0x7147, 0x7159, 0x716b, 0x717d, 0x7190, 0x71a2,
    0x71b4, 0x71c6, 0x71d8, 0x71ea, 0x71fc, 0x720d, 0x721f, 0x7231,
    0x7243, 0x7255, 0x7267, 0x7279, 0x728b, 0x729d, 0x72af, 0x72c0,
    0x72d2, 0x72e4, 0x72f6, 0x7308, 0x731a, 0x732b, 0x733d, 0x734f,
    0x7361, 0x7372, 0x7384, 0x7396, 0x73a8, 0x73b9, 0x73cb, 0x73dd,
    0x73ee, 0x7400, 0x7412, 0x7423, 0x7435, 0x7447, 0x7458, 0x746a,
    0x747b, 0x748d, 0x749e, 0x74b0, 0x74c2, 0x74d3, 0x74e5, 0x74f6,
    0x7508, 0x7519, 0x752b, 0x753c, 0x754e, 0x755f, 0x7570, 0x7582,
    0x7593, 0x75a5, 0x75b6, 0x75c8, 0x75d9, 0x75ea, 0x75fc, 0x760d,
    0x761e, 0x7630, 0x7641, 0x7652, 0x7664, 0x7675, 0x7686, 0x7697,
    0x76a9, 0x76ba, 0x76cb, 0x76dc, 0x76ee, 0x76ff, 0x7710, 0x7721,
    0x7733, 0x7744, 0x7755, 0x7766, 0x7777, 0x7788, 0x7799, 0x77ab,
    0x77bc, 0x77cd, 0x77de, 0x77ef, 0x7800, 0x7811, 0x7822, 0x7833,
    0x7844, 0x7855, 0x7866, 0x7877, 0x7888, 0x7899, 0x78aa, 0x78bb,
    0x78cc, 0x78dd, 0x78ee, 0x78ff, 0x7910, 0x7921, 0x7932, 0x7943,
    0x7953, 0x7964, 0x7975, 0x7986, 0x7997, 0x79a8, 0x79b9, 0x79c9,
    0x79da, 0x79eb, 0x79fc, 0x7a0d, 0x7a1d, 0x7a2e, 0x7a3f, 0x7a50,
    0x7a60, 0x7a71, 0x7a82, 0x7a93, 0x7aa3, 0x7ab4, 0x7ac5, 0x7ad5,
    0x7ae6, 0x7af7, 0x7b07, 0x7b18, 0x7b29, 0x7b39, 0x7b4a, 0x7b5a,
    0x7b6b, 0x7b7c, 0x7b8c, 0x7b9d, 0x7bad, 0x7bbe, 0x7bce, 0x7bdf,
    0x7bef, 0x7c00, 0x7c11, 0x7c21, 0x7c32, 0x7c42, 0x7c52, 0x7c63,
    0x7c73, 0x7c84, 0x7c94, 0x7ca5, 0x7cb5, 0x7cc6, 0x7cd6, 0x7ce6,
    0x7cf7, 0x7d07, 0x7d18, 0x7d28, 0x7d38, 0x7d49, 0x7d59, 0x7d69,
    0x7d7a, 0x7d8a, 0x7d9a, 0x7dab, 0x7dbb, 0x7dcb, 0x7ddb, 0x7dec,
    0x7dfc, 0x7e0c, 0x7e1c, 0x7e2d, 0x7e3d, 0x7e4d, 0x7e5d, 0x7e6e,
    0x7e7e, 0x7e8e, 0x7e9e, 0x7eae, 0x7ebe, 0x7ecf, 0x7edf, 0x7eef,
    0x7eff, 0x7f0f, 0x7f1f, 0x7f2f, 0x7f3f, 0x7f50, 0x7f60, 0x7f70,
    0x7f80, 0x7f90, 0x7fa0, 0x7fb0, 0x7fc0, 0x7fd0, 0x7fe0, 0x7ff0,
    0x7fff
};

