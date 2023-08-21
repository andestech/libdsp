#include "riscv_dsp_basic_math.h"

int main(void)
{
    total_samples = SAMPLES;
    q31_t inputy, inputx, ret = 0;
    acc_result result = {0, 0};
    uint32_t j;

    for(j = 0; j < SAMPLES; j++)
    {  
        inputy = (q31_t) rand();
        inputx = (q31_t) rand();
#ifdef PF_COUNTER
        startPFM;
#endif    
        ret = riscv_dsp_div_q31(inputx, inputy);
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

