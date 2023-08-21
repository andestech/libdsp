#include "riscv_dsp_matrix_math.h"

uint8_t matsrc[MAT_SIZE] FUNC_ATTR_ALIGN;
uint8_t matout[MAT_TRANS_OUT] FUNC_ATTR_ALIGN;
int main(void)
{
    uint32_t i;
    total_samples = MAT_TRANS_OUT;

    for (i = 0; i < MAT_SIZE; i++)
    {
        matsrc[i] = (rand() % 256);
    }

#ifdef PF_COUNTER
    startPFM;
#endif
    riscv_dsp_mat_trans_u8(matsrc, matout, ROW, COL);
#ifdef PF_COUNTER
    stopPFM;
    readResult();
#endif

    return 0;
}
