#include "riscv_dsp_filtering_math.h"

q15_t fsrcq15[SIZE] FUNC_ATTR_ALIGN;
q15_t doutq15[SIZE / DCM_FACTOR] FUNC_ATTR_ALIGN;
q15_t dstq15[FIR_STATE_LEN] FUNC_ATTR_ALIGN = {0};
q15_t dcoq15[FIR_NUM_TAP] FUNC_ATTR_ALIGN;
int main(void)
{
    uint32_t i;

    for (i = 0; i < SIZE; i++)
    {
        fsrcq15[i] = (q15_t)(rand() >> 16);
    }
    //generate coefficient
    for (i = 0; i < FIR_NUM_TAP; i++)
    {
        dcoq15[i] = (q15_t)(rand() >> 16);
    }

    riscv_dsp_dcmfir_q15_t dq15 = {DCM_FACTOR, FIR_NUM_TAP, dcoq15, dstq15};
#ifdef PF_COUNTER
    startPFM;
#endif

    riscv_dsp_dcmfir_fast_q15(&dq15, fsrcq15, doutq15, SIZE);
#ifdef PF_COUNTER
    stopPFM;
    readResult();
#endif

    return 0;
}
