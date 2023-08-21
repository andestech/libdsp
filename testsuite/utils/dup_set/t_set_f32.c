#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "riscv_dsp_utils_math.h"
#include "riscv_dsp_test_accuracy.h"

int main(void)
{
    int i;
    int datasize = 1000;
    uint32_t blocksize = 4;

    FILE *fptr, *gptr;
    float input;
    float golden[blocksize * datasize], out[blocksize * datasize];
    float *tmpout = out;

    const char *srcdir = GETENV_SRCDIR;
    char path_buffer[512];
    sprintf (path_buffer, "%s/input_f32_1.bin", srcdir);
    fptr = fopen(path_buffer, "r");
    sprintf (path_buffer, "%s/golden_set_f32.bin", srcdir);
    gptr = fopen(path_buffer, "r");

    for (i = 0; i < datasize; i++)
    {
        test_Fread(&input, sizeof(float), 1, fptr);
        riscv_dsp_set_f32(input, tmpout, blocksize);
        tmpout += blocksize;
    }
    test_Fread(&golden, sizeof(float), blocksize * datasize, gptr);
    dump2_f32(golden, out, blocksize * datasize);

    fclose(fptr);
    fclose(gptr);

    float32_t NRMSD;
    NRMSD = dump_rmsd(golden, out, blocksize * datasize);

    if (NRMSD == 0)
        return 0;
    else
        return -1;
}
