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

#ifndef __RISCV_DSP_QUATERNION_H__
#define __RISCV_DSP_QUATERNION_H__

#ifdef  __cplusplus
extern "C"
{
#endif

#include "riscv_dsp_math_types.h"

/**
   @brief Conversion of quaternion to equivalent rotation matrix.
   @param[in]       src pointer of the input quaternions vector
   @param[out]      dst pointer of the output 3x3 rotations matrix
   @param[in]       size number of quaternions in a vector
   @return none.
  */
void riscv_dsp_quaternion2rotation_f32(const float32_t *src, float32_t *dst, uint32_t size);

/**
 * @brief Conversion of a rotation matrix to an equivalent quaternion.
 * @param[in]       src points to an array 3x3 rotation matrix (in row order)
 * @param[out]      dst points to an array quaternions
 * @param[in]       size number of quaternions in the array
 * @return none.
 */
void riscv_dsp_rotation2quaternion_f32(const float32_t *src, float32_t *dst, uint32_t size);

/**
  @brief         Floating-point quaternion conjugates.
  @param[in]     src        points to the input vector of quaternions
  @param[out]    dst        points to the output vector of conjugate quaternions
  @param[in]     size       number of quaternions in each vector
  @return        none
 */
void riscv_dsp_quaternion_conj_f32(const float32_t *src, float32_t *dst, uint32_t size);

/**
  @brief         Floating-point quaternion inverse.
  @param[in]     src            points to the input vector of quaternions
  @param[out]    dst          points to the output vector of inverse quaternions
  @param[in]     size                number of quaternions in each vector
  @return        none
 */
void riscv_dsp_quaternion_inv_f32(const float32_t *src, float32_t *dst, uint32_t size);

/**
  @brief         Floating-point quaternion length.
  @param[in]     src       points to the input vector of quaternions
  @param[out]    dst                  points to the output vector of norms
  @param[in]     size           number of quaternions in the input vector
  @return        none
 */
void riscv_dsp_quaternion_length_f32(const float32_t *src, float32_t *dst, uint32_t size);

/**
  @brief         Floating-point normalization of quaternions.
  @param[in]     src            points to the input vector of quaternions
  @param[out]    dst       points to the output vector of normalized quaternions
  @param[in]     size                number of quaternions in each vector
  @return        none
 */
void riscv_dsp_quaternion_normalize_f32(const float32_t *src, float32_t *dst, uint32_t size);

/**
  @brief         Floating-point product of two quaternions.
  @param[in]     src1       first quaternion
  @param[in]     src2       second quaternion
  @param[out]    dst       product of two quaternions
  @return        none
 */
void riscv_dsp_quaternion_product_single_f32(const float32_t *src1, const float32_t *src2, float32_t *dst);

/**
  @brief         Floating-point elementwise product two quaternions.
  @param[in]     src1                  first array of quaternions
  @param[in]     src2                  second array of quaternions
  @param[out]    dst                   elementwise product of quaternions
  @param[in]     size       number of quaternions in the array
  @return        none
 */
void riscv_dsp_quaternion_product_f32(const float32_t *src1, const float32_t *src2, float32_t *dst, uint32_t size);

#endif // __RISCV_DSP_QUATERNION_H__
