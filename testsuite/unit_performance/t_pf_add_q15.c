#include "riscv_dsp_basic_math.h"

q15_t srcq15_1[SIZE] FUNC_ATTR_ALIGN;
q15_t srcq15_2[SIZE] FUNC_ATTR_ALIGN;
q15_t outq15[SIZE] FUNC_ATTR_ALIGN;
int main(void)
{
    uint32_t i;

    for (i = 0; i < SIZE; i++)
    {
        srcq15_1[i] = (q15_t)(rand() >> 16);
        srcq15_2[i] = (q15_t)(rand() >> 16);
    }
#ifdef PF_COUNTER
    startPFM;
#endif
    riscv_dsp_add_q15(srcq15_1, srcq15_2, outq15, SIZE);
#ifdef PF_COUNTER
    stopPFM;
    readResult();
#endif

    return 0;
}