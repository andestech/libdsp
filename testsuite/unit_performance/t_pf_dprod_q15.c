#include "riscv_dsp_basic_math.h"

q15_t srcq15[SIZE] FUNC_ATTR_ALIGN;
q15_t srcq15_2[SIZE] FUNC_ATTR_ALIGN;
int main(void)
{
    uint32_t i;
    q63_t ret;

    for (i = 0; i < SIZE; i++)
    {
        srcq15[i] = (q15_t)(rand() >> 16);
        srcq15_2[i] = (q15_t)(rand() >> 16);
    }

#ifdef PF_COUNTER
    startPFM;
#endif
    ret = riscv_dsp_dprod_q15(srcq15, srcq15_2, SIZE);
#ifdef PF_COUNTER
    stopPFM;
    readResult();
#endif

    PRINTF_INT64(ret);
    return (int) ret;
}
