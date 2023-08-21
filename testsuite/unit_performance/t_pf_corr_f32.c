#include "riscv_dsp_filtering_math.h"

float32_t fsrcf32_1[SIZE]  FUNC_ATTR_ALIGN;
float32_t fsrcf32_2[BLOCKSIZE_2]  FUNC_ATTR_ALIGN;
float32_t convoutf32[2 * SIZE - 1]  FUNC_ATTR_ALIGN;
int main(void)
{
    uint32_t i;

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
    riscv_dsp_corr_f32(fsrcf32_1, SIZE, fsrcf32_2, BLOCKSIZE_2, convoutf32);
#ifdef PF_COUNTER
    stopPFM;
    readResult();
#endif

    return 0;
}
