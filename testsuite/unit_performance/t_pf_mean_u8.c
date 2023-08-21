#include "riscv_dsp_statistics_math.h"

uint8_t src[SIZE] FUNC_ATTR_ALIGN;
int main(void)
{
    uint32_t i,  bsize = SIZE;
    uint8_t ret;

    for (i = 0; i < SIZE; i++)
    {
        src[i] = (rand() % 256);
    }

#ifdef PF_COUNTER
    startPFM;
#endif
    ret = riscv_dsp_mean_u8(src, bsize);
#ifdef PF_COUNTER
    stopPFM;
    readResult();
#endif

    return ret;
}
