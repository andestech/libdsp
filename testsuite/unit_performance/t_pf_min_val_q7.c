#include "riscv_dsp_statistics_math.h"

q7_t src[SIZE] FUNC_ATTR_ALIGN;
int main(void)
{
    uint32_t i = 0;
    q7_t ret;
    do
    {
        q7_t tmpval = (q7_t)(rand() >> 24);
        if(tmpval != 0x80)
        {
            src[i] = tmpval;
            i++;
        }
    }
    while (i < SIZE);
    src[SIZE / 2] = 0x80;

#ifdef PF_COUNTER
    startPFM;
#endif
    ret = riscv_dsp_min_val_q7(src, SIZE);
#ifdef PF_COUNTER
    stopPFM;
    readResult();
#endif

    return ret;
}
