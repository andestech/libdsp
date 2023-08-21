#include "riscv_dsp_utils_math.h"

float32_t outf32[BUF_SIZE_F32] FUNC_ATTR_ALIGN;
int main(void)
{
    total_samples = SIZE_F32;
    float32_t srcf32;
    srcf32 = ((q15_t)(rand() >> 16)) / 32768.0f;
    acc_result result = {0, 0};
    uint32_t j;

    for (j = 0; j < TEST_LOOP_F32; j++)
    {
#ifdef PF_COUNTER
        startPFM;
#endif
        riscv_dsp_set_f32(srcf32, outf32, BUF_SIZE_F32);
#ifdef PF_COUNTER
        stopPFM;
        ACC_Result(&result);
#endif
    }

#ifdef PF_COUNTER
    Read_ACC_Result(&result);
#endif

    return 0;
}
