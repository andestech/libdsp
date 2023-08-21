#include "riscv_dsp_filtering_math.h"

q15_t fsrcq15[SIZE] FUNC_ATTR_ALIGN;
q15_t foutq15[SIZE] FUNC_ATTR_ALIGN;
q15_t reinq15[SIZE] FUNC_ATTR_ALIGN;
q15_t errq15[SIZE] FUNC_ATTR_ALIGN;
q15_t dstq15[FIR_STATE_LEN] FUNC_ATTR_ALIGN = {0};
q15_t dcoq15[FIR_NUM_TAP] FUNC_ATTR_ALIGN;
q15_t ene = 0, x0 = 0;

unsigned int postshift = POST_SHIFT;
int main(void)
{
    uint32_t i, j;
    q15_t uq15;

    uq15 = (q15_t) (MU * (1 << 14));

    for (i = 0; i < FIR_NUM_TAP; i++)
    {
        dcoq15[i] = (q15_t)(rand() >> 16);
    }

    for (i = 0; i < SIZE; i++)
    {
        fsrcq15[i] = (q15_t)(rand() >> 16);
        reinq15[i] = (q15_t)(rand() >> 16);
    }

    riscv_dsp_nlms_q15_t nlmq15 = {FIR_NUM_TAP, dstq15, dcoq15, uq15, postshift, ene, x0};
#ifdef PF_COUNTER
    startPFM;
#endif
    riscv_dsp_nlms_q15(&nlmq15, fsrcq15, reinq15, foutq15, errq15, SIZE);
#ifdef PF_COUNTER
    stopPFM;
    readResult();
#endif

    return 0;
}
