#include "riscv_dsp_matrix_math.h"

q31_t matsrcq31_1[MAT_SIZE] FUNC_ATTR_ALIGN;
q31_t matsrcq31_2[MAT2_SIZE] FUNC_ATTR_ALIGN;
q31_t matoutq31[MAT_MUL_OUT] FUNC_ATTR_ALIGN;
int main(void)
{
    uint32_t i;
    total_samples = MAT_MUL_OUT;

    for (i = 0; i < MAT_SIZE; i++)
    {
        matsrcq31_1[i] = (q31_t)rand();
    }

    for (i = 0; i < MAT2_SIZE; i++)
    {
        matsrcq31_2[i] = (q31_t)rand();
    }

#ifdef PF_COUNTER
    startPFM;
#endif
    riscv_dsp_mat_mul_q31(matsrcq31_1, matsrcq31_2, matoutq31, ROW, COL, COL2);
#ifdef PF_COUNTER
    stopPFM;
    readResult();
#endif

    return 0;
}