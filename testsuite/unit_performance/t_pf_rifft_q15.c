#include "riscv_dsp_transform_math.h"

q15_t dctq15[FFT2_N] FUNC_ATTR_ALIGN;
q15_t *dctptrq15 = &dctq15[0];
int main(void)
{
    uint32_t i; 
    total_samples = FFT2_N;
    /* Test code */
   
   for (i = 0; i < FFT2_N; i++)
    {
        *dctptrq15++ = (q15_t)(rand() >> 16);
    }
#ifdef PF_COUNTER
    startPFM;
#endif
    riscv_dsp_rifft_q15(dctq15, FFT2_LOGN);
#ifdef PF_COUNTER
    stopPFM;
    readResult();
#endif
    /* Test code end*/

    return 0;
}