#include "riscv_dsp_basic_math.h"

q31_t srcq31[SIZE] FUNC_ATTR_ALIGN;
q31_t outq31[SIZE] FUNC_ATTR_ALIGN;
int main(void)
{
    uint32_t blocksize = SIZE, i;
    q31_t *ptrq31;

    for (ptrq31 = &srcq31[0], i = 0; i < blocksize; i++)
    {
        *ptrq31++ = (rand() & 0x1) ? (q31_t) - rand() : (q31_t)rand();

    }

#ifdef PF_COUNTER
    startPFM;
#endif
    riscv_dsp_shift_q31(srcq31, SHIFT_BITS, outq31, blocksize);
#ifdef PF_COUNTER
    stopPFM;
    readResult();
#endif

    return 0;
}
