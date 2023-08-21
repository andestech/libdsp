#include "riscv_dsp_matrix_math.h"

q15_t matsrcq15[MAT_SIZE] FUNC_ATTR_ALIGN;
q15_t matoutq15[MAT_SIZE] FUNC_ATTR_ALIGN;
int main(void)
{
    q15_t matsclaeq15 = 5;
    int32_t sft = 4;
    uint32_t i;
    total_samples = MAT_SIZE;

    for (i = 0; i < MAT_SIZE; i++)
    {
        matsrcq15[i] = (q15_t)(rand() >> 16);
    }

#ifdef PF_COUNTER
    startPFM;
#endif
    riscv_dsp_mat_scale_q15(matsrcq15, matsclaeq15, sft, matoutq15, ROW, COL);
#ifdef PF_COUNTER
    stopPFM;
    readResult();
#endif

    return 0;
}
