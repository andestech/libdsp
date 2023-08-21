#include "riscv_dsp_complex_math.h"

q31_t srcq31[SIZE] FUNC_ATTR_ALIGN;
q31_t clxinq31[SIZE / 2] FUNC_ATTR_ALIGN;
q31_t outq31[SIZE] FUNC_ATTR_ALIGN;
int main(void)
{
    uint32_t i;

    for (i = 0; i < SIZE / 2; i++)
    {
        srcq31[2 * i] = (q31_t)rand();
        srcq31[(2 * i) + 1] = (q31_t)rand();
        clxinq31[i] = (q31_t)rand();
    }

#ifdef PF_COUNTER
    startPFM;
#endif
    riscv_dsp_cmul_real_q31(srcq31, clxinq31, outq31, SIZE / 2);
#ifdef PF_COUNTER
    stopPFM;
    readResult();
#endif

    return 0;
}
