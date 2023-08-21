#include "riscv_dsp_filtering_math.h"

q31_t fsrcq31[SIZE] FUNC_ATTR_ALIGN;
q31_t uoutq31[SIZE * UPSPL_FACTOR] FUNC_ATTR_ALIGN;
q31_t ustq31[UPSPL_STATE_LEN] FUNC_ATTR_ALIGN = {0};
q31_t ucoq31[UPSPL_COE_LEN] FUNC_ATTR_ALIGN;
int main(void)
{
    uint32_t i;

    for (i = 0; i < SIZE; i++)
    {
        fsrcq31[i] = (q31_t)rand();
    }

    //generate coefficient
    for (i = 0; i < UPSPL_COE_LEN; i++)
    {
        ucoq31[i] = (q31_t)rand();
    }

    riscv_dsp_upsplfir_q31_t uuq31 = {UPSPL_FACTOR, PHASE_LEN, ucoq31, ustq31};
#ifdef PF_COUNTER
    startPFM;
#endif
    riscv_dsp_upsplfir_q31(&uuq31, fsrcq31, uoutq31, SIZE);
#ifdef PF_COUNTER
    stopPFM;
    readResult();
#endif

    return 0;
}
