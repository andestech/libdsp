#include "riscv_dsp_complex_math.h"
#include "riscv_dsp_utils_math.h"

q31_t srcq31[SIZE] FUNC_ATTR_ALIGN;
q31_t clxoutq31[SIZE / 2]  FUNC_ATTR_ALIGN;
int main(void)
{
    uint32_t blocksize = SIZE, i;
    q31_t *ptrq31;

    for (ptrq31 = &srcq31[0], i = 0; i < blocksize; i++)
    {
        *ptrq31++ = (q31_t)rand();
    }

#ifdef PF_COUNTER
    startPFM;
#endif
    riscv_dsp_cmag_q31(srcq31, clxoutq31, blocksize / 2);
#ifdef PF_COUNTER
    stopPFM;
    readResult();
#endif

    return 0;
}
