#include "riscv_dsp_complex_math.h"

q15_t srcq15[SIZE] FUNC_ATTR_ALIGN;
q15_t outq15[SIZE] FUNC_ATTR_ALIGN;
int main(void)
{
    uint32_t blocksize = SIZE, i;
    q15_t *ptrq15;

    for (ptrq15 = &srcq15[0], i = 0; i < blocksize; i++)
    {
        *ptrq15++ = (q15_t)(rand() >> 16);
    }

#ifdef PF_COUNTER
    startPFM;
#endif
    riscv_dsp_cconj_q15(srcq15, outq15, blocksize / 2);
#ifdef PF_COUNTER
    stopPFM;
    readResult();
#endif

    return 0;
}
