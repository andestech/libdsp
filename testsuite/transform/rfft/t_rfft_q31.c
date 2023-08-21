#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <inttypes.h>       //for PRIx32
#include "riscv_dsp_transform_math.h"
#include "riscv_dsp_test_accuracy.h"
#include "riscv_dsp_test_data.h"

int main(void)
{
    q31_t buf[TRANSFORM_SAMPLES], buf3[TRANSFORM_SAMPLES];
    q31_t buf2[TRANSFORM_SAMPLES];
    int i;
    FILE *fptr;
    fptr = fopen("golden_rfft_f32.bin", "r");
    FILE *gptr;
    gptr = fopen("golden_rifft_f32.bin", "r");
    float32_t NRMSD;
    FILE *f2ptr;
    f2ptr = fopen("input_rfft_rand_f32.bin", "r");
    FILE *g2ptr;
    g2ptr = fopen("golden_rfft_rand_f32.bin", "r");
    FILE *g3ptr;
    g3ptr = fopen("golden_rifft_rand_f32.bin", "r");

    float32_t golden_dct[TRANSFORM_SAMPLES];
    float32_t golden_idct[TRANSFORM_SAMPLES];
    test_Fread(&golden_dct, sizeof(float32_t), TRANSFORM_SAMPLES, fptr);
    test_Fread(&golden_idct, sizeof(float32_t), TRANSFORM_SAMPLES, gptr);
    float32_t ran_buf_in[TRANSFORM_SAMPLES];
    float32_t ran_buf_rfft_golden[TRANSFORM_SAMPLES];
    float32_t ran_buf_rifft_golden[TRANSFORM_SAMPLES];
    q31_t ran_q31_in[TRANSFORM_SAMPLES];
    test_Fread(&ran_buf_in, sizeof(float32_t), TRANSFORM_SAMPLES, f2ptr);
    test_Fread(&ran_buf_rfft_golden, sizeof(float32_t), TRANSFORM_SAMPLES, g2ptr);
    test_Fread(&ran_buf_rifft_golden, sizeof(float32_t), TRANSFORM_SAMPLES, g3ptr);

    for (i = 0; i < TRANSFORM_SAMPLES; i++)
    {
        buf[i] = buf2[i] = (convert_float_to_q31(test_fft_Input[i]));
        ran_q31_in[i] = (convert_float_to_q31(ran_buf_in[i]));
    }
    dump_q31("randomly generated input array", buf, TRANSFORM_SAMPLES);

    // Sine Tone case
    riscv_dsp_rfft_q31(buf, TRANSFORM_SAMPLES_LOG);
    dump_q31("after rFFT", buf, TRANSFORM_SAMPLES);

    /* check dct with golden */
    printf("--------------\n");
    printf("after rFFT (Forward), ");
    NRMSD = dump2_goldendiff2_q31("after RFFT", golden_dct, buf, TRANSFORM_SAMPLES, 0);
    printf("RFFT out scaleup by %d\n", (4 << TRANSFORM_SAMPLES_LOG));
    LOG("--------------\n");

    if (NRMSD > 0.0000001)
    {
        printf("NRMSD = %2.8f > 0.0000001, RFFT FAIL\n", NRMSD);
        return -1;
    }
    // Random case
    riscv_dsp_rfft_q31(ran_q31_in, TRANSFORM_SAMPLES_LOG);
    NRMSD = dump2_goldendiff2_q31("after RFFT", ran_buf_rfft_golden, ran_q31_in, TRANSFORM_SAMPLES, 0);
    if (NRMSD > 0.0000001)
    {
        printf("check randon1\n");
        printf("NRMSD = %2.8f > 0.0000001, RFFT FAIL\n", NRMSD);
        return -1;
    }

    for (i = 0; i < TRANSFORM_SAMPLES; i++)
    {
        buf3[i] = buf[i];
    }

    /* RIFFT function */
    for (i = 0; i < TRANSFORM_SAMPLES; i++)
    {
        buf[i] = (convert_float_to_q31(golden_dct[i] / (4 << TRANSFORM_SAMPLES_LOG)));
    }
    dump_q31("randomly generated input array", buf, TRANSFORM_SAMPLES);

    // Sine Tone case
    riscv_dsp_rifft_q31(buf, TRANSFORM_SAMPLES_LOG);
    dump2_q31(buf2, buf, TRANSFORM_SAMPLES);

    /* check idct with golden */
    printf("after rFFT  (Backward), ");
    NRMSD = dump2_inv_goldendiff_q31("after RIFFT", golden_idct, buf, TRANSFORM_SAMPLES);
    printf("RIFFT out scaleup by %d\n", 2);
    LOG("--------------\n");

    if (TRANSFORM_SAMPLES_LOG <= 10)
    {
        if (NRMSD > 0.0000001)
        {
            printf("NRMSD = %2.8f > 0.0000001, RIFFT (Backward) FAIL\n", NRMSD);
            return -1;
        }
    }
    for (i = 0; i < TRANSFORM_SAMPLES; i++)
    {
        ran_q31_in[i] >>= 1;
    }
    // Random case
    riscv_dsp_rifft_q31(ran_q31_in, TRANSFORM_SAMPLES_LOG);
    NRMSD = dump2_inv_goldendiff_q31("after RIFFT", ran_buf_rifft_golden, ran_q31_in, TRANSFORM_SAMPLES);
    if (TRANSFORM_SAMPLES_LOG <= 10)
    {
        if (NRMSD >  0.00000011)
        {
            printf("check randon\n");
            printf("NRMSD = %2.8f > 0.00000011, RIFFT (Forward + Backward) FAIL\n", NRMSD);
            return -1;
        }
    }
    else
    {
        if ((NRMSD / TRANSFORM_SAMPLES_LOG) >  0.0000002)
        {
            printf("check randon\n");
            printf("NRMSD = %2.8f > 0.0000002, RIFFT (Forward + Backward) FAIL\n", NRMSD);
            return -1;
        }
    }
    for (i = 0; i < TRANSFORM_SAMPLES; i++)
    {
        buf3[i] >>= 1;
    }

    riscv_dsp_rifft_q31(buf3, TRANSFORM_SAMPLES_LOG);
    printf("after rFFT (Forward + Backward), ");
    NRMSD = dump2_inv_goldendiff_q31("after RIFFT", golden_idct, buf3, TRANSFORM_SAMPLES);

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
            printf("NRMSD = %2.8f > 0.0000001, RIFFT (Forward + Backward) FAIL\n", NRMSD);
            return -1;
        }
    }
    else
    {
        return 0;
    }
}
