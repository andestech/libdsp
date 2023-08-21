#include "riscv_dsp_basic_math.h"

q31_t srcq31[SIZE] FUNC_ATTR_ALIGN;
q31_t outq31[SIZE] FUNC_ATTR_ALIGN;
int main(void)
{
    uint32_t i;
    q31_t high = 0x40000000;
    q31_t low = 0xf0000000;
    int counthigh = 0, countlow = 0;

    i = 0;
    do
    {
        srcq31[i] = (q31_t)rand() + 0x30000000;
        if (srcq31[i] > high)
            i++;
    }
    while (i < 341);
    do
    {
        srcq31[i] = (q31_t)rand() + 0x1fffffff;
        if (srcq31[i] < low)
            i++;
    }
    while (i < 682);
    do
    {
        srcq31[i] = (q31_t)rand();
        if ((srcq31[i] < high) & (srcq31[i] > low))
            i++;
    }
    while (i < SIZE);
#ifdef PF_COUNTER
    startPFM;
#endif
    riscv_dsp_clip_q31(srcq31, outq31, low, high, SIZE);
#ifdef PF_COUNTER
    stopPFM;
    readResult();
#endif

    return 0;
}
