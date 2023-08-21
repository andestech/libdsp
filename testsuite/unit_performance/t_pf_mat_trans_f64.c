#include "riscv_dsp_matrix_math.h"

float64_t matsrcf64[MAT_SIZE] FUNC_ATTR_ALIGN;
float64_t matoutf64[MAT_TRANS_OUT] FUNC_ATTR_ALIGN;
int main(void)
{
    total_samples = MAT_TRANS_OUT;
    uint32_t i;

    for (i = 0; i < MAT_SIZE; i++)
    {
        matsrcf64[i] = ((q15_t)(rand() >> 16)) / 32768.0f;
    }

#ifdef PF_COUNTER
    startPFM;
#endif
    riscv_dsp_mat_trans_f64(matsrcf64, matoutf64, ROW, COL);
#ifdef PF_COUNTER
    stopPFM;
    readResult();
#endif

    return 0;
}
