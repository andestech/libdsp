#include "riscv_dsp_transform_math.h"
 #define CFFT_LOGN 10
 #define CFFT_N (1 << CFFT_LOGN)

float32_t transf32[2 * CFFT_N] FUNC_ATTR_ALIGN;
float32_t *transptrf32 = &transf32[0];
int main(void)
{
    total_samples = CFFT_N;
    uint32_t i;
    for (i = 0; i < CFFT_N; i++)
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
    riscv_dsp_cifft_f32(transf32, CFFT_LOGN);
#ifdef PF_COUNTER
    stopPFM;
    readResult();
#endif
    return 0;
}
