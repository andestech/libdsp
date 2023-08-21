#include "riscv_dsp_filtering_math.h"

float64_t fsrcf64[BUF_SIZE_F32] FUNC_ATTR_ALIGN, foutf64[BUF_SIZE_F32] FUNC_ATTR_ALIGN;
float64_t bstf64[BIQUAD_DF2T_STATE_LEN] FUNC_ATTR_ALIGN = {0};
float64_t bcof64[COE_LEN] FUNC_ATTR_ALIGN;
int main(void)
{
    total_samples = SIZE_F32;
    uint32_t i, j;
    const uint32_t bsize = BUF_SIZE_F32;
    const uint8_t bstage = BIQUAD_STAGE;

    bcof64[0] = 0.1;
    bcof64[4] = -0.1;
    bcof64[8] = -0.1;
    bcof64[12] = 0.1;
    bcof64[1] = 0.2;
    bcof64[5] = 0.1;
    bcof64[9] = -0.1;
    bcof64[13] = -0.1;
    bcof64[2] = 0.3;
    bcof64[6] = 0.1;
    bcof64[10] = 0.1;
    bcof64[14] = -0.1;
    bcof64[3] = -0.4;
    bcof64[7] = 0.1;
    bcof64[11] = 0.1;
    riscv_dsp_bq_df2T_f64_t bsif64 = {bstage, bstf64, bcof64};
    float64_t *fptrf64;
    acc_result result = {0, 0};

    for (j = 0; j < TEST_LOOP_F32; j++)
    {
        for (fptrf64 = &fsrcf64[0], i = 0; i < bsize; i++)
        {
            *fptrf64++ = ((q15_t)(rand() >> 16)) / 32768.0f;
        }
#ifdef PF_COUNTER
        startPFM;
#endif
        riscv_dsp_bq_df2T_f64(&bsif64, fsrcf64, foutf64, bsize);
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
