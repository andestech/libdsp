#include "riscv_dsp_matrix_math.h"

float32_t matsrcf32[MAT_SIZE * 2] FUNC_ATTR_ALIGN;
float32_t matoutf32[MAT_TRANS_OUT * 2] FUNC_ATTR_ALIGN;
int main(void)
{
    total_samples = MAT_TRANS_OUT;
    uint32_t i;

    for (i = 0; i < MAT_SIZE; i++)
    {
        matsrcf32[i] = ((q15_t)(rand() >> 16)) / 32768.0f;
    }

#ifdef PF_COUNTER
    startPFM;
#endif
    riscv_dsp_cmat_trans_f32(matsrcf32, matoutf32, ROW, COL);
#ifdef PF_COUNTER
    stopPFM;
    readResult();
#endif

    return 0;
}