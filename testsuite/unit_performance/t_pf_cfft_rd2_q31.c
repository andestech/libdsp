#include "riscv_dsp_transform_math.h"

q31_t transq31[2 * FFT_N] FUNC_ATTR_ALIGN;
q31_t *transptrq31 = &transq31[0];
int main(void)
{
    total_samples = FFT_N;
    uint32_t i;
    for (i = 0; i < FFT_N; i++)
    {
        *transptrq31++ = (q31_t)rand();
        *transptrq31++ = (q31_t)rand();
    }

#ifdef PF_COUNTER
    startPFM;
#endif
    riscv_dsp_cfft_rd2_q31(transq31, FFT_LOGN);
    riscv_dsp_cifft_rd2_q31(transq31, FFT_LOGN);
#ifdef PF_COUNTER
    stopPFM;
    readResult();
#endif
    return 0;
}
