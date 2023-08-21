#include "riscv_dsp_statistics_math.h"

q31_t fsrcq31[SIZE] FUNC_ATTR_ALIGN;
int main(void)
{
    uint32_t i, bsize = SIZE;
    q31_t *fptrq31;
    q63_t ret;

    for (fptrq31 = &fsrcq31[0], i = 0; i < bsize; i++)
    {
        *fptrq31++ = (q31_t)rand();
    }

#ifdef PF_COUNTER
    startPFM;
#endif
    ret = riscv_dsp_pwr_q31(fsrcq31, bsize);
#ifdef PF_COUNTER
    stopPFM;
    readResult();
#endif

    PRINTF_INT64(ret);
    return (int) ret;
}
