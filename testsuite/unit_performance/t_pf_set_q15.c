#include "riscv_dsp_utils_math.h"

q15_t outq15[SIZE] FUNC_ATTR_ALIGN;
int main(void)
{
    q15_t srcq15;
    srcq15 = (q15_t)(rand());

#ifdef PF_COUNTER
    startPFM;
#endif
    riscv_dsp_set_q15(srcq15, outq15, SIZE);
#ifdef PF_COUNTER
    stopPFM;
    readResult();
#endif

    return 0;
}

