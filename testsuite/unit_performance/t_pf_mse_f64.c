#include "riscv_dsp_statistics_math.h"

float64_t src1[SIZE] FUNC_ATTR_ALIGN;
float64_t src2[SIZE] FUNC_ATTR_ALIGN;
int main(void)
{
    uint32_t bsize = SIZE;
    uint32_t i;
    float64_t result;

    for(i = 0; i < bsize; i++)
    {
        src1[i] = ((q15_t)(rand() >> 16)) / 32768.0f;
        src2[i] = ((q15_t)(rand() >> 16)) / 32768.0f;
    }

#ifdef PF_COUNTER
    startPFM;
#endif
    riscv_dsp_mse_f64(src1, src2, bsize, &result);
#ifdef PF_COUNTER
    stopPFM;
    readResult();
#endif

    return (int) result;
}
