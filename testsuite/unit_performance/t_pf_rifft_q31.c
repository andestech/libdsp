#include "riscv_dsp_transform_math.h"

q31_t dctq31[FFT2_N] FUNC_ATTR_ALIGN;
q31_t *dctptrq31 = &dctq31[0];
int main(void)
{
    uint32_t i;
    total_samples = FFT2_N;
    /* Test code */

    for (i = 0; i < FFT2_N; i++)
    {
        *dctptrq31++ = (q31_t)rand();
    }
#ifdef PF_COUNTER
    startPFM;
#endif
    riscv_dsp_rifft_q31(dctq31, FFT2_LOGN);
#ifdef PF_COUNTER
    stopPFM;
    readResult();
#endif
    /* Test code end*/

    return 0;
}