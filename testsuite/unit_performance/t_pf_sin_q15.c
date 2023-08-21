#include "riscv_dsp_utils_math.h"

int main(void)
{
    q15_t ret, input;
    q31_t tmp;
    acc_result result = {0, 0};
    int32_t j;
    total_samples = SAMPLES;

    // -pi to pi range
    for (j = -SAMPLES; j < SAMPLES; j += 2)
    {
        tmp = j * (interval_q * 32768.0f);

        if (tmp >= 32768)
            input = 32767;
        else
            input = (q15_t) tmp;
#ifdef PF_COUNTER
        startPFM;
#endif
        ret = riscv_dsp_sin_q15(input);
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
