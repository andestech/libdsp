#include "riscv_dsp_basic_math.h"

q15_t srcq15[SIZE] FUNC_ATTR_ALIGN;
q15_t outq15[SIZE] FUNC_ATTR_ALIGN;
int main(void)
{
    uint32_t i;
    q15_t high = 0x4000;
    q15_t low = 0xf000;

    for (i = 0; i < SIZE; i++)
    {
        srcq15[i] = (q15_t)(rand() >> 16) + 0x4fff;
    }
#ifdef PF_COUNTER
    startPFM;
#endif
    riscv_dsp_clip_q15(srcq15, outq15, low, high, SIZE);
#ifdef PF_COUNTER
    stopPFM;
    readResult();
#endif

    return 0;
}
