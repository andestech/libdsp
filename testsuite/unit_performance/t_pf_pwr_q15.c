#include "riscv_dsp_statistics_math.h"

q15_t fsrcq15[SIZE] FUNC_ATTR_ALIGN;
int main(void)
{
    uint32_t i, bsize = SIZE;
    q15_t *fptrq15;
    q63_t ret;

    for (fptrq15 = &fsrcq15[0], i = 0; i < bsize; i++)
    {
        *fptrq15++ = (q15_t)(rand() >> 16);
    }

#ifdef PF_COUNTER
    startPFM;
#endif
    ret = riscv_dsp_pwr_q15(fsrcq15, bsize);
#ifdef PF_COUNTER
    stopPFM;
    readResult();
#endif

    PRINTF_INT64(ret);
    return (int) ret;
}

