#include "riscv_dsp_matrix_math.h"

float64_t matsrcf32_1[MAT_SIZE] FUNC_ATTR_ALIGN;
float64_t matoutf32[MAT_SIZE] FUNC_ATTR_ALIGN;
int main(void)
{
    total_samples = MAT_SIZE;
    uint32_t i;

    for (i = 0; i < MAT_SIZE; i++)
    {
        matsrcf32_1[i] = rand() / 32768.0;
    }

#ifdef PF_COUNTER
    startPFM;
#endif
    riscv_dsp_mat_pwr2_cache_f64(matsrcf32_1, matoutf32, ROW);
#ifdef PF_COUNTER
    stopPFM;
    readResult();
#endif

    return 0;
}
