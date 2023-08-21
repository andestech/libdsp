#include "riscv_dsp_statistics_math.h"

float32_t fsrcf32[SIZE] FUNC_ATTR_ALIGN;
int main(void)
{
    uint32_t bsize = SIZE;
    uint32_t i;
    float32_t *fptrf32;
    float32_t ret;

    for (fptrf32 = &fsrcf32[0], i = 0; i < bsize; i++)
    {
        *fptrf32++ = ((q15_t)(rand() >> 16)) / 32768.0f;
    }

#ifdef PF_COUNTER
    startPFM;
#endif
    ret = riscv_dsp_var_f32(fsrcf32, bsize);
#ifdef PF_COUNTER
    stopPFM;
    readResult();
#endif

    PRINTF_F32(fsrcf32[0]);
    PRINTF_INT32(fsrcf32[0]);
    return (int) ret;
}
