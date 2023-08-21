#include "riscv_dsp_transform_math.h"

float32_t transf32[2 * FFT_N] FUNC_ATTR_ALIGN;
float32_t *transptrf32 = &transf32[0];
int main(void)
{
    total_samples = FFT_N;
    uint32_t i;
    for (i = 0; i < FFT_N; i++)
    {
        int32_t m = rand(), n = rand();
        *transptrf32++ = n ? m / (float)n : (float)m;
        m = rand();
        n = rand();
        *transptrf32++ = n ? m / (float)n : (float)m;
    }

#ifdef PF_COUNTER
    startPFM;
#endif
    riscv_dsp_cfft_f32(transf32, FFT_LOGN);
#ifdef PF_COUNTER
    stopPFM;
    readResult();
#endif
    return 0;
}
