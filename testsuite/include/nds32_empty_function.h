#ifndef __NDS32_EMPTY_FUNCTION_H__
#define __NDS32_EMPTY_FUNCTION_H__
/***************************************************************************
 * nds32_empty_function.h                                                  *
 *                                                                         *
 * Copyright (C) 2012-2016 Andes Technology Corporation                    *
 * All rights reserved.                                                    *
 ***************************************************************************/

/**
 * @defgroup empty function difinitions for conuting code size.
 */

#include "riscv_dsp_math_types.h"

#define ENA_EMPTY_FUNC
#define riscv_dsp_empty_func1(X)                    (void)X
#define riscv_dsp_empty_func2(X, Y)                 riscv_dsp_empty_func1(X); riscv_dsp_empty_func1(Y)
#define riscv_dsp_empty_func3(X, Y, Z)              riscv_dsp_empty_func2(X, Y); riscv_dsp_empty_func1(Z)
#define riscv_dsp_empty_func4(X, Y, Z, U)           riscv_dsp_empty_func3(X, Y, Z); riscv_dsp_empty_func1(U)
#define riscv_dsp_empty_func5(X, Y, Z, U, V)        riscv_dsp_empty_func4(X, Y, Z, U); riscv_dsp_empty_func1(V)
#define riscv_dsp_empty_func6(X, Y, Z, U, V, W)     riscv_dsp_empty_func5(X, Y, Z, U, V); riscv_dsp_empty_func1(W)
#define riscv_dsp_empty_func7(X, Y, Z, U, V, W, P)  riscv_dsp_empty_func6(X, Y, Z, U, V, W); riscv_dsp_empty_func1(P)

#define empty_func_ret                          return 0
#define riscv_dsp_empty_ret_func1(X)                riscv_dsp_empty_func1(X); empty_func_ret
#define riscv_dsp_empty_ret_func2(X, Y)             riscv_dsp_empty_func2(X, Y); empty_func_ret
#define riscv_dsp_empty_ret_func3(X, Y, Z)          riscv_dsp_empty_func3(X, Y, Z); empty_func_ret
#define riscv_dsp_empty_ret_func4(X, Y, Z, U)       riscv_dsp_empty_func4(X, Y, Z, U); empty_func_ret
#define riscv_dsp_empty_ret_func5(X, Y, Z, U, V)    riscv_dsp_empty_func5(X, Y, Z, U, V); empty_func_ret
#define riscv_dsp_empty_ret_func6(X, Y, Z, U, V, W) riscv_dsp_empty_func6(X, Y, Z, U, V, W); empty_func_ret
#define riscv_dsp_empty_ret_func7(X, Y, Z, U, V, W, P) riscv_dsp_empty_func7(X, Y, Z, U, V, W, P); empty_func_ret

#endif // NDS32_EMPTY_FUNCTION_H
