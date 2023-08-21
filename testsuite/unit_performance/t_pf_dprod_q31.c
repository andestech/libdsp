#include "riscv_dsp_basic_math.h"

q31_t srcq31[SIZE] FUNC_ATTR_ALIGN;
q31_t srcq31_2[SIZE] FUNC_ATTR_ALIGN;
int main(void)
{
    uint32_t i;
    q63_t ret;

    for (i = 0; i < SIZE; i++)
    {
        srcq31[i] = (q31_t)rand();
        srcq31_2[i] = (q31_t)rand();
    }

#ifdef PF_COUNTER
    startPFM;
#endif
    ret = riscv_dsp_dprod_q31(srcq31, srcq31_2, SIZE);
#ifdef PF_COUNTER
    stopPFM;
    readResult();
#endif

    PRINTF_INT64(ret);
    return (int) ret;
}
