#include "riscv_dsp_utils_math.h"

q7_t outq7[SIZE] FUNC_ATTR_ALIGN;
int main(void)
{
    q7_t srcq7;
    srcq7 = (q7_t)(rand() >> 24);

#ifdef PF_COUNTER
    startPFM;
#endif
    riscv_dsp_set_q7(srcq7, outq7, SIZE);
#ifdef PF_COUNTER
    stopPFM;
    readResult();
#endif

    return 0;
}
