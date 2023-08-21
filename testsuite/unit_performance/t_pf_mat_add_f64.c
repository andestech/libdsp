#include "riscv_dsp_matrix_math.h"

float64_t matsrcf64[MAT_SIZE_F64] FUNC_ATTR_ALIGN;
float64_t matsrcf64_2[MAT_SIZE_F64] FUNC_ATTR_ALIGN;
float64_t matoutf64[MAT_SIZE_F64] FUNC_ATTR_ALIGN;
int main(void)
{
    total_samples = MAT_SIZE_F64 * 64;
    uint32_t i, j;
    acc_result result = {0, 0};

    for (j = 0; j < 64; j++)
    {
        for (i = 0; i < MAT_SIZE_F64; i++)
        {
            matsrcf64[i] = ((q15_t)(rand() >> 16)) / 32768.0f;
            matsrcf64_2[i] = ((q15_t)(rand() >> 16)) / 32768.0f;
        }
#ifdef PF_COUNTER
        startPFM;
#endif
        riscv_dsp_mat_add_f64(matsrcf64, matsrcf64_2, matoutf64, ROW_F64, COL_F64);
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
