#include <stdio.h>
#include <stdlib.h>
#include "riscv_dsp_transform_math.h"
#include "riscv_dsp_test_accuracy.h"
#include "riscv_dsp_test_data.h"

static float32_t randomfloat(void)
{
    float32_t m, n, out;
    m = rand();
    n = rand();
    out = (m > n) ?  (float32_t)(n / m) : (float32_t)(m / n);
    return out;
}

int main(void)
{
    FILE *fptr;
    fptr = fopen("golden_cfft_f32.bin", "w");
    FILE *gptr;
    gptr = fopen("golden_cifft_f32.bin", "w");
    FILE *f2ptr;
    f2ptr = fopen("input_cfft_rand_f32.bin", "w");
    FILE *g2ptr;
    g2ptr = fopen("golden_cfft_rand_f32.bin", "w");
    FILE *g3ptr;
    g3ptr = fopen("golden_cifft_rand_f32.bin", "w");

    uint32_t i;
    float32_t buf[2 * TRANSFORM_SAMPLES], buf2[2 * TRANSFORM_SAMPLES], cfft_golden[2 * TRANSFORM_SAMPLES];
    float32_t ran_buf_in[2 * TRANSFORM_SAMPLES], ran_buf_golden[2 * TRANSFORM_SAMPLES];

    for (i = 0; i < TRANSFORM_SAMPLES; i++)
    {
        ran_buf_in[2 * i] = ran_buf_golden[2 * i] = randomfloat();
        ran_buf_in[2 * i + 1] = ran_buf_golden[2 * i + 1] = randomfloat();
        buf[2 * i] = buf2[2 * i] = test_fft_Input[i];
        buf[2 * i + 1] = buf2[2 * i + 1] = test_fft_Input[i];
    }
    fwrite(&ran_buf_in, sizeof(float32_t), 2*TRANSFORM_SAMPLES, f2ptr);
    fclose(f2ptr);

    for (i = 0; i < 2 * TRANSFORM_SAMPLES; i++)
    {
        cfft_golden[i] = cfft_f32_golden[i];
        //cfft_golden[i] = (float32_t)cfft_f64_golden[i];
    }

    dump_f32("randomly generated input array", buf, 2 * TRANSFORM_SAMPLES);

    // Sine Tone case
    riscv_dsp_cfft_f32(buf, TRANSFORM_SAMPLES_LOG);
    dump_f32("after cFFT (Forward)", buf, 2 * TRANSFORM_SAMPLES);
    // Random case
    riscv_dsp_cfft_f32(ran_buf_in, TRANSFORM_SAMPLES_LOG);

    float32_t NRMSD;

    printf("after CFFT_RD2 (Forward), ");
    dump2_f32(cfft_golden, buf, 2 * TRANSFORM_SAMPLES);
    NRMSD = dump_rmsd(cfft_golden, buf, 2 * TRANSFORM_SAMPLES);

    if (TRANSFORM_SAMPLES_LOG <= 10)
    {
        if (NRMSD > 0.0000001)
        {
            printf("NRMSD = %2.8f > 0.0000001, FAIL\n", NRMSD);
            return -1;
        }
    }

    for (i = 0; i < 2 * TRANSFORM_SAMPLES; i++)
    {
        fwrite(&cfft_golden[i], sizeof(float32_t), 1, fptr);
        fwrite(&ran_buf_in[i], sizeof(float32_t), 1, g2ptr);
    }
    fclose(fptr);
    fclose(g2ptr);

    // Sine Tone case
    riscv_dsp_cifft_f32(cfft_golden, TRANSFORM_SAMPLES_LOG);
    dump_f32("after cIFFT (Backward)", cfft_golden, 2 * TRANSFORM_SAMPLES);
    // Random case
    riscv_dsp_cifft_f32(ran_buf_in, TRANSFORM_SAMPLES_LOG);

    for (i = 0; i < 2 * TRANSFORM_SAMPLES; i++)
    {
        fwrite(&buf2[i], sizeof(float32_t), 1, gptr);
        fwrite(&ran_buf_in[i], sizeof(float32_t), 1, g3ptr);
    }
    fclose(gptr);
    fclose(g3ptr);

    printf("after CIFFT_RD2 (Backward), ");
    dump2_f32(buf2, cfft_golden, 2 * TRANSFORM_SAMPLES);
    NRMSD = dump_rmsd(buf2, cfft_golden, 2 * TRANSFORM_SAMPLES);

    if (TRANSFORM_SAMPLES_LOG <= 10)
    {
        if (NRMSD > 0.0000001)
        {
            printf("NRMSD = %2.8f > 0.0000001, FAIL\n", NRMSD);
            return -1;
        }
    }

    riscv_dsp_cifft_f32(buf, TRANSFORM_SAMPLES_LOG);
    dump_f32("after cIFFT (Forward + Backward)", buf, 2 * TRANSFORM_SAMPLES);

    printf("after CIFFT_RD2 (Forward + Backward), ");
    dump2_f32(buf, buf2, 2 * TRANSFORM_SAMPLES);
    NRMSD = dump_rmsd(buf, buf2, 2 * TRANSFORM_SAMPLES);

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