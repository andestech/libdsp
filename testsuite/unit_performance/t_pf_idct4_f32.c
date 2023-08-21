#include "riscv_dsp_transform_math.h"

float32_t dct4f32[FFT2_N] FUNC_ATTR_ALIGN;
float32_t *dct4ptrf32 = &dct4f32[0];
int main(void)
{
    total_samples = FFT2_N;
    uint32_t i;
    /* Test code */
    for (i = 0; i < FFT2_N; i++)
    {
        int32_t m = rand(), n = rand();
        *dct4ptrf32++ = n ? m / (float)n : (float)m;
    }
#ifdef PF_COUNTER
    startPFM;
#endif
    riscv_dsp_idct4_f32(dct4f32, FFT2_LOGN);
#ifdef PF_COUNTER
    stopPFM;
    readResult();
#endif
    /* Test code end*/

    return 0;
}
