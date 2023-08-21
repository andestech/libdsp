#include "riscv_dsp_matrix_math.h"

q31_t matsrcq31[MAT_SIZE * 2] FUNC_ATTR_ALIGN;
q31_t matoutq31[MAT_TRANS_OUT * 2] FUNC_ATTR_ALIGN;
int main(void)
{
    uint32_t i;
    total_samples = MAT_TRANS_OUT;

    for (i = 0; i < MAT_SIZE; i++)
    {
        matsrcq31[i] = (q31_t)rand();
    }

#ifdef PF_COUNTER
    startPFM;
#endif
    riscv_dsp_cmat_trans_q31(matsrcq31, matoutq31, ROW, COL);
#ifdef PF_COUNTER
    stopPFM;
    readResult();
#endif

    return 0;
}
