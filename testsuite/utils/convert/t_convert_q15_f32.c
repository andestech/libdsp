#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "riscv_dsp_utils_math.h"
#include "riscv_dsp_test_accuracy.h"

int main(void)
{
    uint32_t blocksize = 65536;

    FILE *fptr, *gptr;
    q15_t input[blocksize];
    float32_t golden[blocksize], out[blocksize];

    const char *srcdir = GETENV_SRCDIR;
    char path_buffer[512];
    sprintf (path_buffer, "%s/input_q15.bin", srcdir);
    fptr = fopen(path_buffer, "r");
    sprintf (path_buffer, "%s/golden_q15_to_f.bin", srcdir);
    gptr = fopen(path_buffer, "r");

    test_Fread(&input, sizeof(q15_t), blocksize, fptr);
    test_Fread(&golden, sizeof(float32_t), blocksize, gptr);
    riscv_dsp_convert_q15_f32(input, out, blocksize);
    dump2_f32(golden, out, blocksize);

    fclose(fptr);
    fclose(gptr);

    float32_t NRMSD;
    NRMSD = dump_rmsd(golden, out, blocksize);

    if (NRMSD == 0)
        return 0;
    else
        return -1;
}
