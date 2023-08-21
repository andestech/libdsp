#include "riscv_dsp_complex_math.h"

q31_t srcq31[SIZE] FUNC_ATTR_ALIGN;
q31_t srcq31_2[SIZE] FUNC_ATTR_ALIGN;
int main(void)
{
    q63_t clxoutq63, clxinq63;
    uint32_t i;

    for (i = 0; i < SIZE; i++)
    {
        srcq31[i] = (q31_t)rand();
        srcq31_2[i] = (q31_t)rand();
    }

#ifdef PF_COUNTER
    startPFM;
#endif
    riscv_dsp_cdprod_typ2_q31(srcq31, srcq31_2, SIZE / 2, &clxoutq63, &clxinq63);
#ifdef PF_COUNTER
    stopPFM;
    readResult();
#endif

    PRINTF_INT64(clxoutq63);
    PRINTF_INT64(clxinq63);
    return (int) (clxoutq63 + clxinq63);
}
