#include "riscv_dsp_filtering_math.h"

q31_t sstq31[SIZE + MAX_DELAY] FUNC_ATTR_ALIGN = {0};
q31_t fsrcq31[SIZE] FUNC_ATTR_ALIGN;
q31_t foutq31[SIZE] FUNC_ATTR_ALIGN;
q31_t scrainq31[SIZE] FUNC_ATTR_ALIGN;
q31_t tapdly[NUM_TAP] FUNC_ATTR_ALIGN;
q31_t dcoq31[NUM_TAP] FUNC_ATTR_ALIGN;
int main(void)
{
    const uint32_t bsize = SIZE, numtap = NUM_TAP;
    const uint16_t maxdly = MAX_DELAY;
    uint32_t i;

    dcoq31[0] = 0x4000000;
    dcoq31[1] = 0x8000000;
    dcoq31[2] = 0x2000000;
    dcoq31[3] = 0x20000000;

    tapdly[0] = 3;
    tapdly[1] = 2;
    tapdly[2] = 0;
    tapdly[3] = 1;

    for (i = 0; i < bsize; i++)
    {
        fsrcq31[i] = (q31_t)rand();
    }

    riscv_dsp_spafir_q31_t sq31 = {numtap, 0, sstq31, dcoq31, maxdly, tapdly};
#ifdef PF_COUNTER
    startPFM;
#endif
    riscv_dsp_spafir_q31(&sq31, fsrcq31, foutq31, scrainq31, bsize);
#ifdef PF_COUNTER
    stopPFM;
    readResult();
#endif

    return 0;
}
