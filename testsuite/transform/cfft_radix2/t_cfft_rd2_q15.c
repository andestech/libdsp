#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "riscv_dsp_test_accuracy.h"
#include "riscv_dsp_test_data.h"
#include "riscv_dsp_transform_math.h"

int main(void)
{
    q15_t buf[2 * TRANSFORM_SAMPLES] __attribute__ ((aligned(4)));
    q15_t buf2[2 * TRANSFORM_SAMPLES] __attribute__ ((aligned(4)));
    q15_t buf3[2 * TRANSFORM_SAMPLES] __attribute__ ((aligned(4)));
    int i;
    FILE *fptr;
    fptr = fopen("golden_cfft_rd2_f32.bin", "r");
    FILE *gptr;
    gptr = fopen("golden_cifft_rd2_f32.bin", "r");
    float32_t NRMSD;
    FILE *f2ptr;
    f2ptr = fopen("input_cfft_rd2_rand_f32.bin", "r");
    FILE *g2ptr;
    g2ptr = fopen("golden_cfft_rd2_rand_f32.bin", "r");
    FILE *g3ptr;
    g3ptr = fopen("golden_cifft_rd2_rand_f32.bin", "r");

    float32_t golden_buf[2 * TRANSFORM_SAMPLES];
    float32_t golden_ibuf[2 * TRANSFORM_SAMPLES];
    float32_t ran_buf_in[2 * TRANSFORM_SAMPLES];
    float32_t ran_buf_cfft_golden[2 * TRANSFORM_SAMPLES];
    float32_t ran_buf_cifft_golden[2 * TRANSFORM_SAMPLES];
    q15_t ran_q15_in[2 * TRANSFORM_SAMPLES];
    test_Fread(&golden_buf, sizeof(float32_t), 2 * TRANSFORM_SAMPLES, fptr);
    test_Fread(&golden_ibuf, sizeof(float32_t), 2 * TRANSFORM_SAMPLES, gptr);
    test_Fread(&ran_buf_in, sizeof(float32_t), 2 * TRANSFORM_SAMPLES, f2ptr);
    test_Fread(&ran_buf_cfft_golden, sizeof(float32_t), 2 * TRANSFORM_SAMPLES, g2ptr);
    test_Fread(&ran_buf_cifft_golden, sizeof(float32_t), 2 * TRANSFORM_SAMPLES, g3ptr);

    for (i = 0; i < TRANSFORM_SAMPLES; i++)
    {
        buf[2 * i] = buf2[2 * i] = (convert_float_to_q15(test_fft_Input[i])) >> 1;
        buf[2 * i + 1] = buf2[2 * i + 1] = (convert_float_to_q15(test_fft_Input[i])) >> 1;
    }
    dump_q15("randomly generated input array", buf, 2 * TRANSFORM_SAMPLES);
    for (i = 0; i < 2*TRANSFORM_SAMPLES; i++)
    {
        ran_q15_in[i] = (convert_float_to_q15(ran_buf_in[i]));
    }

    // Sine Tone case
    riscv_dsp_cfft_rd2_q15(buf, TRANSFORM_SAMPLES_LOG);
    dump_q15("after cFFT (Forward)", buf, 2 * TRANSFORM_SAMPLES);

    /* check dct with golden */
    printf("--------------\n");
    NRMSD = dump2_goldendiff_q15("after CFFT_RD2 (Forward)", golden_buf, buf, 2 * TRANSFORM_SAMPLES, 1);
    printf("CFFT_RD2 out scaleup by %d\n", (2 << TRANSFORM_SAMPLES_LOG));
    LOG("--------------\n");

    if (NRMSD > 0.00007)
    {
        printf("NRMSD = %2.8f > 0.00007, CFFT_RD2 FAIL\n", NRMSD);
        return -1;
    }
    // Random case
    riscv_dsp_cfft_rd2_q15(ran_q15_in, TRANSFORM_SAMPLES_LOG);
    NRMSD = dump2_goldendiff2_q15("after CFFT_RD4 random case (Forward)", ran_buf_cfft_golden, ran_q15_in, 2 * TRANSFORM_SAMPLES, 1);

    for (i = 0; i < 2 * TRANSFORM_SAMPLES; i++)
    {
        buf3[i] = buf[i];
    }

    /* CIFFT function */
    for (i = 0; i < 2 * TRANSFORM_SAMPLES; i++)
    {
        buf[i] = (convert_float_to_q15(golden_buf[i] / (4 << TRANSFORM_SAMPLES_LOG)));
    }
    dump_q15("randomly generated input array", buf, TRANSFORM_SAMPLES);

    // Sine Tone case
    riscv_dsp_cifft_rd2_q15(buf, TRANSFORM_SAMPLES_LOG);
    dump_q15("after CIFFT (Backward)", buf, 2 * TRANSFORM_SAMPLES);

    /* check idct with golden */
    NRMSD = dump2_inv_goldendiff_q15("after CIFFT_RD2 (Backward)", golden_ibuf, buf, 2 * TRANSFORM_SAMPLES);
    printf("CIFFT_RD2 out scaleup by % d\n", 2);
    LOG("--------------\n");

    if (TRANSFORM_SAMPLES_LOG <= 10)
    {
        if (NRMSD > 0.0003)
        {
            printf("NRMSD = %2.8f > 0.0003, CIFFT_RD2 FAIL\n", NRMSD);
            return -1;
        }
    }
    // Random case
    riscv_dsp_cifft_rd2_q15(ran_q15_in, TRANSFORM_SAMPLES_LOG);
    NRMSD = dump2_inv_goldendiff2_q15("after CIFFT_RD4 random case (Backward)", ran_buf_cifft_golden, ran_q15_in, 2 * TRANSFORM_SAMPLES);

    riscv_dsp_cifft_rd2_q15(buf3, TRANSFORM_SAMPLES_LOG);
    dump_q15("after CIFFT (Forward + Backward)", buf3, 2 * TRANSFORM_SAMPLES);

    /* check idct with golden */
    NRMSD = dump2_inv_goldendiff_q15("after CIFFT_RD2 (Forward + Backward)", golden_ibuf, buf3, 2 * TRANSFORM_SAMPLES);
    printf("CIFFT_RD2 out scaleup by % d\n", 2);
    LOG("--------------\n");

    fclose(fptr);
    fclose(gptr);
    fclose(f2ptr);
    fclose(g2ptr);
    fclose(g3ptr);

    if (TRANSFORM_SAMPLES_LOG <= 10)
    {
        if (NRMSD <  0.003)
        {
            return 0;
        }
        else
        {
            printf("NRMSD = %2.8f > 0.003, CIFFT_RD2 FAIL\n", NRMSD);
            return -1;
        }
    }
    else
    {
        return 0;
    }
}
