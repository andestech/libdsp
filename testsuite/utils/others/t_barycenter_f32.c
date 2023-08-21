#include <stdio.h>
#include <math.h>
#include "riscv_dsp_test_accuracy.h"
#include "riscv_dsp_utils_math.h"

int main(void)
{
    uint32_t numofvec = 1250;
    uint32_t dimofvec = 8;
    uint32_t totalsize = numofvec * dimofvec;
    float32_t input[totalsize];
    float32_t weight[numofvec];
    float32_t output[dimofvec], golden[dimofvec];

    FILE *fptr;
    FILE *f2ptr;
    FILE *gptr;
    const char *srcdir = GETENV_SRCDIR;

    char path_buffer[512];
    sprintf (path_buffer, "%s/input_f32.bin", srcdir);
    fptr = fopen(path_buffer, "r");
    if (fptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/input_f32_2.bin", srcdir);
    f2ptr = fopen(path_buffer, "r");
    if (f2ptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/golden_barycenter_f32.bin", srcdir);
    gptr = fopen(path_buffer, "r");
    if (gptr == NULL)
        return -1;

    test_Fread(&input, sizeof(float32_t), totalsize, fptr);
    test_Fread(&weight, sizeof(float32_t), numofvec, f2ptr);
    riscv_dsp_barycenter_f32(input, weight, output, numofvec, dimofvec);

    test_Fread(&golden, sizeof(float32_t), dimofvec, gptr);
    dump2_f32(golden, output, dimofvec);

    fclose(fptr);
    fclose(f2ptr);
    fclose(gptr);

    float32_t NRMSD;
    NRMSD = dump_rmsd(golden, output, dimofvec);

    if (NRMSD <= 0.0000005)
        return 0;
    else
        return -1;
}
