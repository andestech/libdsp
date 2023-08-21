#include "riscv_dsp_complex_math.h"

q31_t srcq31[SIZE] FUNC_ATTR_ALIGN;
q31_t clxoutq31[SIZE / 2] FUNC_ATTR_ALIGN;
int main(void)
{
    uint32_t i;

    for (i = 0; i < SIZE; i++)
    {
        srcq31[i] = (q31_t)rand();
    }

#ifdef PF_COUNTER
    startPFM;
#endif
    riscv_dsp_cmag_sqr_q31(srcq31, clxoutq31, SIZE / 2);
#ifdef PF_COUNTER
    stopPFM;
    readResult();
#endif

    return 0;
}
