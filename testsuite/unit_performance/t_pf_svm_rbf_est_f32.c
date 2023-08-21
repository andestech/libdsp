#include "riscv_dsp_svm_math.h"

float32_t fsrcf32[SVMDIM] FUNC_ATTR_ALIGN;
float32_t dualcoe[SVMVEC] FUNC_ATTR_ALIGN;
float32_t vec[SVMVEC * SVMDIM] FUNC_ATTR_ALIGN ;
int32_t pclass[2] = {3, 7};
int main(void)
{
    uint32_t i;
    float32_t ret;
    int32_t *out;

    for (i = 0; i < SVMDIM; i++)
    {
        fsrcf32[i] = ((q15_t)(rand() >> 16)) / 32768.0f;
    }
    for (i = 0; i < SVMVEC; i++)
    {
        dualcoe[i] = ((q15_t)(rand() >> 16)) / 32768.0f;
    }
    for (i = 0; i < SVMVEC * SVMDIM; i++)
    {
        vec[i] = ((q15_t)(rand() >> 16)) / 32768.0f;
    }

    riscv_dsp_svm_rbf_f32_t Si = {SVMVEC, SVMDIM, INTERCEPT, dualcoe, vec, pclass, GAMMA};

#ifdef PF_COUNTER
    startPFM;
#endif
    riscv_dsp_svm_rbf_est_f32(&Si, fsrcf32, out);
#ifdef PF_COUNTER
    stopPFM;
    readResult();
#endif

    return (int) ret;
}
