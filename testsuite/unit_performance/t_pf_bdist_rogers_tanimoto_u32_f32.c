#include "riscv_dsp_distance_math.h"

uint32_t src_1[SIZE] FUNC_ATTR_ALIGN;
uint32_t src_2[SIZE] FUNC_ATTR_ALIGN;
int main(void)
{
    uint32_t i, j;

    for (i = 0; i < SIZE; i++)
    {
        src_1[i] = (uint32_t)rand();
        src_2[i] = (uint32_t)rand();
    }
#ifdef PF_COUNTER
    startPFM;
#endif
    float32_t out = riscv_dsp_bdist_rogers_tanimoto_u32_f32(src_1, src_2, SIZE);
#ifdef PF_COUNTER
    stopPFM;
    readResult();
#endif

    return (int)out;
}
