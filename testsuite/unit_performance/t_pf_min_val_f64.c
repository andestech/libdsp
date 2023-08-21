#include "riscv_dsp_statistics_math.h"

float64_t fsrcf64[BUF_SIZE_F32] FUNC_ATTR_ALIGN;

int main(void)
{
    uint32_t i;
    float64_t *fptrf64;
    float64_t ret;

    for (fptrf64 = &fsrcf64[0], i = 0; i < BUF_SIZE_F32; i++)
    {
        *fptrf64 = ((q15_t)(rand() >> 16)) / 32768.0f;
        if (i == (int)(SIZE / 2))
        {
            *fptrf64 = -3.0f;
        }
        fptrf64++;
    }
#ifdef PF_COUNTER
    startPFM;
#endif
    ret = riscv_dsp_min_val_f64(fsrcf64, BUF_SIZE_F32);
#ifdef PF_COUNTER
    stopPFM;
    readResult();
#endif

    return (int) ret;
}
