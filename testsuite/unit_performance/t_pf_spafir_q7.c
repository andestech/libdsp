#include "riscv_dsp_filtering_math.h"

q7_t sstq7[SIZE + MAX_DELAY] FUNC_ATTR_ALIGN = {0};
q7_t fsrcq7[SIZE] FUNC_ATTR_ALIGN;
q7_t foutq7[SIZE] FUNC_ATTR_ALIGN;
q7_t scrainq7[SIZE] FUNC_ATTR_ALIGN;
q31_t scratchout[SIZE];
q31_t tapdly[NUM_TAP];
q7_t dcoq7[NUM_TAP] FUNC_ATTR_ALIGN;
int main(void)
{
    const uint32_t bsize = SIZE, numtap = NUM_TAP;
    const uint16_t maxdly = MAX_DELAY;
    uint32_t i;

    tapdly[0] = 3;
    tapdly[1] = 2;
    tapdly[2] = 0;
    tapdly[3] = 1;

    dcoq7[0] = 0x4;
    dcoq7[1] = 0x4;
    dcoq7[2] = 0x4;
    dcoq7[3] = 0x4;

    for (i = 0; i < bsize; i++)
    {
        fsrcq7[i] = (q7_t)(rand() >> 24);
    }

    riscv_dsp_spafir_q7_t sq7 = {numtap, 0, sstq7, dcoq7, maxdly, tapdly};
#ifdef PF_COUNTER
    startPFM;
#endif
    riscv_dsp_spafir_q7(&sq7, fsrcq7, foutq7, scrainq7, scratchout, bsize);
#ifdef PF_COUNTER
    stopPFM;
    readResult();
#endif

    return 0;
}
