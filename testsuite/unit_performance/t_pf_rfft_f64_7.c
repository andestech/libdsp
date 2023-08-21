#include "riscv_dsp_transform_math.h"
 #define RFFT_LOGN 7
 #define RFFT_N (1 << RFFT_LOGN)

float64_t dctf64[RFFT_N] FUNC_ATTR_ALIGN;
float64_t *dctptrf64 = &dctf64[0];
int main(void)
{
    total_samples = RFFT_N;
    uint32_t i;
    /* Test code */
    for (i = 0; i < RFFT_N; i++)
    {
        int32_t m = rand(), n = rand();
        *dctptrf64++ = n ? m / (double)n : (double)m;
    }

#ifdef PF_COUNTER
    startPFM;
#endif
    riscv_dsp_rfft_f64(dctf64, RFFT_LOGN);
#ifdef PF_COUNTER
    stopPFM;
    readResult();
#endif
    /* Test code end*/

    return 0;
}
