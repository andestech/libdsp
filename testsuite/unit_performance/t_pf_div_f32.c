#include "riscv_dsp_basic_math.h"

float32_t srcf32[SIZE] FUNC_ATTR_ALIGN;
float32_t srcf32_2[SIZE] FUNC_ATTR_ALIGN;
float32_t outf32[SIZE] FUNC_ATTR_ALIGN;
int main(void)
{
    uint32_t i;

    for (i = 0; i < SIZE; i++)
    {
        srcf32[i] = ((q15_t)(rand() >> 16)) / 32768.0f;
        srcf32_2[i] = ((q15_t)(rand() >> 16)) / 32768.0f;
    }

#ifdef PF_COUNTER
    startPFM;
#endif
    riscv_dsp_div_f32(srcf32, srcf32_2, outf32, SIZE);
#ifdef PF_COUNTER
    stopPFM;
    readResult();
#endif

    return 0;
}
