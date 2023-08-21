#include "riscv_dsp_filtering_math.h"

q7_t fsrcq7[SIZE] FUNC_ATTR_ALIGN;
q7_t foutq7[SIZE] FUNC_ATTR_ALIGN;
q7_t dstq7[FIR_STATE_LEN] FUNC_ATTR_ALIGN = {0};
q7_t dcoq7[FIR_NUM_TAP] FUNC_ATTR_ALIGN;
int main(void)
{
    uint32_t i;

    for (i = 0; i < SIZE; i++)
    {
        fsrcq7[i] = (q7_t)(rand() >> 24);
    }

    //generate coefficient
    for (i = 0; i < FIR_NUM_TAP; i++)
    {
        dcoq7[i] = (q7_t)(rand() >> 24);
    }

    riscv_dsp_fir_q7_t fq7 = {FIR_NUM_TAP, dstq7, dcoq7};
#ifdef PF_COUNTER
    startPFM;
#endif
    riscv_dsp_fir_q7(&fq7, fsrcq7, foutq7, SIZE);
#ifdef PF_COUNTER
    stopPFM;
    readResult();
#endif

    return 0;
}
