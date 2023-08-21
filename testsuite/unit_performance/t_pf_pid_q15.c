#include "riscv_dsp_controller_math.h"

q15_t a[SIZE] FUNC_ATTR_ALIGN;
riscv_dsp_pid_q15_t Si;
int main(void)
{
    int32_t resetState = RESET;
    q15_t ret = 0;
    uint32_t i;

    Si.Kp = KP_VAL_Q;
    Si.Ki = KI_VAL_Q;
    Si.Kd = KD_VAL_Q;

    for (i = 0; i < SIZE; i++)
    {
        a[i] = (q15_t)(rand() >> 16);
    }

#ifdef PF_COUNTER
    startPFM;
#endif
    riscv_dsp_init_pid_q15((riscv_dsp_pid_q15_t *)&Si, resetState);
    for (i = 0; i < SIZE; i++)
    {
        ret = riscv_dsp_pid_q15(&Si, a[i]);
    }
#ifdef PF_COUNTER
    stopPFM;
    readResult();
#endif
    return ret;
}
