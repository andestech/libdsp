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
#ifndef __INTERNAL_SVM_MATH_H__
#define __INTERNAL_SVM_MATH_H__

#include "riscv_dsp_sort_math.h"  //for struct types
#include <string.h>               //for memcpy

void riscv_dsp_sort_bitonic_f32(const riscv_dsp_sort_f32_t * instance, float32_t * src, float32_t * dst, uint32_t size);
void riscv_dsp_sort_bubble_f32(const riscv_dsp_sort_f32_t * instance, float32_t * src, float32_t * dst, uint32_t size);
void riscv_dsp_sort_heap_f32(const riscv_dsp_sort_f32_t * instance, float32_t * src, float32_t * dst, uint32_t size);
void riscv_dsp_sort_insertion_f32(const riscv_dsp_sort_f32_t * instance, float32_t * src, float32_t * dst, uint32_t size);
void riscv_dsp_sort_quick_f32(const riscv_dsp_sort_f32_t * instance, float32_t * src, float32_t * dst, uint32_t size);
void riscv_dsp_sort_selection_f32(const riscv_dsp_sort_f32_t * instance, float32_t * src, float32_t * dst, uint32_t size);

#endif // __INTERNAL_SVM_MATH_H__
