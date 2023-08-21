#include "riscv_dsp_filtering_math.h"

q15_t sstq15[SIZE + MAX_DELAY] FUNC_ATTR_ALIGN = {0};
q15_t fsrcq15[SIZE] FUNC_ATTR_ALIGN;
q15_t foutq15[SIZE] FUNC_ATTR_ALIGN;
q15_t scrainq15[SIZE] FUNC_ATTR_ALIGN;
q31_t scratchout[SIZE]  FUNC_ATTR_ALIGN;
q31_t tapdly[NUM_TAP];
q15_t dcoq15[NUM_TAP] FUNC_ATTR_ALIGN;
int main(void)
{
    const uint32_t bsize = SIZE, numtap = NUM_TAP;
    const uint16_t maxdly = MAX_DELAY;
    uint32_t i;

    tapdly[0] = 3;
    tapdly[1] = 2;
    tapdly[2] = 0;
    tapdly[3] = 1;

    dcoq15[0] = 0x400;
    dcoq15[1] = 0x800;
    dcoq15[2] = 0x200;
    dcoq15[3] = 0x2000;

    for (i = 0; i < bsize; i++)
    {
        fsrcq15[i] = (q15_t)(rand() >> 16);
    }

    riscv_dsp_spafir_q15_t sq15 = {numtap, 0, sstq15, dcoq15, maxdly, tapdly};
#ifdef PF_COUNTER
    startPFM;
#endif
    riscv_dsp_spafir_q15(&sq15, fsrcq15, foutq15, scrainq15, scratchout, bsize);
#ifdef PF_COUNTER
    stopPFM;
    readResult();
#endif

    return 0;
}
