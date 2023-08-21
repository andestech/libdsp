#include "riscv_dsp_statistics_math.h"

q31_t fsrcq31[SIZE] FUNC_ATTR_ALIGN;
int main(void)
{
    uint32_t i, bsize = SIZE;
    q31_t *fptrq31;
    q31_t ret;

    for (fptrq31 = &fsrcq31[0], i = 0; i < bsize; i++)
    {
        *fptrq31++ = (q31_t)rand();
    }

#ifdef PF_COUNTER
    startPFM;
#endif
    ret = riscv_dsp_mean_q31(fsrcq31, bsize);
#ifdef PF_COUNTER
    stopPFM;
    readResult();
#endif

    return ret;
}
