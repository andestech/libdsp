#include "riscv_dsp_filtering_math.h"

q15_t fsrcq15[SIZE] FUNC_ATTR_ALIGN;
q15_t foutq15[SIZE] FUNC_ATTR_ALIGN;
q15_t listq15[LIIR_STATE_LEN] FUNC_ATTR_ALIGN = {0};
q15_t licoq15[LIIR_STAGE] FUNC_ATTR_ALIGN;
q15_t livcoq15[V_COE_LEN] FUNC_ATTR_ALIGN;
int main(void)
{
    uint32_t i;

    for (i = 0; i < SIZE; i++)
    {
        fsrcq15[i] = (q15_t)(rand() >> 16);
    }

    for (i = 0; i < LIIR_STAGE; i++)
    {
        licoq15[i] = (q15_t)(rand() >> 16);
    }

    for (i = 0; i < V_COE_LEN; i++)
    {
        livcoq15[i] = (q15_t)(rand() >> 16);
    }

    riscv_dsp_liir_q15_t liq15 = {LIIR_STAGE, listq15, licoq15, livcoq15};
#ifdef PF_COUNTER
    startPFM;
#endif
    riscv_dsp_liir_q15(&liq15, fsrcq15, foutq15, SIZE);
#ifdef PF_COUNTER
    stopPFM;
    readResult();
#endif

    return 0;
}
