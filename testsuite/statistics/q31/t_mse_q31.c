#include <stdio.h>
#include <math.h>
#include <inttypes.h>       //for PRId32
#include "riscv_dsp_statistics_math.h"
#include "riscv_dsp_test_accuracy.h"

int main(void)
{
    uint32_t i;
    int datasize = 9990;
    uint32_t blocksize = 999;
    uint32_t loop = datasize / blocksize;
    q31_t input1[blocksize], input2[blocksize];
    q31_t result;
    q31_t outputda[loop], golden[loop];

    FILE *fptr;
    FILE *f2ptr;
    FILE *gptr;
    const char *srcdir = GETENV_SRCDIR;

    char path_buffer[512];
    sprintf (path_buffer, "%s/input_sat_q31.bin", srcdir);
    fptr = fopen(path_buffer, "r");
    if (fptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/input_std_q31.bin", srcdir);
    f2ptr = fopen(path_buffer, "r");
    if (f2ptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/golden_mse_q31.bin", srcdir);
    gptr = fopen(path_buffer, "r");
    if (gptr == NULL)
        return -1;

    for (i = 0; i < loop; i++)
    {
        test_Fread(&input1, sizeof(q31_t), blocksize, fptr);
        test_Fread(&input2, sizeof(q31_t), blocksize, f2ptr);
        riscv_dsp_mse_q31(input1, input2, blocksize, &result);
        outputda[i] = result;
    }
    test_Fread(&golden, sizeof(q31_t), loop, gptr);
    dump2_q31(golden, outputda, loop);

    fclose(fptr);
    fclose(gptr);

    float32_t golden_f32[loop], output_f32[loop];
    riscv_dsp_convert_q31_f32(golden, golden_f32, loop);
    riscv_dsp_convert_q31_f32(outputda, output_f32, loop);

    float32_t NRMSD;
    NRMSD = dump_rmsd(golden_f32, output_f32, loop);

    if (NRMSD == 0)
        return 0;
    else
        return -1;
}
