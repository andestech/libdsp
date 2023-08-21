#include "riscv_dsp_statistics_math.h"

q31_t fsrcq31[SIZE] FUNC_ATTR_ALIGN;
int main(void)
{
    uint32_t i;
    q31_t *fptrq31;
    q31_t ret;

    for (fptrq31 = &fsrcq31[0], i = 0; i < SIZE; i++)
    {
        *fptrq31 = (q31_t)rand();
        if (i == (int)(SIZE / 2)) {
            *fptrq31 = (q31_t)2147483647;
        }
        fptrq31++;
    }

#ifdef PF_COUNTER
    startPFM;
#endif
    ret = riscv_dsp_absmax_val_q31(fsrcq31, SIZE);
#ifdef PF_COUNTER
    stopPFM;
    readResult();
#endif

    return ret;
}
