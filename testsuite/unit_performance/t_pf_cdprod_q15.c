#include "riscv_dsp_complex_math.h"

q15_t srcq15[SIZE] FUNC_ATTR_ALIGN;
q15_t srcq15_2[SIZE] FUNC_ATTR_ALIGN;
q15_t outq15[SIZE] FUNC_ATTR_ALIGN;
int main(void)
{
    uint32_t i;

    for (i = 0; i < SIZE; i++)
    {
        srcq15[i] = (q15_t)(rand() >> 16);
        srcq15_2[i] = (q15_t)(rand() >> 16);
    }

#ifdef PF_COUNTER
    startPFM;
#endif
    riscv_dsp_cdprod_q15(srcq15, srcq15_2, SIZE / 2, outq15);
#ifdef PF_COUNTER
    stopPFM;
    readResult();
#endif

    return 0;
}
