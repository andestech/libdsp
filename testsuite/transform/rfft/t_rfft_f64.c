#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "riscv_dsp_test_accuracy.h"
#include "riscv_dsp_test_data.h"
#include "riscv_dsp_transform_math.h"

static float64_t randomfloat(void)
{
    float64_t m, n, out;
    m = rand();
    n = rand();
    out = (m > n) ?  (float64_t)(n / m) : (float64_t)(m / n);
    return out;
}
int main(void)
{
    float64_t ran_buf_in[TRANSFORM_SAMPLES], ran_buf_golden[TRANSFORM_SAMPLES];
    float64_t buf[TRANSFORM_SAMPLES], buf2[TRANSFORM_SAMPLES], rfft_golden[TRANSFORM_SAMPLES];
    int i;
    float64_t NRMSD;

#ifdef WRITE_GOLDEN
    FILE *fptr;
    fptr = fopen("input_rfft_rand_f64.bin", "w");
    FILE *g2ptr;
    g2ptr = fopen("golden_rfft_rand_f64.bin", "w");
    FILE *g3ptr;
    g3ptr = fopen("golden_rifft_rand_f64.bin", "w");
#endif

    for (i = 0; i < TRANSFORM_SAMPLES; i++)
    {
        ran_buf_in[i] = ran_buf_golden[i] = randomfloat();
        buf[i] = buf2[i] = test_fft_Input[i];
        rfft_golden[i] = rfft_f64_golden[i];
    }

    dump_f64("randomly generated input array", ran_buf_in, TRANSFORM_SAMPLES);
#ifdef WRITE_GOLDEN
    fwrite(&ran_buf_in, sizeof(float64_t), TRANSFORM_SAMPLES, fptr);
    fclose(fptr);
#endif
    // Sine tone case
    riscv_dsp_rfft_f64(buf, TRANSFORM_SAMPLES_LOG);
    printf("rfft sintune results:\n");
    dump2_f64(buf, rfft_golden, TRANSFORM_SAMPLES);
    NRMSD = dump_rmsd64(buf, rfft_golden, TRANSFORM_SAMPLES);

    // Random case
    riscv_dsp_rfft_f64(ran_buf_in, TRANSFORM_SAMPLES_LOG);
#ifdef WRITE_GOLDEN
    fwrite(&ran_buf_in, sizeof(float64_t), TRANSFORM_SAMPLES, g2ptr);
    fclose(g2ptr);
#endif

    // Sine tone case
    riscv_dsp_rifft_f64(buf, TRANSFORM_SAMPLES_LOG);
    printf("rifft sintune results:\n");
    dump2_f64(buf, buf2, TRANSFORM_SAMPLES);
    NRMSD = dump_rmsd64(buf, buf2, TRANSFORM_SAMPLES);

    // Random case
    riscv_dsp_rifft_f64(ran_buf_in, TRANSFORM_SAMPLES_LOG);

#ifdef WRITE_GOLDEN
    fwrite(&ran_buf_in, sizeof(float64_t), TRANSFORM_SAMPLES, g3ptr);
    fclose(g3ptr);
#endif

    dump2_f64(ran_buf_in, ran_buf_golden, TRANSFORM_SAMPLES);
    NRMSD = dump_rmsd64(ran_buf_in, ran_buf_golden, TRANSFORM_SAMPLES);

    if (TRANSFORM_SAMPLES_LOG <= 10)
    {
        if (NRMSD < 0.0000001)
        {
            return 0;
        }
        else
        {
            printf("NRMSD = %2.8f > 0.0000001, FAIL\n", NRMSD);
            return -1;
        }
    }
    else
    {
        return 0;
    }
}
