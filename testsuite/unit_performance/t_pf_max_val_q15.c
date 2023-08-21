#include "riscv_dsp_statistics_math.h"


q15_t src[SIZE] FUNC_ATTR_ALIGN;
int main(void)
{
    uint32_t i = 0;
    q15_t ret;
    do
    {
        q15_t tmpval = (q15_t)(rand() >> 16);
        if(tmpval != 0x7fff)
        {
            src[i] = tmpval;
            i++;
        }
    }
    while (i < SIZE);
    src[SIZE / 2] = 0x7f;

#ifdef PF_COUNTER
    startPFM;
#endif
    ret = riscv_dsp_max_val_q15(src, SIZE);
#ifdef PF_COUNTER
    stopPFM;
    readResult();
#endif

    return ret;
}
