#include "riscv_dsp_utils_math.h"

float32_t fsrcf32_1[VECDIM * VECNUM] FUNC_ATTR_ALIGN;
float32_t fsrcf32_2[VECDIM] FUNC_ATTR_ALIGN;
float32_t out[VECDIM] FUNC_ATTR_ALIGN;

int main(void)
{
    uint32_t bsize = (VECDIM * VECNUM);
    uint32_t i;
    float32_t ret;

    for (i = 0; i < bsize; i++)
    {
        fsrcf32_1[i] = ((q15_t)(rand() >> 16)) / 32768.0f;
    }
    for (i = 0; i < VECDIM; i++)
    {
        fsrcf32_1[i] = ((q15_t)(rand() >> 16)) / 32768.0f;
    }

#ifdef PF_COUNTER
    startPFM;
#endif
    riscv_dsp_barycenter_f32(fsrcf32_1, fsrcf32_2, out, VECNUM, VECDIM);
#ifdef PF_COUNTER
    stopPFM;
    readResult();
#endif

    PRINTF_F32(fsrcf32_1[0]);
    return (int) ret;
}
