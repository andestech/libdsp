#include "riscv_dsp_statistics_math.h"


q15_t fsrcq15[SIZE] FUNC_ATTR_ALIGN;
int main(void)
{
    uint32_t i, pindex;
    q15_t *fptrq15;
    q15_t ret;

    for (fptrq15 = &fsrcq15[0], i = 0; i < SIZE; i++)
    {
        *fptrq15 = (q15_t)(rand() >> 16);
        if (i == (int)(SIZE / 2)) {
            *fptrq15 = (q15_t)32767;
        }
        fptrq15++;
    }

#ifdef PF_COUNTER
    startPFM;
#endif
    ret = riscv_dsp_absmax_q15(fsrcq15, SIZE, &pindex);
#ifdef PF_COUNTER
    stopPFM;
    readResult();
#endif

    return ret;
}
