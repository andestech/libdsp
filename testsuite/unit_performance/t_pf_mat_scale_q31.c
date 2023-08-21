#include "riscv_dsp_matrix_math.h"

q31_t matsrcq31[MAT_SIZE] FUNC_ATTR_ALIGN;
q31_t matoutq31[MAT_SIZE] FUNC_ATTR_ALIGN;
int main(void)
{
    uint32_t i;
    q31_t matsclaeq31 = 20;
    int32_t sftq31 = 14;
    total_samples = MAT_SIZE;

    for (i = 0; i < MAT_SIZE; i++)
    {
        matsrcq31[i] = (q31_t)rand();
    }

#ifdef PF_COUNTER
    startPFM;
#endif
    riscv_dsp_mat_scale_q31(matsrcq31, matsclaeq31, sftq31, matoutq31, ROW, COL);
#ifdef PF_COUNTER
    stopPFM;
    readResult();
#endif

    return 0;
}
