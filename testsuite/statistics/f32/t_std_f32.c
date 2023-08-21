#include <stdio.h>
#include <math.h>
#include "riscv_dsp_test_accuracy.h"
#include "riscv_dsp_statistics_math.h"

int main(void)
{
    uint32_t i;
    int datasize = 10000;
    uint32_t blocksize = 8;
    uint32_t loop = datasize / blocksize;
    float32_t input[blocksize];
    float32_t outputda[loop], golden[loop];

    FILE *fptr;
    FILE *gptr;
    const char *srcdir = GETENV_SRCDIR;

    char path_buffer[512];
    sprintf (path_buffer, "%s/input_std_f32.bin", srcdir);
    fptr = fopen(path_buffer, "r");
    if (fptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/golden_std_f32.bin", srcdir);
    gptr = fopen(path_buffer, "r");
    if (gptr == NULL)
        return -1;

    for (i = 0; i < loop; i++)
    {
        test_Fread(&input, sizeof(float32_t), blocksize, fptr);
        outputda[i] = riscv_dsp_std_f32(input, blocksize);
    }
    test_Fread(&golden, sizeof(float32_t), loop, gptr);
    dump2_f32(golden, outputda, loop);

    fclose(fptr);
    fclose(gptr);

    float32_t NRMSD;
    NRMSD = dump_rmsd(golden, outputda, loop);

    if (NRMSD <= 0.0000002)
        return 0;
    else
        return -1;
}
