#include <stdio.h>
#include <math.h>
#include "riscv_dsp_test_accuracy.h"
#include "riscv_dsp_basic_math.h"

int main(void)
{
    uint32_t blocksize = 65536;
    float inputAvec[blocksize], inputBvec[blocksize], outputvec[blocksize], goldenvec[blocksize];

    FILE *fptr;
    FILE *gptr;
    FILE *f2ptr;
    const char *srcdir = GETENV_SRCDIR;

    char path_buffer[512];
    sprintf (path_buffer, "%s/input_f32_a.bin", srcdir);
    fptr = fopen(path_buffer, "r");
    if (fptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/input_f32_b.bin", srcdir);
    f2ptr = fopen(path_buffer, "r");
    if (fptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/golden_div_f32.bin", srcdir);
    gptr = fopen(path_buffer, "r");
    if (gptr == NULL)
        return -1;

    test_Fread(&inputAvec, sizeof(float32_t), blocksize, fptr);
    test_Fread(&inputBvec, sizeof(float32_t), blocksize, f2ptr);
    test_Fread(&goldenvec, sizeof(float32_t), blocksize, gptr);

    riscv_dsp_div_f32(inputAvec, inputBvec, outputvec, blocksize);
    dump2_f32(goldenvec, outputvec, blocksize);

    fclose(fptr);
    fclose(f2ptr);
    fclose(gptr);

    float32_t NRMSD;
    NRMSD = dump_rmsd(goldenvec, outputvec, blocksize);

    if (NRMSD == 0)
        return 0;
    else
        return -1;
}
