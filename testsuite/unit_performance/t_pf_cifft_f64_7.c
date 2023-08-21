#include "riscv_dsp_transform_math.h"
 #define CFFT_LOGN 7
 #define CFFT_N (1 << CFFT_LOGN)

float64_t transf64[2 * CFFT_N] FUNC_ATTR_ALIGN;
float64_t *transptrf64 = &transf64[0];
int main(void)
{
    total_samples = CFFT_N;
    uint32_t i;
    for (i = 0; i < CFFT_N; i++)
    {
        int32_t m = rand(), n = rand();
        *transptrf64++ = n ? m / (double)n : (double)m;
        m = rand();
        n = rand();
        *transptrf64++ = n ? m / (double)n : (double)m;
    }

#ifdef PF_COUNTER
    startPFM;
#endif
    riscv_dsp_cifft_f64(transf64, CFFT_LOGN);
#ifdef PF_COUNTER
    stopPFM;
    readResult();
#endif
    return 0;
}
