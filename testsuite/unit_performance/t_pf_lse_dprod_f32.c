#include "riscv_dsp_statistics_math.h"

float32_t fsrcf32_1[SIZE] FUNC_ATTR_ALIGN;
float32_t fsrcf32_2[SIZE] FUNC_ATTR_ALIGN;
float32_t buf[SIZE] FUNC_ATTR_ALIGN;
int main(void)
{
    uint32_t bsize = SIZE;
    uint32_t i;
    float32_t ret;

    for (i = 0; i < bsize; i++)
    {
        fsrcf32_1[i] = ((q15_t)(rand() >> 16)) / 32768.0f;
        fsrcf32_2[i] = ((q15_t)(rand() >> 16)) / 32768.0f;
    }

#ifdef PF_COUNTER
    startPFM;
#endif
    ret = riscv_dsp_lse_dprod_f32(fsrcf32_1, fsrcf32_2, bsize, buf);
#ifdef PF_COUNTER
    stopPFM;
    readResult();
#endif

    PRINTF_F32(fsrcf32_1[0]);
    return (int) ret;
}
