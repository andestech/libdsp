#include "riscv_dsp_filtering_math.h"

q31_t fsrcq31[SIZE] FUNC_ATTR_ALIGN;
q31_t foutq31[SIZE] FUNC_ATTR_ALIGN;
q31_t dstq31[FIR_STATE_LEN] FUNC_ATTR_ALIGN = {0};
q31_t dcoq31[FIR_NUM_TAP] FUNC_ATTR_ALIGN;
int main(void)
{
    uint32_t i;

    for (i = 0; i < SIZE; i++)
    {
        fsrcq31[i] = (q31_t)rand();
    }

    //generate coefficient
    for (i = 0; i < FIR_NUM_TAP; i++)
    {
        dcoq31[i] = (q31_t)rand();
    }

    riscv_dsp_fir_q31_t fq31 = {FIR_NUM_TAP, dstq31, dcoq31};
#ifdef PF_COUNTER
    startPFM;
#endif
    riscv_dsp_fir_fast_q31(&fq31, fsrcq31, foutq31, SIZE);
#ifdef PF_COUNTER
    stopPFM;
    readResult();
#endif

    return 0;
}
