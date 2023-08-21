#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "riscv_dsp_utils_math.h"
#include <inttypes.h>       //for PRIx32
#include "riscv_dsp_test_accuracy.h"

#ifdef FOUR_PI_RANGE
#define TEST_NDS32_COS_Q_4PI_USE_ARRAY
#else
#define TEST_NDS32_COS_Q_USE_ARRAY
#endif
#include "riscv_dsp_test_data.h"
#define SIN_RES_N 1024

int main(void)
{
    int n;
    float tmp = PI / (SIN_RES_N << 2);
#ifdef FOUR_PI_RANGE
    int blocksize = SIN_RES_N << 3;
#else
    int blocksize = SIN_RES_N << 2;
#endif

    q31_t q1[2 * blocksize];
    q31_t q2[2 * blocksize];
    float32_t NRMSD;
    float32_t golden_f32[2 * blocksize], output_f32[2 * blocksize];

    for (n = 0; n < (2 * blocksize); n++)
    {
        float input = (float)(n - blocksize) * tmp;
#ifdef TEST_NDS32_COS_Q_USE_ARRAY
#ifdef FOUR_PI_RANGE
        float golden_val = golden_cos_q4pi[n];
#else
        float golden_val = golden_cos_q[n];
#endif
#else
        float golden_val = cosf(input);
#endif
        q31_t q = map_radian_float_to_q31(input);
        q1[n] = convert_float_to_q31(golden_val);
        q2[n] = riscv_dsp_cos_q31(q);
    }

    dump2_q31(q1, q2, 2 * blocksize);
    riscv_dsp_convert_q31_f32(q1, golden_f32, 2 * blocksize);
    riscv_dsp_convert_q31_f32(q2, output_f32, 2 * blocksize);
    NRMSD = dump_rmsd(golden_f32, output_f32, 2 * blocksize);

    if (NRMSD <= 0.000011)
        return 0;
    else
        return -1;
}
