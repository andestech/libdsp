#include "riscv_dsp_controller_math.h"

float32_t a[BUF_SIZE_F32] FUNC_ATTR_ALIGN;
riscv_dsp_pid_f32_t Si;
int main(void)
{
    total_samples = SIZE_F32;
    int32_t resetState = RESET;
    float32_t ret = 0.0f;
    uint32_t i, j;
    acc_result result = {0, 0};
    Si.Kp = KP_VAL;
    Si.Ki = KI_VAL;
    Si.Kd = KD_VAL;

    for (j = 0; j < TEST_LOOP_F32; j++)
    {
        for (i = 0; i < BUF_SIZE_F32; i++)
        {
            a[i] = ((q15_t)(rand() >> 16)) / 32768.0f;
        }
#ifdef PF_COUNTER
        startPFM;
#endif
        riscv_dsp_init_pid_f32((riscv_dsp_pid_f32_t *)&Si, resetState);
        for (i = 0; i < BUF_SIZE_F32; i++)
        {
            //ret = perf_test_riscv_dsp_pid_f32(&Si, a[i]);
            ret = riscv_dsp_pid_f32(&Si, a[i]);
        }
#ifdef PF_COUNTER
        stopPFM;
        ACC_Result(&result);
#endif
    }

#ifdef PF_COUNTER
    Read_ACC_Result(&result);
#endif

    return ret;
}
