#include "riscv_dsp_matrix_math.h"

q15_t matsrcq15[MAT_SIZE] FUNC_ATTR_ALIGN;
q15_t matsrcq15_2[MAT_SIZE] FUNC_ATTR_ALIGN;
q15_t matoutq15[MAT_SIZE] FUNC_ATTR_ALIGN;
int main(void)
{
    uint32_t i;
    total_samples = MAT_SIZE;

    for (i = 0; i < MAT_SIZE; i++)
    {
        matsrcq15[i] = (q15_t)(rand() >> 16);
        matsrcq15_2[i] = (q15_t)(rand() >> 16);
    }

#ifdef PF_COUNTER
    startPFM;
#endif
    riscv_dsp_mat_sub_q15(matsrcq15, matsrcq15_2, matoutq15, ROW, COL);
#ifdef PF_COUNTER
    stopPFM;
    readResult();
#endif

    return 0;
}
