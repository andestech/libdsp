#include "riscv_dsp_basic_math.h"

q7_t srcq7_1[SIZE] FUNC_ATTR_ALIGN;
q7_t srcq7_2[SIZE] FUNC_ATTR_ALIGN;
q7_t outq7[SIZE] FUNC_ATTR_ALIGN;
int main(void)
{
    uint32_t i;

    for (i = 0; i < SIZE; i++)
    {
        srcq7_1[i] = (q7_t)(rand() >> 24);
        srcq7_2[i] = (q7_t)(rand() >> 24);
    }

#ifdef PF_COUNTER
    startPFM;
#endif
    riscv_dsp_add_q7(srcq7_1, srcq7_2, outq7, SIZE);
#ifdef PF_COUNTER
    stopPFM;
    readResult();
#endif

    return 0;
}
