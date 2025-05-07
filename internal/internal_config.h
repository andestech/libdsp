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

/**
 * @defgroup libdsp config header file
 */

/**
 * This header file will be included for each libdsp's C file
 * with while compiling. So any configuration or common
 * definition can be written here.
 */

#ifndef __INTERNAL_CONFIG_H__
#define __INTERNAL_CONFIG_H__

#ifdef  __cplusplus
extern "C"
{
#endif

#include "internal_nds_config.h"
#include "riscv_dsp_math_types.h"

// Using taylor_poly_f32 to reduce cycle for optimize performance.
// MAXDIFF is 0.00000048 compare to math library
#define ENA_HIGHER_PERFORMANCE_VEXP_F32

// Using taylor_poly_f32 to reduce cycle for optimize performance.
// MAXDIFF is  0.00000262 compare to math library
#define ENA_HIGHER_PERFORMANCE_VLOG_F32

// Option for exp, log, sogmoid, cos, sin f16 functions
#define ENA_HIGH_ACCURACY_F16

//Unroll the nstage to reduce cycle for optimize performance.
#define ENA_HIGHER_PERFORMANCE_LIIR_F32

//For all FIR function
//Use unroll the coefficient using to reduce LW for optimize performance.
#define ENA_HIGHER_PERFORMANCE_FIR

//Use nonsaturation alogrithm for n10
//If "size" less than ((2^8)-1), there is no risk for overflow.
#define ENA_HIGHER_PERFORMANCE_DPROD_U8XQ15

//Use less cycles to calc atan2_q31 (from 848 to 268 cycles)
//Otherwise calc more precicsion.
#define ENA_HIGHER_PERFORMANCE_ATAN2_Q31

//Use higher accuracy for atan2_f32 ifdef "NDS32_EXT_FPU_SP"
#if defined(__NDS32_EXT_FPU_SP__) || defined(__riscv_float_abi_single) || defined(__riscv_float_abi_double)
#define ENA_LUT_ATAN
#endif

//Use higher performance but bigger size compare with Keil"
#define ENA_HIGHER_PERFORMANCE_MAX_AND_MIN

//Using special function to process F32 calulation by usign integer ALU.
#if !defined(__NDS32_EXT_FPU_SP__) && !defined(__riscv_float_abi_single) && !defined(__riscv_float_abi_double)
//#define ENA_HIGHER_PERFORMANCE_F32_HMT
#endif

// Use higher performance but lower accuracy for scale_q31
#define ENA_HIGHER_PERFORMANCE_SCALE_Q31

//Using not checking inf, -inf, NAN for higher performance.
#define ENA_HIGHER_PERFORMANCE_CONVERT_F32

//For Changes of CMSIS 1.4.4 (Changed to use linear interpolation)
//For Cos/Sin q31 and q15 functions,
//libdsp still uses Hermite table but change the interpolation to linear instead of Hermite.
//This can help to reduce the cycles but still has better accuracy than CMSIS.
#define ENA_CS_Q_HMT_TBL_LNR_INTP

// Will remove this define after branch merge is all done.
#define ENA_FUNC_RESTRICT
//

#ifdef ENA_FUNC_RESTRICT
    #define FUNC_RESTRICT       __restrict
#else
    #define FUNC_RESTRICT
#endif


// libdsp specific function attribute to using fmax/fmin instead branch
#if defined (ENA_NDS_TOOLCHAIN) && !defined (__clang__)
#define FUNC_ATTR_FFAST_MATH  __attribute__((optimize("-ffast-math")))
#else
#define FUNC_ATTR_FFAST_MATH
#endif

#ifdef  __cplusplus
}
#endif


#endif      //__INTERNAL_CONFIG_H__
