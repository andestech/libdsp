#include "riscv_dsp_basic_math.h"

q7_t srcq7[SIZE] FUNC_ATTR_ALIGN;
q7_t outq7[SIZE] FUNC_ATTR_ALIGN;
int main(void)
{
    uint32_t i;
    q7_t *ptrq7;

    for (ptrq7 = &srcq7[0], i = 0; i < SIZE; i++)
    {
        *ptrq7++ = (q7_t)(rand() >> 24);
    }

#ifdef PF_COUNTER
    startPFM;
#endif
    riscv_dsp_neg_q7(srcq7, outq7, SIZE);
#ifdef PF_COUNTER
    stopPFM;
    readResult();
#endif

    return 0;
}