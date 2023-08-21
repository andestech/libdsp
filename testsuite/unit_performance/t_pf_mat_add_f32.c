#include "riscv_dsp_matrix_math.h"

float32_t matsrcf32_1[MAT_SIZE_F32]  FUNC_ATTR_ALIGN;
float32_t matsrcf32_2[MAT_SIZE_F32] FUNC_ATTR_ALIGN;
float32_t matoutf32[MAT_SIZE_F32] FUNC_ATTR_ALIGN;
int main(void)
{
    total_samples =  MAT_SIZE_F32 * 64;
    uint32_t i, j;
    acc_result result = {0, 0};

    for (j = 0; j < 64; j++)
    {
        for (i = 0; i < MAT_SIZE_F32; i++)
        {
            matsrcf32_1[i] = ((q15_t)(rand() >> 16)) / 32768.0f;
            matsrcf32_2[i] = ((q15_t)(rand() >> 16)) / 32768.0f;
        }
#ifdef PF_COUNTER
        startPFM;
#endif
        riscv_dsp_mat_add_f32(matsrcf32_1, matsrcf32_2, matoutf32, ROW_F32, COL_F32);
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
