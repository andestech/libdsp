#include "riscv_dsp_transform_math.h"

q15_t transq15[2 * FFT_N] FUNC_ATTR_ALIGN;
q15_t *transptrq15 = &transq15[0];
int main(void)
{
    total_samples = FFT_N;
    uint32_t i;

    for (i = 0; i < FFT_N; i++)
    {
        *transptrq15++ = (q15_t)(rand() >> 16);
        *transptrq15++ = (q15_t)(rand() >> 16);
    }

#ifdef PF_COUNTER
    startPFM;
#endif
    riscv_dsp_cfft_q15(transq15, FFT_LOGN);
#ifdef PF_COUNTER
    stopPFM;
    readResult();
#endif
    return 0;
}
