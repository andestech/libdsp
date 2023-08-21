#include "riscv_dsp_filtering_math.h"

q15_t fsrcq15[SIZE] FUNC_ATTR_ALIGN;
q15_t foutq15[SIZE] FUNC_ATTR_ALIGN;
q15_t reinq15[SIZE] FUNC_ATTR_ALIGN;
q15_t errq15[SIZE] FUNC_ATTR_ALIGN;
q15_t dstq15[FIR_STATE_LEN] FUNC_ATTR_ALIGN = {0};
q15_t dcoq15[FIR_NUM_TAP] FUNC_ATTR_ALIGN;
int main(void)
{
    uint32_t i;
    q15_t uq15 FUNC_ATTR_ALIGN = (q15_t)(MU * 0x7FFF);

    for (i = 0; i < SIZE; i++)
    {
        fsrcq15[i] = (q15_t)(rand() >> 16);
        reinq15[i] = (q15_t)(rand() >> 16);
    }

    for (i = 0; i < FIR_NUM_TAP; i++)
    {
        dcoq15[i] = (q15_t)(rand() >> 16);
    }

    riscv_dsp_lms_q15_t lmq15 = {FIR_NUM_TAP, dstq15, dcoq15, uq15, LMS_SHIFT};
#ifdef PF_COUNTER
    startPFM;
#endif
    riscv_dsp_lms_q15(&lmq15, fsrcq15, reinq15, foutq15, errq15, SIZE);
#ifdef PF_COUNTER
    stopPFM;
    readResult();
#endif

    return 0;
}
