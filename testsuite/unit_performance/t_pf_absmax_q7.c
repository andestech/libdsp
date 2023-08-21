#include "riscv_dsp_statistics_math.h"


q7_t fsrcq7[SIZE] FUNC_ATTR_ALIGN FUNC_ATTR_ALIGN;
int main(void)
{
    uint32_t i, pindex;
    q7_t *fptrq7;
    q7_t ret;

    for (fptrq7 = &fsrcq7[0], i = 0; i < SIZE; i++)
    {
        *fptrq7++ = (q7_t)(rand() >> 24);

        if (i == (int)(SIZE / 2)) {
            *fptrq7 = (q7_t)127;
        }
    }

#ifdef PF_COUNTER
    startPFM;
#endif
    ret = riscv_dsp_absmax_q7(fsrcq7, SIZE, &pindex);
#ifdef PF_COUNTER
    stopPFM;
    readResult();
#endif

    return ret;
}
