#include "riscv_dsp_transform_math.h"
 #define RFFT_LOGN 7
 #define RFFT_N (1 << RFFT_LOGN)

q31_t dctq31[RFFT_N] FUNC_ATTR_ALIGN;
q31_t *dctptrq31 = &dctq31[0];
int main(void)
{
    uint32_t i;
    total_samples = RFFT_N;
    /* Test code */

    for (i = 0; i < RFFT_N; i++)
    {
        *dctptrq31++ = (q31_t)rand();
    }
#ifdef PF_COUNTER
    startPFM;
#endif
    riscv_dsp_rfft_q31(dctq31, RFFT_LOGN);
#ifdef PF_COUNTER
    stopPFM;
    readResult();
#endif
    /* Test code end*/

    return 0;
}
