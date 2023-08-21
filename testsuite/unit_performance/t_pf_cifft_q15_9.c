#include "riscv_dsp_transform_math.h"
 #define CFFT_LOGN 9
 #define CFFT_N (1 << CFFT_LOGN)

q15_t transq15[2 * CFFT_N] FUNC_ATTR_ALIGN;
q15_t *transptrq15 = &transq15[0];
int main(void)
{
    total_samples = CFFT_N;
    uint32_t i;

    for (i = 0; i < CFFT_N; i++)
    {
        *transptrq15++ = (q15_t)(rand() >> 16);
        *transptrq15++ = (q15_t)(rand() >> 16);
    }

#ifdef PF_COUNTER
    startPFM;
#endif
    riscv_dsp_cifft_q15(transq15, CFFT_LOGN);
#ifdef PF_COUNTER
    stopPFM;
    readResult();
#endif
    return 0;
}
