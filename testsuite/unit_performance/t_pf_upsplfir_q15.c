#include "riscv_dsp_filtering_math.h"

q15_t fsrcq15[SIZE] FUNC_ATTR_ALIGN;
q15_t uoutq15[SIZE * UPSPL_FACTOR] FUNC_ATTR_ALIGN;
q15_t ustq15[UPSPL_STATE_LEN] FUNC_ATTR_ALIGN = {0};
q15_t ucoq15[UPSPL_COE_LEN] FUNC_ATTR_ALIGN;
int main(void)
{
    uint32_t i;

    for (i = 0; i < SIZE; i++)
    {
        fsrcq15[i] = (q15_t)(rand() >> 16);
    }

    //generate coefficient
    for (i = 0; i < UPSPL_COE_LEN; i++)
    {
        ucoq15[i] = (q15_t)(rand() >> 16);
    }

    riscv_dsp_upsplfir_q15_t uuq15 = {UPSPL_FACTOR, PHASE_LEN, ucoq15, ustq15};
#ifdef PF_COUNTER
    startPFM;
#endif
    riscv_dsp_upsplfir_q15(&uuq15, fsrcq15, uoutq15, SIZE);
#ifdef PF_COUNTER
    stopPFM;
    readResult();
#endif

    return 0;
}
