#include "riscv_dsp_statistics_math.h"

float32_t fsrcf32[BUF_SIZE_F32] FUNC_ATTR_ALIGN;

int main(void)
{
    uint32_t i;
    float32_t *fptrf32;
    float32_t ret;

    for (fptrf32 = &fsrcf32[0], i = 0; i < BUF_SIZE_F32; i++)
    {
        *fptrf32 = ((q15_t)(rand() >> 16)) / 32768.0f;
        if (i == (int)(SIZE / 2))
        {
            *fptrf32 = 3.0f;
        }
        fptrf32++;
    }
#ifdef PF_COUNTER
    startPFM;
#endif
    ret = riscv_dsp_max_val_f32(fsrcf32, BUF_SIZE_F32);
#ifdef PF_COUNTER
    stopPFM;
    readResult();
#endif
    return (int) ret;
}
