#include "riscv_dsp_basic_math.h"

q31_t srcq31[SIZE] FUNC_ATTR_ALIGN;
q31_t outq31[SIZE] FUNC_ATTR_ALIGN;
int main(void)
{
    uint32_t i;
    q31_t *ptrq31;

    for (ptrq31 = &srcq31[0], i = 0; i < SIZE; i++)
    {
        *ptrq31++ = (q31_t)rand();
    }
#ifdef PF_COUNTER
    startPFM;
#endif
    riscv_dsp_neg_q31(srcq31, outq31, SIZE);
#ifdef PF_COUNTER
    stopPFM;
    readResult();
#endif

    return 0;
}
