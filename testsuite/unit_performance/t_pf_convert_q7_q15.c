#include "riscv_dsp_utils_math.h"

q7_t srcq7[SIZE] FUNC_ATTR_ALIGN;
q15_t outq15[SIZE] FUNC_ATTR_ALIGN;
int main(void)
{
    uint32_t blocksize = SIZE, i;
    q7_t *ptrq7;

    for (ptrq7 = &srcq7[0], i = 0; i < blocksize; i++)
    {
        *ptrq7++ = (q7_t)(rand() >> 24);
    }

#ifdef PF_COUNTER
    startPFM;
#endif
    riscv_dsp_convert_q7_q15(srcq7, outq15, blocksize);
#ifdef PF_COUNTER
    stopPFM;
    readResult();
#endif

    return 0;
}
