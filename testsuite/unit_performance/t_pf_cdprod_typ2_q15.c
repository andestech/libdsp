#include "riscv_dsp_complex_math.h"

q15_t srcq15[SIZE] FUNC_ATTR_ALIGN;
q15_t srcq15_2[SIZE] FUNC_ATTR_ALIGN;
int main(void)
{
    q31_t clxoutq31, clxinq31;
    uint32_t i;

    for (i = 0; i < SIZE; i++)
    {
        srcq15[i] = (q15_t)(rand() >> 16);
        srcq15_2[i] = (q15_t)(rand() >> 16);
    }

#ifdef PF_COUNTER
    startPFM;
#endif
    riscv_dsp_cdprod_typ2_q15(srcq15, srcq15_2, SIZE / 2, &clxoutq31, &clxinq31);
#ifdef PF_COUNTER
    stopPFM;
    readResult();
#endif

    return (clxoutq31 + clxinq31);
}
