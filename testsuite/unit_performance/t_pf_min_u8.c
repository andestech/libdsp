#include "riscv_dsp_statistics_math.h"

uint8_t src[SIZE] FUNC_ATTR_ALIGN;
int main(void)
{
    uint32_t i, pindex;
    uint8_t ret;
    i = 0;
    do
    {
        u8_t tmpval = (u8_t)(rand() >> 24);
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
    ret = riscv_dsp_min_u8(src, SIZE, &pindex);
#ifdef PF_COUNTER
    stopPFM;
    readResult();
#endif

    return ret;
}
