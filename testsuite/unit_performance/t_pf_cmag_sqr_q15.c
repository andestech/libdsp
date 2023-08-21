#include "riscv_dsp_complex_math.h"

q15_t srcq15[SIZE] FUNC_ATTR_ALIGN;
q15_t clxoutq15[SIZE / 2] FUNC_ATTR_ALIGN;
int main(void)
{
    uint32_t i;

    for (i = 0; i < SIZE; i++)
    {
        srcq15[i] = (q15_t)(rand() >> 16);
    }

#ifdef PF_COUNTER
    startPFM;
#endif
    riscv_dsp_cmag_sqr_q15(srcq15, clxoutq15, SIZE / 2);
#ifdef PF_COUNTER
    stopPFM;
    readResult();
#endif

    return 0;
}
