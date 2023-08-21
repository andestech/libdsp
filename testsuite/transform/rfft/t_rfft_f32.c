#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "riscv_dsp_test_accuracy.h"
#include "riscv_dsp_test_data.h"
#include "riscv_dsp_transform_math.h"

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
    fptr = fopen("golden_rfft_f32.bin", "w");
    FILE *gptr;
    gptr = fopen("golden_rifft_f32.bin", "w");
    FILE *f2ptr;
    f2ptr = fopen("input_rfft_rand_f32.bin", "w");
    FILE *g2ptr;
    g2ptr = fopen("golden_rfft_rand_f32.bin", "w");
    FILE *g3ptr;
    g3ptr = fopen("golden_rifft_rand_f32.bin", "w");

    float buf[TRANSFORM_SAMPLES];
    float buf2[TRANSFORM_SAMPLES];
    float rfft_golden[TRANSFORM_SAMPLES];
    float32_t ran_buf_in[TRANSFORM_SAMPLES], ran_buf_golden[TRANSFORM_SAMPLES];
    int i;
    float32_t NRMSD;

    for (i = 0; i < TRANSFORM_SAMPLES; i++)
    {
        ran_buf_in[i] = ran_buf_golden[i] = randomfloat();
        buf[i] = buf2[i] = test_fft_Input[i];
        rfft_golden[i] = rfft_f32_golden[i];
    }
    fwrite(&ran_buf_in, sizeof(float32_t), TRANSFORM_SAMPLES, f2ptr);
    fclose(f2ptr);

    dump_f32("randomly generated input array", buf, TRANSFORM_SAMPLES);
    // Sine Tone case
    riscv_dsp_rfft_f32(buf, TRANSFORM_SAMPLES_LOG);
    dump_f32("after rFFT", buf, TRANSFORM_SAMPLES);
    // Random case
    riscv_dsp_rfft_f32(ran_buf_in, TRANSFORM_SAMPLES_LOG);

    printf("after rFFT (Forward), ");
    dump2_f32(rfft_golden, buf, TRANSFORM_SAMPLES);
    NRMSD = dump_rmsd(rfft_golden, buf, TRANSFORM_SAMPLES);

    if (TRANSFORM_SAMPLES_LOG <= 10)
    {
        if (NRMSD > 0.0000001)
        {
            printf("NRMSD = %2.8f > 0.0000001, FAIL\n", NRMSD);
            return -1;
        }
    }

    for (i = 0; i < TRANSFORM_SAMPLES; i++)
    {
        fwrite(&rfft_golden[i], sizeof(float32_t), 1, fptr);
        fwrite(&ran_buf_in[i], sizeof(float32_t), 1, g2ptr);
    }
    fclose(fptr);
    fclose(g2ptr);

    // Sine Tone case
    riscv_dsp_rifft_f32(rfft_golden, TRANSFORM_SAMPLES_LOG);
    riscv_dsp_rifft_f32(ran_buf_in, TRANSFORM_SAMPLES_LOG);

    printf("after riFFT (Backward), ");
    dump2_f32(rfft_golden, buf2, TRANSFORM_SAMPLES);
    NRMSD = dump_rmsd(rfft_golden, buf2, TRANSFORM_SAMPLES);

    if (TRANSFORM_SAMPLES_LOG <= 10)
    {
        if (NRMSD > 0.0000001)
        {
            printf("NRMSD = %2.8f > 0.0000001, FAIL\n", NRMSD);
            return -1;
        }
    }

    riscv_dsp_rifft_f32(buf, TRANSFORM_SAMPLES_LOG);
    dump_f32("after rIFFT", buf, TRANSFORM_SAMPLES);

    for (i = 0; i < TRANSFORM_SAMPLES; i++)
    {
        fwrite(&buf2[i], sizeof(float32_t), 1, gptr);
        fwrite(&ran_buf_in[i], sizeof(float32_t), 1, g3ptr);
    }
    fclose(gptr);
    fclose(g3ptr);

    printf("after RFFT (Forward + Backward) sine tone, ");
    dump2_f32(buf2, buf, TRANSFORM_SAMPLES);
    NRMSD = dump_rmsd(buf2, buf, TRANSFORM_SAMPLES);
    if (TRANSFORM_SAMPLES_LOG <= 10)
    {
        if (NRMSD > 0.0000001)
        {
            printf("NRMSD = %2.8f > 0.0000001, FAIL\n", NRMSD);
            return -1;
        }
    }

    printf("after RFFT (Forward + Backward) random case, ");
    dump2_f32(ran_buf_in, ran_buf_golden, TRANSFORM_SAMPLES);
    NRMSD = dump_rmsd(ran_buf_in, ran_buf_golden, TRANSFORM_SAMPLES);

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
