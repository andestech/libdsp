#include "riscv_dsp_matrix_math.h"

q7_t matsrcq7_1[MXV_ROW * MXV_COL] FUNC_ATTR_ALIGN;
q7_t matsrcq7_2[MXV_COL] FUNC_ATTR_ALIGN;
q7_t matoutq7[MXV_ROW] FUNC_ATTR_ALIGN;
int main(void)
{
    uint32_t i;
    total_samples = MXV_ROW * MXV_COL;

    for (i = 0; i < MXV_ROW * MXV_COL; i++)
    {
        matsrcq7_1[i] = (q7_t)(rand() >> 24);
    }

    for (i = 0; i < MXV_COL; i++)
    {
        matsrcq7_2[i] = (q7_t)(rand() >> 24);
    }

#ifdef PF_COUNTER
    startPFM;
#endif
    riscv_dsp_mat_mul_mxv_q7(matsrcq7_1, matsrcq7_2, matoutq7, MXV_ROW, MXV_COL);
#ifdef PF_COUNTER
    stopPFM;
    readResult();
#endif

    return 0;
}
