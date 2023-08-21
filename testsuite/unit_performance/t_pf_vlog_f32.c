#include "riscv_dsp_utils_math.h"

float32_t srcf32[SIZE] FUNC_ATTR_ALIGN;
float32_t outf32[SIZE] FUNC_ATTR_ALIGN;
int main(void)
{
    uint32_t i;
    total_samples = SIZE;

    for (i = 0; i < BUF_SIZE_F32; i++)
    {
        srcf32[i] = ((q15_t)(rand() >> 16)) / 32768.0f;
    }
#ifdef PF_COUNTER
    startPFM;
#endif
    riscv_dsp_vlog_f32(srcf32, outf32, SIZE);
#ifdef PF_COUNTER
    stopPFM;
    readResult();
#endif

    return 0;
}
