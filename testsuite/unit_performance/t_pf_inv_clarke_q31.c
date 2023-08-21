#include "riscv_dsp_controller_math.h"

q31_t srcq31_1[SIZE] FUNC_ATTR_ALIGN;
q31_t srcq31_2[SIZE] FUNC_ATTR_ALIGN;
q31_t outq31_1[SIZE] FUNC_ATTR_ALIGN, outq31_2[SIZE] FUNC_ATTR_ALIGN;
int main(void)
{
    uint32_t i;

    for (i = 0; i < SIZE; i++)
    {
        srcq31_1[i] = (q31_t)rand();
        srcq31_2[i] = (q31_t)rand();
    }

#ifdef PF_COUNTER
    startPFM;
#endif
    for (i = 0; i < SIZE; i++)
    {
        riscv_dsp_inv_clarke_q31(srcq31_1[i], srcq31_2[i], &outq31_1[i], &outq31_2[i]);
    }
#ifdef PF_COUNTER
    stopPFM;
    readResult();
#endif
    for (i = 0; i < 2; i++)
    {
        PRINTF_INT32(outq31_1[i]);
        PRINTF_INT32(outq31_2[i]);
    }
    return 0;
}
