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
    float64_t ran_buf_in[2 * TRANSFORM_SAMPLES], ran_buf_golden[2 * TRANSFORM_SAMPLES];
    float64_t buf[2 * TRANSFORM_SAMPLES], buf2[2 * TRANSFORM_SAMPLES], cfft_golden[2 * TRANSFORM_SAMPLES];
    int i;
    float64_t NRMSD;

#ifdef WRITE_GOLDEN
    FILE *fptr;
    fptr = fopen("input_cfft_rand_f64.bin", "w");
    FILE *g2ptr;
    g2ptr = fopen("golden_cfft_rand_f64.bin", "w");
    FILE *g3ptr;
    g3ptr = fopen("golden_cifft_rand_f64.bin", "w");
#endif

    for (i = 0; i < TRANSFORM_SAMPLES; i++)
    {
        ran_buf_in[2 * i] = ran_buf_golden[2 * i] = randomfloat();
        ran_buf_in[2 * i + 1] = ran_buf_golden[2 * i + 1] = randomfloat();
        buf[2 * i] = buf2[2 * i] = test_fft_Input_f64[i];
        buf[2 * i + 1] = buf2[2 * i + 1] = test_fft_Input_f64[i];
        cfft_golden[2 * i] = cfft_f64_golden[2 * i];
        cfft_golden[2 * i + 1] = cfft_f64_golden[2 * i + 1];
    }

    dump_f64("randomly generated input array", ran_buf_in, 2 * TRANSFORM_SAMPLES);
#ifdef WRITE_GOLDEN
    fwrite(&ran_buf_in, sizeof(float64_t), 2 * TRANSFORM_SAMPLES, fptr);
    fclose(fptr);
#endif
    // Sine tone case
    riscv_dsp_cfft_f64(buf, TRANSFORM_SAMPLES_LOG);

    printf("cfft sintune results:\n");
    dump2_f64(buf, cfft_golden, 2 * TRANSFORM_SAMPLES);
    NRMSD = dump_rmsd64(buf, cfft_golden, 2 * TRANSFORM_SAMPLES);
    if (TRANSFORM_SAMPLES_LOG <= 10)
    {
        if (NRMSD > 0.0000001)
        {
            printf("NRMSD = %2.8f > 0.0000001, FAIL\n", NRMSD);
            return -1;
        }
    }

    // Random case
    riscv_dsp_cfft_f64(ran_buf_in, TRANSFORM_SAMPLES_LOG);
#ifdef WRITE_GOLDEN
    fwrite(&ran_buf_in, sizeof(float64_t), 2 * TRANSFORM_SAMPLES, g2ptr);
    fclose(g2ptr);
#endif

    // Sine tone case
    riscv_dsp_cifft_f64(buf, TRANSFORM_SAMPLES_LOG);
    printf("cifft sintune results:\n");
    dump2_f64(buf, buf2, 2 * TRANSFORM_SAMPLES);
    NRMSD = dump_rmsd64(buf, buf2, 2 * TRANSFORM_SAMPLES);
    if (TRANSFORM_SAMPLES_LOG <= 10)
    {
        if (NRMSD > 0.0000001)
        {
            printf("NRMSD = %2.8f > 0.0000001, FAIL\n", NRMSD);
            return -1;
        }
    }

    // Random case
    riscv_dsp_cifft_f64(ran_buf_in, TRANSFORM_SAMPLES_LOG);

#ifdef WRITE_GOLDEN
    fwrite(&ran_buf_in, sizeof(float64_t), 2 * TRANSFORM_SAMPLES, g3ptr);
    fclose(g3ptr);
#endif

    printf("cfft + cifft random results :\n");
    dump2_f64(ran_buf_in, ran_buf_golden, 2 * TRANSFORM_SAMPLES);
    NRMSD = dump_rmsd64(ran_buf_in, ran_buf_golden, 2 * TRANSFORM_SAMPLES);

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
