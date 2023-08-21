#include "riscv_dsp_utils_math.h"

int main(void)
{
    q31_t ret, input;
    acc_result result = {0, 0};
    uint32_t j;
    total_samples = SAMPLES;
    
    for(j = 0; j < SAMPLES; j++)
    {  
        input = j * 65536;
#ifdef PF_COUNTER
        startPFM;
#endif
        ret =  riscv_dsp_atan_q31(input);
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
