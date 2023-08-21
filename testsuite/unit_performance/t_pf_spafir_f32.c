#include "riscv_dsp_filtering_math.h"

float32_t sstf32[BUF_SIZE_F32 + MAX_DELAY] FUNC_ATTR_ALIGN = {0.0};
float32_t fsrcf32[BUF_SIZE_F32] FUNC_ATTR_ALIGN;
float32_t foutf32[BUF_SIZE_F32] FUNC_ATTR_ALIGN;
float32_t scraf32[BUF_SIZE_F32] FUNC_ATTR_ALIGN;
q31_t tapdly[NUM_TAP] FUNC_ATTR_ALIGN;
float32_t dcof32[NUM_TAP] FUNC_ATTR_ALIGN;
int main(void)
{
    total_samples = SIZE_F32;
    const uint32_t bsize = BUF_SIZE_F32, numtap = NUM_TAP;
    const uint16_t maxdly = MAX_DELAY;

    uint32_t i, j;
    acc_result result = {0, 0};

    tapdly[0] = 3;
    tapdly[1] = 2;
    tapdly[2] = 0;
    tapdly[3] = 1;

    dcof32[0] = 0.1;
    dcof32[1] = 0.2;
    dcof32[2] = 0.3;
    dcof32[3] = 0.4;

    riscv_dsp_spafir_f32_t sf32 = {numtap, 0, sstf32, dcof32, maxdly, tapdly};

    for (j = 0; j < TEST_LOOP_F32; j++)
    {
        for (i = 0; i < bsize; i++)
        {
            fsrcf32[i] = ((q15_t)(rand() >> 16)) / 32768.0f;
        }
#ifdef PF_COUNTER
        startPFM;
#endif
        riscv_dsp_spafir_f32(&sf32, fsrcf32, foutf32, scraf32, bsize);
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
