#include "riscv_dsp_complex_math.h"

q15_t srcq15[SIZE] FUNC_ATTR_ALIGN;
q15_t clxinq15[SIZE / 2] FUNC_ATTR_ALIGN;
q15_t outq15[SIZE] FUNC_ATTR_ALIGN;
int main(void)
{
    uint32_t i;

    for (i = 0; i < SIZE / 2; i++)
    {
        srcq15[2 * i] = (q15_t)(rand() >> 16);
        srcq15[(2 * i) + 1] = (q15_t)(rand() >> 16);
        clxinq15[i] = (q15_t)(rand() >> 16);
    }

#ifdef PF_COUNTER
    startPFM;
#endif
    riscv_dsp_cmul_real_q15(srcq15, clxinq15, outq15, SIZE / 2);
#ifdef PF_COUNTER
    stopPFM;
    readResult();
#endif

    return 0;
}
