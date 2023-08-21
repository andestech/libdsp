#include "riscv_dsp_basic_math.h"

q7_t srcq7[SIZE] FUNC_ATTR_ALIGN;
q7_t outq7[SIZE] FUNC_ATTR_ALIGN;
int main(void)
{
    uint32_t i;
    q7_t high = 0x40;
    q7_t low = 0xf0;

    for (i = 0; i < SIZE; i++)
    {
        srcq7[i] = (q7_t)(rand() >> 24) + 0x4f;
    }
#ifdef PF_COUNTER
    startPFM;
#endif
    riscv_dsp_clip_q7(srcq7, outq7, low, high, SIZE);
#ifdef PF_COUNTER
    stopPFM;
    readResult();
#endif

    return 0;
}
