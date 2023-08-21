#include "riscv_dsp_utils_math.h"

q15_t srcq15[SIZE] FUNC_ATTR_ALIGN;
q31_t outq31[SIZE]  FUNC_ATTR_ALIGN;
int main(void)
{
    uint32_t blocksize = SIZE, i;
    q15_t *ptrq15;

    for (ptrq15 = &srcq15[0], i = 0; i < blocksize; i++)
    {
        *ptrq15++ = (q15_t)(rand());
    }

#ifdef PF_COUNTER
    startPFM;
#endif
    riscv_dsp_convert_q15_q31(srcq15, outq31, blocksize);
#ifdef PF_COUNTER
    stopPFM;
    readResult();
#endif

    return 0;
}
