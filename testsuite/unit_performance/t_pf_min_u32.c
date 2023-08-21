#include "riscv_dsp_statistics_math.h"


u32_t src[SIZE] FUNC_ATTR_ALIGN ;
int main(void)
{
    uint32_t i = 0, pindex;
    u32_t ret;

    do
    {
        u32_t tmpval = (rand() % 4294967296);
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
    ret = riscv_dsp_min_u32(src, SIZE, &pindex);
#ifdef PF_COUNTER
    stopPFM;
    readResult();
#endif

    return ret;
}
