#include "riscv_dsp_utils_math.h"

#define TEST_SIZE 16384
#define CONST_PI (3.14159265358979323846)
#define INTER_PI (4 * CONST_PI / TEST_SIZE)

float16_t srcf16[TEST_SIZE]  FUNC_ATTR_ALIGN;
int main(void)
{
    float16_t ret;
    total_samples = TEST_SIZE;
    acc_result result = {0, 0};
    uint32_t i, j;

    for (i = 0; i < TEST_SIZE; i++)
    {
        int tmp = i - (TEST_SIZE / 2);
        float16_t srcin = (float16_t)tmp * INTER_PI;
        srcf16[i] = srcin;
    }
#ifdef PF_COUNTER
    startPFM;
#endif
    for (i = 0; i < TEST_SIZE; i++)
    {
        ret = riscv_dsp_cos_f16(srcf16[i]);
    }
#ifdef PF_COUNTER
    stopPFM;
    readResult();
#endif
    return ret;
}
