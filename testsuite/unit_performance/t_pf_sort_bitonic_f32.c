#include "riscv_dsp_sort_math.h"

float32_t srcf32[BUF_SIZE_F32] FUNC_ATTR_ALIGN;
float32_t outf32[BUF_SIZE_F32] FUNC_ATTR_ALIGN;
int main(void)
{
    total_samples = BUF_SIZE_F32 * 2;
    uint32_t i;
    riscv_dsp_sort_f32_t *instance;

    for (i = 0; i < BUF_SIZE_F32; i++)
    {
        srcf32[i] = ((q15_t)(rand() >> 16)) / 32768.0f;
    }
#ifdef PF_COUNTER
    startPFM;
#endif
    riscv_dsp_sort_init_f32(instance, RISCV_DSP_SORT_BITONIC, RISCV_DSP_SORT_ASCENDING);
    riscv_dsp_sort_f32(instance, srcf32, outf32, BUF_SIZE_F32);

    riscv_dsp_sort_init_f32(instance, RISCV_DSP_SORT_BITONIC, RISCV_DSP_SORT_DESCENDING);
    riscv_dsp_sort_f32(instance, srcf32, outf32, BUF_SIZE_F32);
#ifdef PF_COUNTER
    stopPFM;
    readResult();
#endif
    return 0;
}
