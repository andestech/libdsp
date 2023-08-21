#include "riscv_dsp_basic_math.h"

q7_t srcq7[SIZE] FUNC_ATTR_ALIGN;
q7_t srcq7_2[SIZE] FUNC_ATTR_ALIGN;
int main(void)
{
    uint32_t i;
    q31_t ret;

    for (i = 0; i < SIZE; i++)
    {
        srcq7[i] = (q7_t)(rand() >> 24);
        srcq7_2[i] = (q7_t)(rand() >> 24);
    }

#ifdef PF_COUNTER
    startPFM;
#endif
    ret = riscv_dsp_dprod_q7(srcq7, srcq7_2, SIZE);
#ifdef PF_COUNTER
    stopPFM;
    readResult();
#endif

    return ret;
}
