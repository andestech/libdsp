#include "riscv_dsp_matrix_math.h"

float32_t matsrcf32[INV_SIZE] FUNC_ATTR_ALIGN;
float32_t matoutf32[INV_SIZE] FUNC_ATTR_ALIGN;
int main(void)
{
    total_samples = INV_SIZE;
    uint32_t i;

    for (i = 0; i < INV_SIZE; i++)
    {
        matsrcf32[i] = ((q15_t)(rand() >> 16)) / 32768.0f;
    }

#ifdef PF_COUNTER
    startPFM;
#endif
    riscv_dsp_mat_inv_f32(matsrcf32, matoutf32, INV_ROW);
#ifdef PF_COUNTER
    stopPFM;
    readResult();
#endif

    return 0;
}
