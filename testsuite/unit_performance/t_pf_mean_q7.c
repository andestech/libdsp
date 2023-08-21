#include "riscv_dsp_statistics_math.h"

q7_t fsrcq7[SIZE] FUNC_ATTR_ALIGN;
int main(void)
{
    uint32_t i,  bsize = SIZE;
    q7_t *fptrq7;
    q7_t ret;

    for (fptrq7 = &fsrcq7[0], i = 0; i < bsize; i++)
    {
        *fptrq7++ = (q7_t)(rand() >> 24);
    }

#ifdef PF_COUNTER
    startPFM;
#endif
    ret = riscv_dsp_mean_q7(fsrcq7, bsize);
#ifdef PF_COUNTER
    stopPFM;
    readResult();
#endif

    return ret;
}
