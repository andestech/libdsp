#include "riscv_dsp_filtering_math.h"

q15_t fsrcq15[SIZE] FUNC_ATTR_ALIGN;
q15_t foutq15[SIZE] FUNC_ATTR_ALIGN;
q15_t bstq15[STATE_LEN] FUNC_ATTR_ALIGN = {0};
q15_t coq15[COE_LEN] FUNC_ATTR_ALIGN;
int main(void)
{
    uint32_t i;
    const uint32_t bsize = SIZE;
    const uint8_t bstage = BIQUAD_STAGE;

    int8_t firshift = POST_SHIFT;
    q15_t *fptrq15;

#ifdef NDS_BQ_DF1_Q15_USE_SIMD
    //For DSP ext. configuration
    coq15[0] = 0x200;
    coq15[1] = 0;           //SHOULD be zero
    coq15[2] = 0;
    coq15[3] = 0x200;
    coq15[4] = 0x200;
    coq15[5] = -0x800;
    coq15[6] = -0x800;
    coq15[7] = 0;           //SHOULD be zero
    coq15[8] = 0x200;
    coq15[9] = 0;
    coq15[10] = 0x200;
    coq15[11] = 0x200;
    coq15[12] = -0x800;
    coq15[13] = 0;          //SHOULD be zero
    coq15[14] = -0x800;
    coq15[15] = 0x200;
    coq15[16] = 0;
    coq15[17] = 0x200;
#else
    //For normal configuration
    coq15[0] = 0x200;
    coq15[6] = 0x200;
    coq15[12] = 0x200;
    coq15[1] = 0;
    coq15[7] = 0;
    coq15[13] = 0;
    coq15[2] = 0x200;
    coq15[8] = 0x200;
    coq15[14] = 0x200;
    coq15[3] = 0x200;
    coq15[9] = 0x200;
    coq15[4] = -0x800;
    coq15[10] = -0x800;
    coq15[5] = -0x800;
    coq15[11] = -0x800;
#endif

    riscv_dsp_bq_df1_q15_t bsiq15 = {bstage, bstq15, coq15, firshift};


    for (fptrq15 = &fsrcq15[0], i = 0; i < bsize; i++)
    {
        *fptrq15++ = (q15_t)(rand() >> 16);
    }

#ifdef PF_COUNTER
    startPFM;
#endif
    riscv_dsp_bq_df1_q15(&bsiq15, fsrcq15, foutq15, bsize);
#ifdef PF_COUNTER
    stopPFM;
    readResult();
#endif

    return 0;
}
