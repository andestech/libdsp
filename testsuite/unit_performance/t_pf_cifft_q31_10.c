#include "riscv_dsp_transform_math.h"
 #define CFFT_LOGN 10
 #define CFFT_N (1 << CFFT_LOGN)

q31_t transq31[2 * CFFT_N] FUNC_ATTR_ALIGN;
q31_t *transptrq31 = &transq31[0];
int main(void)
{
    total_samples = CFFT_N;
    uint32_t i;
    for (i = 0; i < CFFT_N; i++)
    {
        *transptrq31++ = (q31_t)rand();
        *transptrq31++ = (q31_t)rand();
    }

#ifdef PF_COUNTER
    startPFM;
#endif
    riscv_dsp_cifft_q31(transq31, CFFT_LOGN);
#ifdef PF_COUNTER
    stopPFM;
    readResult();
#endif
    return 0;
}
