#include "riscv_dsp_filtering_math.h"

q31_t fsrcq31[SIZE] FUNC_ATTR_ALIGN, foutq31[SIZE] FUNC_ATTR_ALIGN;
q31_t listq31[LIIR_STATE_LEN] FUNC_ATTR_ALIGN = {0};
q31_t licoq31[LIIR_STAGE] FUNC_ATTR_ALIGN;
q31_t livcoq31[V_COE_LEN] FUNC_ATTR_ALIGN;
int main(void)
{
    uint32_t i;

    for (i = 0; i < SIZE; i++)
    {
        fsrcq31[i] = (q31_t)rand();
    }

    for (i = 0; i < LIIR_STAGE; i++)
    {
        licoq31[i] = (q31_t)rand();
    }

    for (i = 0; i < V_COE_LEN; i++)
    {
        livcoq31[i] = (q31_t)rand();
    }

    riscv_dsp_liir_q31_t liq31 = {LIIR_STAGE, listq31, licoq31, livcoq31};
#ifdef PF_COUNTER
    startPFM;
#endif
    riscv_dsp_liir_q31(&liq31, fsrcq31, foutq31, SIZE);
#ifdef PF_COUNTER
    stopPFM;
    readResult();
#endif

    return 0;
}
