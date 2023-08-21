#include "riscv_dsp_filtering_math.h"

q31_t fsrcq31[SIZE] FUNC_ATTR_ALIGN, foutq31[SIZE] FUNC_ATTR_ALIGN;
q31_t lstq31[LFIR_STATE_LEN] FUNC_ATTR_ALIGN = {0};
q31_t lcoq31[LFIR_STATE_LEN] FUNC_ATTR_ALIGN;
int main(void)
{
    uint32_t i;

    for (i = 0; i < SIZE; i++)
    {
        fsrcq31[i] = (q31_t)rand();
    }

    for (i = 0; i < LFIR_STATE_LEN; i++)
    {
        lcoq31[i] = (q31_t)rand();
    }

    riscv_dsp_lfir_q31_t lq31 = {LFIR_STATE_LEN, lstq31, lcoq31};
#ifdef PF_COUNTER
    startPFM;
#endif
    riscv_dsp_lfir_q31(&lq31, fsrcq31, foutq31, SIZE);
#ifdef PF_COUNTER
    stopPFM;
    readResult();
#endif

    return 0;
}
