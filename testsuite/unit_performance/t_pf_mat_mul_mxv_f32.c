#include "riscv_dsp_matrix_math.h"

float32_t matsrcf32_1[MXV_ROW * MXV_COL] FUNC_ATTR_ALIGN;
float32_t matsrcf32_2[MXV_COL] FUNC_ATTR_ALIGN;
float32_t matoutf32[MXV_ROW] FUNC_ATTR_ALIGN;
int main(void)
{
    uint32_t i;
    total_samples = MXV_ROW * MXV_COL;

    for (i = 0; i < MXV_ROW * MXV_COL; i++)
    {
        matsrcf32_1[i] = ((q15_t)(rand() >> 16)) / 32768.0f;
    }

    for (i = 0; i < MXV_COL; i++)
    {
        matsrcf32_2[i] = ((q15_t)(rand() >> 16)) / 32768.0f;
    }

#ifdef PF_COUNTER
    startPFM;
#endif
    riscv_dsp_mat_mul_mxv_f32(matsrcf32_1, matsrcf32_2, matoutf32, MXV_ROW, MXV_COL);
#ifdef PF_COUNTER
    stopPFM;
    readResult();
#endif

    return 0;
}
