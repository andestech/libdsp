#include "riscv_dsp_transform_math.h"

float32_t dctf32[FFT2_N] FUNC_ATTR_ALIGN;
float32_t *dctptrf32 = &dctf32[0];
int main(void)
{
    total_samples = FFT2_N;
    uint32_t i;
    /* Test code */
    for (i = 0; i < FFT2_N; i++)
    {
        int32_t m = rand(), n = rand();
        *dctptrf32++ = n ? m / (float)n : (float)m;
    }

#ifdef PF_COUNTER
    startPFM;
#endif
    riscv_dsp_rfft_f32(dctf32, FFT2_LOGN);
#ifdef PF_COUNTER
    stopPFM;
    readResult();
#endif
    /* Test code end*/

    return 0;
}
