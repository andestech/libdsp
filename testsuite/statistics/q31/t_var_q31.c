#include <stdio.h>
#include <math.h>
#include <inttypes.h>       //for PRId32
#include "riscv_dsp_statistics_math.h"
#include "riscv_dsp_test_accuracy.h"

int main(void)
{
    uint32_t i;
    int datasize = 1500;
    uint32_t blocksize = 15;
    uint32_t loop = datasize / blocksize;
    q31_t input[blocksize];
    q63_t outputda[loop], golden[loop];
    q63_t max = 0;

    FILE *fptr;
    FILE *gptr;
    const char *srcdir = GETENV_SRCDIR;

    char path_buffer[512];
    sprintf (path_buffer, "%s/input_var_q31.bin", srcdir);
    fptr = fopen(path_buffer, "r");
    if (fptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/golden_var_q31.bin", srcdir);
    gptr = fopen(path_buffer, "r");
    if (gptr == NULL)
        return -1;

    for (i = 0; i < loop; i++)
    {
        test_Fread(&input, sizeof(q31_t), blocksize, fptr);
        outputda[i] = riscv_dsp_var_q31(input, blocksize);
    }
    test_Fread(&golden, sizeof(q63_t), loop, gptr);
    max = dump2_q63(golden, outputda, loop);

    fclose(fptr);
    fclose(gptr);

    double golden_f32[loop], output_f32[loop];
    _dsp_convert_q63_f64(golden, golden_f32, loop);
    _dsp_convert_q63_f64(outputda, output_f32, loop);

    dump_rmsd64(golden_f32, output_f32, loop);

    if (max <= 1)
        return 0;
    else
        return -1;
}
