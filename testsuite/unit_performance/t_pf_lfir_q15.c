#include "riscv_dsp_filtering_math.h"

q15_t fsrcq15[SIZE] FUNC_ATTR_ALIGN;
q15_t foutq15[SIZE] FUNC_ATTR_ALIGN;
q15_t lstq15[LFIR_STATE_LEN] FUNC_ATTR_ALIGN = {0};
q15_t lcoq15[LFIR_STATE_LEN] FUNC_ATTR_ALIGN;
int main(void)
{
    uint32_t i;

    for (i = 0; i < SIZE; i++)
    {
        fsrcq15[i] = (q15_t)(rand() >> 16);
    }

    for (i = 0; i < LFIR_STATE_LEN; i++)
    {
        lcoq15[i] = (q15_t)(rand() >> 16);
    }

    riscv_dsp_lfir_q15_t lq15 = {LFIR_STATE_LEN, lstq15, lcoq15};
#ifdef PF_COUNTER
    startPFM;
#endif
    riscv_dsp_lfir_q15(&lq15, fsrcq15, foutq15, SIZE);
#ifdef PF_COUNTER
    stopPFM;
    readResult();
#endif

    return 0;
}
