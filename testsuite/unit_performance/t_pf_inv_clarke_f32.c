#include "riscv_dsp_controller_math.h"

float32_t srcf32_1[BUF_SIZE_F32]  FUNC_ATTR_ALIGN;
float32_t srcf32_2[BUF_SIZE_F32]  FUNC_ATTR_ALIGN;
float32_t outf32_1[BUF_SIZE_F32] FUNC_ATTR_ALIGN, outf32_2[BUF_SIZE_F32] FUNC_ATTR_ALIGN;
int main(void)
{
    total_samples = SIZE_F32;
    uint32_t i, j;
    acc_result result = {0, 0};

    for (j = 0; j < TEST_LOOP_F32; j++)
    {
        for (i = 0; i < BUF_SIZE_F32; i++)
        {
            srcf32_1[i] = ((q15_t)(rand() >> 16)) / 32768.0f;
            srcf32_2[i] = ((q15_t)(rand() >> 16)) / 32768.0f;
        }
#ifdef PF_COUNTER
        startPFM;
#endif
        for (i = 0; i < BUF_SIZE_F32; i++)
        {
            riscv_dsp_inv_clarke_f32(srcf32_1[i], srcf32_2[i], &outf32_1[i], &outf32_2[i]);
        }
#ifdef PF_COUNTER
        stopPFM;
        ACC_Result(&result);
#endif
    }

#ifdef PF_COUNTER
    Read_ACC_Result(&result);
#endif

    for (i = 0; i < 2; i++)
    {
        PRINTF_F32(outf32_1[i]);
        PRINTF_F32(outf32_2[i]);
    }
    return (outf32_2[1]);
}
