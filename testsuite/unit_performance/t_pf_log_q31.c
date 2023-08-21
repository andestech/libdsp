#include <math.h>
#include "riscv_dsp_utils_math.h"

q31_t src[SIZE]  FUNC_ATTR_ALIGN;

int main(void)
{
    q15_t ret;
    uint32_t i;

    for (i = 0; i < BUF_SIZE_F32; i++)
    {
        q31_t tmpin = abs((q31_t)rand());
        src[i] = tmpin;
    }
#ifdef PF_COUNTER
    startPFM;
#endif
    for (i = 0; i < SIZE; i++)
    {
        ret = riscv_dsp_log_q31(src[i]);
    }
#ifdef PF_COUNTER
    stopPFM;
    readResult();
#endif

    return ret;
}
