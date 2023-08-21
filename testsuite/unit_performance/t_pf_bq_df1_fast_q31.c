#include "riscv_dsp_filtering_math.h"

q31_t fsrcq31[SIZE] FUNC_ATTR_ALIGN, foutq31[SIZE] FUNC_ATTR_ALIGN;
q31_t bstq31[STATE_LEN] FUNC_ATTR_ALIGN = {0};
q31_t coq31[COE_LEN] FUNC_ATTR_ALIGN;
int main(void)
{
    uint32_t i;
    const uint32_t bsize = SIZE;
    const uint8_t bstage = BIQUAD_STAGE;

    int8_t firshift = POST_SHIFT;
    q31_t *fptrq31;
    coq31[0] = 0x2000000;
    coq31[6] = 0x2000000;
    coq31[12] = 0x2000000;
    coq31[1] = 0x2000000;
    coq31[7] = 0x2000000;
    coq31[13] = -0x8000000;
    coq31[2] = 0x2000000;
    coq31[8] = -0x2000000;
    coq31[14] = -0x8000000;
    coq31[3] = -0x2000000;
    coq31[9] = -0x2000000;
    coq31[4] = -0x8000000;
    coq31[10] = -0x8000000;
    coq31[5] = -0x8000000;
    coq31[11] = 0x8000000;
    riscv_dsp_bq_df1_q31_t bsiq31 = {bstage, bstq31, coq31, firshift};

    for (fptrq31 = &fsrcq31[0], i = 0; i < bsize; i++)
    {
        *fptrq31++ = (q31_t)rand();
    }

#ifdef PF_COUNTER
    startPFM;
#endif
    riscv_dsp_bq_df1_fast_q31(&bsiq31, fsrcq31, foutq31, bsize);
#ifdef PF_COUNTER
    stopPFM;
    readResult();
#endif

    return 0;
}
