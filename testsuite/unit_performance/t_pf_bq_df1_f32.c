#include "riscv_dsp_filtering_math.h"

float32_t fsrcf32[BUF_SIZE_F32] FUNC_ATTR_ALIGN, foutf32[BUF_SIZE_F32] FUNC_ATTR_ALIGN;
float32_t bstf32[STATE_LEN] FUNC_ATTR_ALIGN = {0};
float32_t bcof32[COE_LEN] FUNC_ATTR_ALIGN;
int main(void)
{
    total_samples = SIZE_F32;
    uint32_t i, j;
    const uint32_t bsize = BUF_SIZE_F32;
    const uint8_t bstage = BIQUAD_STAGE;

    bcof32[0] = 0.1;
    bcof32[4] = -0.1;
    bcof32[8] = -0.1;
    bcof32[12] = 0.1;
    bcof32[1] = 0.2;
    bcof32[5] = 0.1;
    bcof32[9] = -0.1;
    bcof32[13] = -0.1;
    bcof32[2] = 0.3;
    bcof32[6] = 0.1;
    bcof32[10] = 0.1;
    bcof32[14] = -0.1;
    bcof32[3] = -0.4;
    bcof32[7] = 0.1;
    bcof32[11] = 0.1;
    riscv_dsp_bq_df1_f32_t bsif32 = {bstage, bstf32, bcof32};
    float32_t *fptrf32;
    acc_result result = {0, 0};

    for (j = 0; j < TEST_LOOP_F32; j++)
    {
        for (fptrf32 = &fsrcf32[0], i = 0; i < bsize; i++)
        {
            *fptrf32++ = ((q15_t)(rand() >> 16)) / 32768.0f;
        }
#ifdef PF_COUNTER
        startPFM;
#endif
        riscv_dsp_bq_df1_f32(&bsif32, fsrcf32, foutf32, bsize);
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
