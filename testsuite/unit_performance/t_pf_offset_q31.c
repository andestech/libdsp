#include "riscv_dsp_basic_math.h"

q31_t srcq31[SIZE] FUNC_ATTR_ALIGN;
q31_t outq31[SIZE] FUNC_ATTR_ALIGN;
int main(void)
{
    uint32_t blocksize = SIZE, i;
    q31_t *ptrq31;
    q31_t offsetq31 = 45678;

    for (ptrq31 = &srcq31[0], i = 0; i < blocksize; i++)
    {
        *ptrq31++ = (q31_t)rand();
    }

#ifdef PF_COUNTER
    startPFM;
#endif
    riscv_dsp_offset_q31(srcq31, offsetq31, outq31, blocksize);
#ifdef PF_COUNTER
    stopPFM;
    readResult();
#endif

    return 0;
}
