#include "riscv_dsp_utils_math.h"

q31_t srcq31[SIZE]  FUNC_ATTR_ALIGN;
q15_t outq15[SIZE] FUNC_ATTR_ALIGN;
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
    riscv_dsp_convert_q31_q15(srcq31, outq15, blocksize);
#ifdef PF_COUNTER
    stopPFM;
    readResult();
#endif

    return 0;
}
