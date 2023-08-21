#include "riscv_dsp_matrix_math.h"

float32_t matsrcf32_1[MAT_SIZE] FUNC_ATTR_ALIGN;
float32_t matsrcf32_2[MAT2_SIZE] FUNC_ATTR_ALIGN;
float32_t matoutf32[MAT_MUL_OUT] FUNC_ATTR_ALIGN;
int main(void)
{
    total_samples = MAT_MUL_OUT;
    uint32_t i;

    for (i = 0; i < MAT_SIZE; i++)
    {
        matsrcf32_1[i] = ((q15_t)(rand() >> 16)) / 32768.0f;
    }

    for (i = 0; i < MAT2_SIZE; i++)
    {
        matsrcf32_2[i] = ((q15_t)(rand() >> 16)) / 32768.0f;
    }

#ifdef PF_COUNTER
    startPFM;
#endif
    riscv_dsp_mat_mul_f32(matsrcf32_1, matsrcf32_2, matoutf32, ROW, COL, COL2);
#ifdef PF_COUNTER
    stopPFM;
    readResult();
#endif

    return 0;
}
