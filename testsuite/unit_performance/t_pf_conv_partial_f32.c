#include "riscv_dsp_filtering_math.h"

float32_t fsrcf32_1[SIZE]  FUNC_ATTR_ALIGN;
float32_t fsrcf32_2[BLOCKSIZE_2]  FUNC_ATTR_ALIGN;
float32_t convoutf32[CONV_LEN]  FUNC_ATTR_ALIGN;
int main(void)
{
    total_samples = CONVSIZE;
    uint32_t i;
    int status;

    for (i = 0; i < SIZE; i++)
    {
        fsrcf32_1[i] = ((q15_t)(rand() >> 16)) / 32768.0f;
    }

    for (i = 0; i < BLOCKSIZE_2; i++)
    {
        fsrcf32_2[i] = ((q15_t)(rand() >> 16)) / 32768.0f;
    }

#ifdef PF_COUNTER
    startPFM;
#endif
    status = riscv_dsp_conv_partial_f32(fsrcf32_1, SIZE, fsrcf32_2, BLOCKSIZE_2, convoutf32, STARTINDEX, CONVSIZE);
#ifdef PF_COUNTER
    stopPFM;
    readResult();
#endif
    if (status == -1)
        return -1;
    else
        return 0;
}
