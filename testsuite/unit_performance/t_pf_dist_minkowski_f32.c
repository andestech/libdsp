#include "riscv_dsp_distance_math.h"

float32_t srcf32_1[SIZE] FUNC_ATTR_ALIGN;
float32_t srcf32_2[SIZE] FUNC_ATTR_ALIGN;
int main(void)
{
    uint32_t i, j;

    for (i = 0; i < SIZE; i++)
    {
        srcf32_1[i] = ((q15_t)(rand() >> 16)) / 32768.0f;
        srcf32_2[i] = ((q15_t)(rand() >> 16)) / 32768.0f;
    }
#ifdef PF_COUNTER
    startPFM;
#endif
    float32_t out = riscv_dsp_dist_minkowski_f32(srcf32_1, srcf32_2, ORDER, SIZE);
#ifdef PF_COUNTER
    stopPFM;
    readResult();
#endif

    return (int)out;
}
