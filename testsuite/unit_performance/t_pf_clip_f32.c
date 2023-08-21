#include "riscv_dsp_basic_math.h"

float32_t srcf32[SIZE] FUNC_ATTR_ALIGN;
float32_t outf32[SIZE] FUNC_ATTR_ALIGN;
int main(void)
{
    uint32_t i;
    float32_t high = 0.5;
    float32_t low = -0.5;

    for (i = 0; i < SIZE; i++)
    {
        srcf32[i] = gen_frandom();
    }
#ifdef PF_COUNTER
    startPFM;
#endif
    riscv_dsp_clip_f32(srcf32, outf32, low, high, SIZE);
#ifdef PF_COUNTER
    stopPFM;
    readResult();
#endif

    return 0;
}
