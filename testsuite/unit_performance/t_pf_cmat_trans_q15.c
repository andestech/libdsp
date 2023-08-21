#include "riscv_dsp_matrix_math.h"

q15_t matsrcq15[MAT_SIZE * 2] FUNC_ATTR_ALIGN;
q15_t matoutq15[MAT_TRANS_OUT * 2] FUNC_ATTR_ALIGN;
int main(void)
{
    uint32_t i;
    total_samples = MAT_TRANS_OUT;

    for (i = 0; i < MAT_SIZE; i++)
    {
        matsrcq15[i] = (q15_t)(rand() >> 16);
    }

#ifdef PF_COUNTER
    startPFM;
#endif
    riscv_dsp_cmat_trans_q15(matsrcq15, matoutq15, ROW, COL);
#ifdef PF_COUNTER
    stopPFM;
    readResult();
#endif

    return 0;
}
