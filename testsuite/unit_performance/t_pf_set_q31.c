#include "riscv_dsp_utils_math.h"

q31_t outq31[SIZE] FUNC_ATTR_ALIGN;
int main(void)
{
    q31_t srcq31;

    srcq31 = (rand() & 0x1) ? (q31_t) - rand() : (q31_t)rand();

#ifdef PF_COUNTER
    startPFM;
#endif
    riscv_dsp_set_q31(srcq31, outq31, SIZE);
#ifdef PF_COUNTER
    stopPFM;
    readResult();
#endif

    return 0;
}
