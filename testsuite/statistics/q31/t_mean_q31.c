#include <stdio.h>
#include <math.h>
#include <inttypes.h>       //for PRId32
#include "riscv_dsp_statistics_math.h"
#include "riscv_dsp_test_accuracy.h"

int main(void)
{
    uint32_t i;
    int datasize = 13100;
    uint32_t blocksize = 131;
    uint32_t loop = datasize / blocksize;
    q31_t input[blocksize];
    q31_t outputda[loop], golden[loop];

    FILE *fptr;
    FILE *gptr;
    const char *srcdir = GETENV_SRCDIR;

    char path_buffer[512];
    sprintf (path_buffer, "%s/input_q31.bin", srcdir);
    fptr = fopen(path_buffer, "r");
    if (fptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/golden_mean_q31.bin", srcdir);
    gptr = fopen(path_buffer, "r");
    if (gptr == NULL)
        return -1;

    for (i = 0; i < loop; i++)
    {
        test_Fread(&input, sizeof(q31_t), blocksize, fptr);
        outputda[i] = riscv_dsp_mean_q31(input, blocksize);
    }
    test_Fread(&golden, sizeof(q31_t), loop, gptr);
    dump2_q31(golden, outputda, loop);

    fclose(fptr);
    fclose(gptr);

    float32_t NRMSD;
    float32_t golden_f32[loop], output_f32[loop];
    riscv_dsp_convert_q31_f32(golden, golden_f32, loop);
    riscv_dsp_convert_q31_f32(outputda, output_f32, loop);

    NRMSD = dump_rmsd(golden_f32, output_f32, loop);

    if (NRMSD <= 0.00000001)
        return 0;
    else
        return -1;
}
