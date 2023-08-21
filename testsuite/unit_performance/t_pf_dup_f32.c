#include "riscv_dsp_utils_math.h"

float32_t srcf32[SIZE] FUNC_ATTR_ALIGN;
float32_t outf32[SIZE] FUNC_ATTR_ALIGN;
int main(void)
{
    float32_t *ptrf32;
    uint32_t blocksize = SIZE, i;

    for (ptrf32 = &srcf32[0], i = 0; i < blocksize; i++)
    {
        *ptrf32++ = ((q15_t)(rand() >> 16)) / 32768.0f;
    }

#ifdef PF_COUNTER
    startPFM;
#endif
    riscv_dsp_dup_f32(srcf32, outf32, blocksize);
#ifdef PF_COUNTER
    stopPFM;
    readResult();
#endif

    return 0;
}
