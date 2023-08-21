#include "riscv_dsp_filtering_math.h"

q31_t fsrcq31[SIZE] FUNC_ATTR_ALIGN;
q31_t foutq31[SIZE] FUNC_ATTR_ALIGN;
q31_t reinq31[SIZE] FUNC_ATTR_ALIGN;
q31_t errq31[SIZE] FUNC_ATTR_ALIGN;
q31_t ene = 0, x0 = 0;
q31_t dsq31[FIR_STATE_LEN] FUNC_ATTR_ALIGN = {0};
q31_t dcoq31[FIR_NUM_TAP] FUNC_ATTR_ALIGN;

unsigned int postshift = POST_SHIFT;
int main(void)
{
    uint32_t i, j;
    q31_t uq31;

    uq31 = (q31_t) (MU * (1 << 30));

    for (i = 0; i < FIR_NUM_TAP; i++)
    {
        dcoq31[i] = (q31_t)rand();
    }

    for (i = 0; i < SIZE; i++)
    {
        fsrcq31[i] = (q31_t)rand();
        reinq31[i] = (q31_t)rand();
    }

    riscv_dsp_nlms_q31_t nlmq31 = {FIR_NUM_TAP, dsq31, dcoq31, uq31, postshift, ene, x0};
#ifdef PF_COUNTER
    startPFM;
#endif
    riscv_dsp_nlms_q31(&nlmq31, fsrcq31, reinq31, foutq31, errq31, SIZE);
#ifdef PF_COUNTER
    stopPFM;
    readResult();
#endif

    return 0;
}
