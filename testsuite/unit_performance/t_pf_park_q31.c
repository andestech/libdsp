#include <math.h>
#include "riscv_dsp_controller_math.h"

q31_t srcq31_1[SIZE] FUNC_ATTR_ALIGN;
q31_t srcq31_2[SIZE] FUNC_ATTR_ALIGN;
q31_t sinval[SIZE] FUNC_ATTR_ALIGN;
q31_t cosval[SIZE] FUNC_ATTR_ALIGN;
q31_t outq31_1[SIZE] FUNC_ATTR_ALIGN, outq31_2[SIZE] FUNC_ATTR_ALIGN;
int main(void)
{
    uint32_t i;
    float32_t tmp;

    for (i = 0; i < SIZE; i++)
    {
        srcq31_1[i] = (q31_t)rand();
        srcq31_2[i] = (q31_t)rand();
        tmp = ((q15_t)(rand() >> 16)) / 32768.0f;
        sinval[i] = sin(tmp) * 1073741824 * 2;
        cosval[i] = cos(tmp) * 1073741824 * 2;

    }

#ifdef PF_COUNTER
    startPFM;
#endif
#ifndef ENA_EMPTY_SIZE
    for (i = 0; i < SIZE; i++)
    {
        riscv_dsp_park_q31(srcq31_1[i], srcq31_2[i], &outq31_1[i], &outq31_2[i], sinval[i], cosval[i]);
    }
#endif
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
