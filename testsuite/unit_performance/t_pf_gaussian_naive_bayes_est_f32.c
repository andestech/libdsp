#include "riscv_dsp_statistics_math.h"

float32_t fsrcf32[NUMOFDIM] FUNC_ATTR_ALIGN;
float32_t mean[NUMOFCLASS * NUMOFDIM];
float32_t var[NUMOFCLASS * NUMOFDIM];
float32_t prior[NUMOFCLASS];
float32_t buffer[NUMOFCLASS];
int main(void)
{
    total_samples = NUMOFDIM;
    uint32_t i, j, idx;

    for (i = 0; i < NUMOFDIM; i++)
    {
        fsrcf32[i] = ((q15_t)(rand() >> 16)) / 32768.0f;
    }
    for (i = 0; i < NUMOFCLASS * NUMOFDIM; i++)
    {
        mean[i] = ((q15_t)(rand() >> 16)) / 32768.0f;
        var[i] = ((q15_t)(rand() >> 16)) / 32768.0f;
    }
    for (i = 0; i < NUMOFCLASS; i++)
    {
        prior[i] = ((q15_t)(rand() >> 16)) / 32768.0f;
    }

    riscv_dsp_gaussian_naivebayes_f32_t Si = {NUMOFDIM, NUMOFCLASS, mean, var, prior, ADDVAR};

#ifdef PF_COUNTER
    startPFM;
#endif
    idx = riscv_dsp_gaussian_naive_bayes_est_f32(&Si, fsrcf32, buffer);
#ifdef PF_COUNTER
    stopPFM;
    readResult();
#endif

    return 0;
}
