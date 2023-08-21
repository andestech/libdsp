#include "riscv_dsp_filtering_math.h"

q31_t fsrcq31_1[SIZE]  FUNC_ATTR_ALIGN;
q31_t fsrcq31_2[BLOCKSIZE_2]  FUNC_ATTR_ALIGN;
q31_t convoutq31[CONV_LEN]  FUNC_ATTR_ALIGN;
int main(void)
{
    uint32_t i;

    for (i = 0; i < SIZE; i++)
    {
        fsrcq31_1[i] = (q31_t)rand();
    }

    for (i = 0; i < BLOCKSIZE_2; i++)
    {
        fsrcq31_2[i] = (q31_t)rand();
    }

#ifdef PF_COUNTER
    startPFM;
#endif
    riscv_dsp_conv_q31(fsrcq31_1, SIZE, fsrcq31_2, BLOCKSIZE_2, convoutq31);
#ifdef PF_COUNTER
    stopPFM;
    readResult();
#endif

    return 0;
}
