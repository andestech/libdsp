#include "riscv_dsp_matrix_math.h"

q7_t matsrcq7_1[MAT_SIZE] FUNC_ATTR_ALIGN;
q7_t matsrcq7_2[MAT2_SIZE] FUNC_ATTR_ALIGN;
q7_t matoutq7[MAT_MUL_OUT] FUNC_ATTR_ALIGN;
int main(void)
{
    uint32_t i;
    total_samples = MAT_MUL_OUT;

    for (i = 0; i < MAT_SIZE; i++)
    {
        matsrcq7_1[i] = (q7_t)(rand() >> 26);
    }

    for (i = 0; i < MAT2_SIZE; i++)
    {
        matsrcq7_2[i] = (q7_t)(rand() >> 26);
    }

#ifdef PF_COUNTER
    startPFM;
#endif
    riscv_dsp_mat_mul_q7(matsrcq7_1, matsrcq7_2, matoutq7, ROW, COL, COL2);
#ifdef PF_COUNTER
    stopPFM;
    readResult();
#endif
    //printf_matrix(matoutq7, ROW, COL2);

    return 0;
}
