#include "riscv_dsp_filtering_math.h"

q15_t fsrcq15_1[SIZE]  FUNC_ATTR_ALIGN;
q15_t fsrcq15_2[BLOCKSIZE_2]  FUNC_ATTR_ALIGN;
q15_t convoutq15[CONV_LEN]  FUNC_ATTR_ALIGN;
int main(void)
{
    uint32_t i;
    total_samples = CONVSIZE;

    for (i = 0; i < SIZE; i++)
    {
        fsrcq15_1[i] = (q15_t)(rand() >> 16);
    }

    for (i = 0; i < BLOCKSIZE_2; i++)
    {
        fsrcq15_2[i] = (q15_t)(rand() >> 16);
    }

#ifdef PF_COUNTER
    startPFM;
#endif
    riscv_dsp_conv_partial_q15(fsrcq15_1, SIZE, fsrcq15_2, BLOCKSIZE_2, convoutq15, STARTINDEX, CONVSIZE);
#ifdef PF_COUNTER
    stopPFM;
    readResult();
#endif

    return 0;
}
