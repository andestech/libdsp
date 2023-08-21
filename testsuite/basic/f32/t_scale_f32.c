#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "riscv_dsp_basic_math.h"
#include "riscv_dsp_test_accuracy.h"

int main(void)
{
    uint32_t blocksize = 2048;
    float32_t scaleval = 0.432;
    float inputvec[blocksize];
    float outputvec[blocksize];
    float goldenvec[blocksize];

    FILE *fptr;
    FILE *gptr;
    const char *srcdir = GETENV_SRCDIR;

    char path_buffer[512];
    sprintf (path_buffer, "%s/input_f32.bin", srcdir);
    fptr = fopen(path_buffer, "r");
    if (fptr == NULL)
        return -1;
    sprintf (path_buffer, "%s/golden_scale_f32.bin", srcdir);
    gptr = fopen(path_buffer, "r");
    if (gptr == NULL)
        return -1;

    test_Fread(&inputvec, sizeof(float32_t), blocksize, fptr);
    dump_f32("randomly generated input array", inputvec, blocksize);
    test_Fread(&goldenvec, sizeof(float32_t), blocksize, gptr);
    fclose(fptr);
    fclose(gptr);

    riscv_dsp_scale_f32(inputvec, scaleval, outputvec, blocksize);
    dump2_f32(goldenvec, outputvec, blocksize);

    float32_t NRMSD;
    NRMSD = dump_rmsd(goldenvec, outputvec, blocksize);

    if (NRMSD <= 0.00000003)
        return 0;
    else
        return -1;
}
