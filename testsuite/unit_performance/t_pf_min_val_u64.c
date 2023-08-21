#include "riscv_dsp_statistics_math.h"


u64_t src[SIZE] FUNC_ATTR_ALIGN ;
int main(void)
{
    uint32_t i = 0;
    u64_t ret;

    do
    {
        u64_t tmpval = (rand() * RAND_MAX);
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
    ret = riscv_dsp_min_val_u64(src, SIZE);
#ifdef PF_COUNTER
    stopPFM;
    readResult();
#endif

    return ret;
}
