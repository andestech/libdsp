#include <stdio.h>
#include <math.h>
#include <inttypes.h>       //for PRId32
#include "riscv_dsp_statistics_math.h"
#include "riscv_dsp_test_accuracy.h"

int main(void)
{
    uint32_t i;
    int datasize = 250;
    uint32_t blocksize = 25;
    uint32_t loop = datasize / blocksize;
    q7_t input1[blocksize], input2[blocksize];
    q7_t result;
    q7_t outputda[loop];
#ifndef WRITE_GOLDEN
    q7_t golden[loop];
#endif

    FILE *fptr;
    FILE *f2ptr;
    FILE *gptr;
    const char *srcdir = GETENV_SRCDIR;

    char path_buffer[512];
    sprintf (path_buffer, "%s/input_mult_q7_a.bin", srcdir);
    fptr = fopen(path_buffer, "r");
    if (fptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/input_mult_q7_b.bin", srcdir);
    f2ptr = fopen(path_buffer, "r");
    if (f2ptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/golden_mse_q7.bin", srcdir);
#ifdef WRITE_GOLDEN
    gptr = fopen(path_buffer, "wb+");
#else
    gptr = fopen(path_buffer, "r");
#endif
    if (gptr == NULL)
        return -1;

    for (i = 0; i < loop; i++)
    {
        test_Fread(&input1, sizeof(q7_t), blocksize, fptr);
        test_Fread(&input2, sizeof(q7_t), blocksize, f2ptr);
        riscv_dsp_mse_q7(input1, input2, blocksize, &result);
        outputda[i] = result;
    }
    fclose(fptr);
    fclose(f2ptr);
#ifdef WRITE_GOLDEN
    fwrite(&outputda, sizeof(q7_t), loop, gptr);
    printf("write golden to golden_mse_q7.bin\n");
    fclose(gptr);
#else
    test_Fread(&golden, sizeof(q7_t), loop, gptr);
    dump2_q7(golden, outputda, loop);

    fclose(gptr);

    float32_t golden_f32[loop], output_f32[loop];
    riscv_dsp_convert_q7_f32(golden, golden_f32, loop);
    riscv_dsp_convert_q7_f32(outputda, output_f32, loop);

    float32_t NRMSD;
    NRMSD = dump_rmsd(golden_f32, output_f32, loop);

    if (NRMSD < 0.00004)
        return 0;
    else
        return -1;
#endif
}
