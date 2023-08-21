#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "riscv_dsp_utils_math.h"
#include <inttypes.h>       //for PRIx32
#include "riscv_dsp_test_accuracy.h"

#ifdef FOUR_PI_RANGE
#define TEST_NDS32_SIN_Q_4PI_USE_ARRAY
#else
#define TEST_NDS32_SIN_Q_USE_ARRAY
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

    q15_t q1[2 * blocksize];
    q15_t q2[2 * blocksize];
    float32_t NRMSD;
    float32_t golden_f32[2 * blocksize], output_f32[2 * blocksize];

    for (n = 0; n < (2 * blocksize); n++)
    {
        float input = (float)(n - blocksize) * tmp;
#ifdef TEST_NDS32_SIN_Q_USE_ARRAY
#ifdef FOUR_PI_RANGE
        float golden_val = golden_sin_q4pi[n];
#else
        float golden_val = golden_sin_q[n];
#endif
#else
        float golden_val = sinf(input);
#endif
        q15_t q = map_radian_float_to_q15(input);

        q1[n] = convert_float_to_q15(golden_val);
        q2[n] = riscv_dsp_sin_q15(q);
    }

    dump2_q15(q1, q2, 2 * blocksize);
    riscv_dsp_convert_q15_f32(q1, golden_f32, 2 * blocksize);
    riscv_dsp_convert_q15_f32(q2, output_f32, 2 * blocksize);
    NRMSD = dump_rmsd(golden_f32, output_f32, 2 * blocksize);

    if (NRMSD <= 0.00003)
        return 0;
    else
        return -1;
}
