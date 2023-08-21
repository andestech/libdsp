#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <inttypes.h>       //for PRIx32
#include "riscv_dsp_transform_math.h"
#include "riscv_dsp_test_accuracy.h"
#include "riscv_dsp_test_data.h"

int main(void)
{
    q31_t buf[2 * TRANSFORM_SAMPLES], buf2[2 * TRANSFORM_SAMPLES], buf3[2 * TRANSFORM_SAMPLES];
    int i;
    FILE *fptr;
    fptr = fopen("golden_cfft_rd4_f32.bin", "r");
    FILE *gptr;
    gptr = fopen("golden_cifft_rd4_f32.bin", "r");
    float32_t NRMSD;
    FILE *f2ptr;
    f2ptr = fopen("input_cfft_rd4_rand_f32.bin", "r");
    FILE *g2ptr;
    g2ptr = fopen("golden_cfft_rd4_rand_f32.bin", "r");
    FILE *g3ptr;
    g3ptr = fopen("golden_cifft_rd4_rand_f32.bin", "r");

    float32_t golden_buf[2 * TRANSFORM_SAMPLES];
    float32_t golden_ibuf[2 * TRANSFORM_SAMPLES];
    float32_t ran_buf_in[2 * TRANSFORM_SAMPLES];
    float32_t ran_buf_cfft_golden[2 * TRANSFORM_SAMPLES];
    float32_t ran_buf_cifft_golden[2 * TRANSFORM_SAMPLES];
    q31_t ran_q31_in[2 * TRANSFORM_SAMPLES];
    test_Fread(&golden_buf, sizeof(float32_t), 2 * TRANSFORM_SAMPLES, fptr);
    test_Fread(&golden_ibuf, sizeof(float32_t), 2 * TRANSFORM_SAMPLES, gptr);
    test_Fread(&ran_buf_in, sizeof(float32_t), 2 * TRANSFORM_SAMPLES, f2ptr);
    test_Fread(&ran_buf_cfft_golden, sizeof(float32_t), 2 * TRANSFORM_SAMPLES, g2ptr);
    test_Fread(&ran_buf_cifft_golden, sizeof(float32_t), 2 * TRANSFORM_SAMPLES, g3ptr);

    for (i = 0; i < TRANSFORM_SAMPLES; i++)
    {
        buf[2 * i] = buf2[2 * i] = (convert_float_to_q31(test_fft_Input[i]));
        buf[2 * i + 1] = buf2[2 * i + 1] = (convert_float_to_q31(test_fft_Input[i]));
    }
    for (i = 0; i < 2*TRANSFORM_SAMPLES; i++)
    {
        ran_q31_in[i] = (convert_float_to_q31(ran_buf_in[i]));
    }
    dump_q31("randomly generated input array", ran_q31_in, 2 * TRANSFORM_SAMPLES);

    // Sine Tone case
    riscv_dsp_cfft_rd4_q31(buf, TRANSFORM_SAMPLES_LOG);
    dump_q31("after cFFT (Forward)", buf, 2 * TRANSFORM_SAMPLES);

    /* check dct with golden */
    printf("--------------\n");
    NRMSD = dump2_goldendiff2_q31("after CFFT_RD4 (Forward)", golden_buf, buf, 2 * TRANSFORM_SAMPLES, 1);
    printf("CIFFT_RD2 out scaleup by %d\n", (1 << TRANSFORM_SAMPLES_LOG));
    LOG("--------------\n");

    if (NRMSD > 0.0000001)
    {
        printf("NRMSD = %2.8f > 0.0000001, CFFT_RD4 FAIL\n", NRMSD);
        return -1;
    }
    // Random case
    riscv_dsp_cfft_rd4_q31(ran_q31_in, TRANSFORM_SAMPLES_LOG);
    NRMSD = dump2_goldendiff2_q31("after CFFT_RD4 random case (Forward)", ran_buf_cfft_golden, ran_q31_in, 2 * TRANSFORM_SAMPLES, 1);

    for (i = 0; i < 2 * TRANSFORM_SAMPLES; i++)
    {
        buf3[i] =  buf[i];
    }

    /* CIFFT function */
    for (i = 0; i < 2 * TRANSFORM_SAMPLES; i++)
    {
        buf[i] = (convert_float_to_q31(golden_buf[i] / (4 << TRANSFORM_SAMPLES_LOG)));
    }
    dump_q31("randomly generated input array", buf, TRANSFORM_SAMPLES);

    // Sine Tone case
    riscv_dsp_cifft_rd4_q31(buf, TRANSFORM_SAMPLES_LOG);
    dump_q31("after cIFFT (Backward)", buf, 2 * TRANSFORM_SAMPLES);

    /* check idct with golden */
    NRMSD = dump2_inv_goldendiff_q31("after CFFT_RD4 (Backward)", golden_ibuf, buf, 2 * TRANSFORM_SAMPLES);
    printf("CIFFT_RD4 out scaleup by %d\n", 2);
    LOG("--------------\n");

    if (TRANSFORM_SAMPLES_LOG <= 10)
    {
        if (NRMSD >  0.0000001)
        {
            printf("NRMSD = %2.8f > 0.0000001, CIFFT_RD4 FAIL\n", NRMSD);
            return -1;
        }
    }
    // Random case
    riscv_dsp_cifft_rd4_q31(ran_q31_in, TRANSFORM_SAMPLES_LOG);
    NRMSD = dump2_inv_goldendiff2_q31("after CIFFT_RD4 random case (Backward)", ran_buf_cifft_golden, ran_q31_in, 2 * TRANSFORM_SAMPLES);

    for (i = 0; i < 2 * TRANSFORM_SAMPLES; i++)
    {
        buf3[i] >>= 1;
    }

    riscv_dsp_cifft_rd4_q31(buf3, TRANSFORM_SAMPLES_LOG);
    dump_q31("after cIFFT (Forward + Backward)", buf3, 2 * TRANSFORM_SAMPLES);

    /* check idct with golden */
    NRMSD = dump2_inv_goldendiff_q31("after CFFT_RD4 (Forward + Backward)", golden_ibuf, buf3, 2 * TRANSFORM_SAMPLES);
    printf("CIFFT_RD4 out scaleup by %d\n", 2);
    LOG("--------------\n");

    fclose(fptr);
    fclose(gptr);
    fclose(f2ptr);
    fclose(g2ptr);
    fclose(g3ptr);

    if (TRANSFORM_SAMPLES_LOG <= 10)
    {
        if (NRMSD <  0.0000001)
        {
            return 0;
        }
        else
        {
            printf("NRMSD = %2.8f > 0.0000001, CIFFT_RD4 FAIL\n", NRMSD);
            return -1;
        }
    }
    else
    {
        return 0;
    }
}
