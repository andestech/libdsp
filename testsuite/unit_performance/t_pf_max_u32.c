#include "riscv_dsp_statistics_math.h"


u32_t src[SIZE] FUNC_ATTR_ALIGN ;
int main(void)
{
    uint32_t i, pindex;
    u32_t ret;

    for (i = 0; i < SIZE; i++)
    {
        src[i] = (rand() % 4294967295);
        if (i == (int)(SIZE / 2))
        {
            src[i] = 0xffffffff;
        }
    }

#ifdef PF_COUNTER
    startPFM;
#endif
    ret = riscv_dsp_max_u32(src, SIZE, &pindex);
#ifdef PF_COUNTER
    stopPFM;
    readResult();
#endif

    return ret;
}
