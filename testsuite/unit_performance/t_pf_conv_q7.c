#include "riscv_dsp_filtering_math.h"

q7_t fsrcq7_1[SIZE] FUNC_ATTR_ALIGN;
q7_t fsrcq7_2[BLOCKSIZE_2] FUNC_ATTR_ALIGN;
q7_t convoutq7[CONV_LEN] FUNC_ATTR_ALIGN;
int main(void)
{
    uint32_t i;

    for (i = 0; i < SIZE; i++)
    {
        fsrcq7_1[i] = (q7_t)(rand() >> 24);
    }

    for (i = 0; i < BLOCKSIZE_2; i++)
    {
        fsrcq7_2[i] = (q7_t)(rand() >> 24);
    }

#ifdef PF_COUNTER
    startPFM;
#endif
    riscv_dsp_conv_q7(fsrcq7_1, SIZE, fsrcq7_2, BLOCKSIZE_2, convoutq7);
#ifdef PF_COUNTER
    stopPFM;
    readResult();
#endif

    return 0;
}
