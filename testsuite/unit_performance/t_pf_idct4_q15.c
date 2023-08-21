#include "riscv_dsp_transform_math.h"

q15_t dct4q15[FFT2_N] FUNC_ATTR_ALIGN;
q15_t *dct4ptrq15 = &dct4q15[0];
int main(void)
{
    uint32_t i; 
    total_samples = FFT2_N;
    /* Test code */

    for (i = 0; i < FFT2_N; i++)
    {
        *dct4ptrq15++ = (q15_t)(rand() >> 16);
    }

#ifdef PF_COUNTER
    startPFM;
#endif
    riscv_dsp_idct4_q15(dct4q15, FFT2_LOGN);
#ifdef PF_COUNTER
    stopPFM;
    readResult();
#endif
    /* Test code end*/

    return 0;
}
