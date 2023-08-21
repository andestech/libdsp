#include "riscv_dsp_statistics_math.h"

float64_t fsrcf32[BUF_SIZE_F32] FUNC_ATTR_ALIGN;

int main(void)
{
    total_samples = SIZE_F32;
    uint32_t pindex;
    uint32_t i, j;
    float64_t *fptrf32;
    float64_t ret;
    acc_result result = {0, 0};

    for (j = 0; j < TEST_LOOP_F32; j++)
    {
        for (fptrf32 = &fsrcf32[0], i = 0; i < BUF_SIZE_F32; i++)
        {
            *fptrf32++ = ((q15_t)(rand() >> 16)) / 32768.0f;
            if (i == (int)(SIZE / 2)) {
                *fptrf32 = ((q15_t)(2147483647 >> 16)) / 32768.0f;
            }
        }
#ifdef PF_COUNTER
        startPFM;
#endif
        ret = riscv_dsp_absmax_f64(fsrcf32, BUF_SIZE_F32, &pindex);
#ifdef PF_COUNTER
        stopPFM;
        ACC_Result(&result);
#endif
    }

#ifdef PF_COUNTER
    Read_ACC_Result(&result);
#endif

    PRINTF_F32(fsrcf32[0]);
    PRINTF_INT32(fsrcf32[0]);
    return (int) ret;
}
