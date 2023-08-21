#include "riscv_dsp_statistics_math.h"


q31_t src[SIZE] FUNC_ATTR_ALIGN;
int main(void)
{
    uint32_t i = 0;
    q31_t ret;
    do
    {
        q31_t tmpval = (q31_t)(rand());
        if(tmpval != 0x7fffffff)
        {
            src[i] = tmpval;
            i++;
        }
    }
    while (i < SIZE);
    src[SIZE / 2] = 0x7fffffff;

#ifdef PF_COUNTER
    startPFM;
#endif
    ret = riscv_dsp_max_val_q31(src, SIZE);
#ifdef PF_COUNTER
    stopPFM;
    readResult();
#endif

    return ret;
}
