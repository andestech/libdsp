#include "riscv_dsp_matrix_math.h"

q31_t matsrcq31_1[ROW] FUNC_ATTR_ALIGN;
q31_t matsrcq31_2[COL] FUNC_ATTR_ALIGN;
q31_t matoutq31[ROW * COL] FUNC_ATTR_ALIGN;
int main(void)
{
    uint32_t i;
    total_samples = ROW * COL;

    for (i = 0; i < ROW; i++)
    {
        matsrcq31_1[i] = (q31_t)rand();
    }

    for (i = 0; i < COL; i++)
    {
        matsrcq31_2[i] = (q31_t)rand();
    }

#ifdef PF_COUNTER
    startPFM;
#endif
    riscv_dsp_mat_oprod_q31(matsrcq31_1, matsrcq31_2, matoutq31, ROW, COL);
#ifdef PF_COUNTER
    stopPFM;
    readResult();
#endif

    return 0;
}
