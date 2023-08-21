#include "riscv_dsp_statistics_math.h"


u16_t src[SIZE] FUNC_ATTR_ALIGN ;
int main(void)
{
    uint32_t i = 0, pindex;
    u16_t ret;

    do
    {
        u16_t tmpval = (rand() % 65536);
        if(tmpval != 0)
        {
            src[i] = tmpval;
            i++;
        }
    }
    while (i < SIZE);
    src[SIZE / 2] = 0x0;

#ifdef PF_COUNTER
    startPFM;
#endif
    ret = riscv_dsp_min_u16(src, SIZE, &pindex);
#ifdef PF_COUNTER
    stopPFM;
    readResult();
#endif

    return ret;
}
