#include "riscv_dsp_matrix_math.h"

q15_t matsrcq15_1[MAT_SIZE * 2] FUNC_ATTR_ALIGN;
q15_t matsrcq15_2[MAT2_SIZE * 2] FUNC_ATTR_ALIGN;
q15_t matoutq15[MAT_MUL_OUT * 2] FUNC_ATTR_ALIGN;
int main(void)
{
    uint32_t i;
    total_samples = MAT_MUL_OUT * 2;

    for (i = 0; i < MAT_SIZE * 2; i++)
    {
        matsrcq15_1[i] = (q15_t)(rand() >> 16);
    }

    for (i = 0; i < MAT2_SIZE * 2; i++)
    {
        matsrcq15_2[i] = (q15_t)(rand() >> 16);
    }

#ifdef PF_COUNTER
    startPFM;
#endif
    riscv_dsp_cmat_mul_q15(matsrcq15_1, matsrcq15_2, matoutq15, ROW, COL, COL2);
#ifdef PF_COUNTER
    stopPFM;
    readResult();
#endif

    return 0;
}
