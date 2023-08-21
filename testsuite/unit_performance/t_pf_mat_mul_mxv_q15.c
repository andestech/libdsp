#include "riscv_dsp_matrix_math.h"

q15_t matsrcq15_1[MXV_ROW * MXV_COL] FUNC_ATTR_ALIGN;
q15_t matsrcq15_2[MXV_COL] FUNC_ATTR_ALIGN;
q15_t matoutq15[MXV_ROW] FUNC_ATTR_ALIGN;
int main(void)
{
    uint32_t i;
    total_samples = MXV_ROW * MXV_COL;

    for (i = 0; i < MXV_ROW * MXV_COL; i++)
    {
        matsrcq15_1[i] = (q15_t)(rand() >> 16);
    }

    for (i = 0; i < MXV_COL; i++)
    {
        matsrcq15_2[i] = (q15_t)(rand() >> 16);
    }

#ifdef PF_COUNTER
    startPFM;
#endif
    riscv_dsp_mat_mul_mxv_q15(matsrcq15_1, matsrcq15_2, matoutq15, MXV_ROW, MXV_COL);
#ifdef PF_COUNTER
    stopPFM;
    readResult();
#endif

    return 0;
}
