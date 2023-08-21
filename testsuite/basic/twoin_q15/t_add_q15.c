#include <stdio.h>
#include <math.h>
#include "riscv_dsp_test_accuracy.h"
#include "riscv_dsp_basic_math.h"

int main(void)
{
    uint32_t blocksize = 65535;
    short inputAvec[blocksize] __attribute__ ((aligned(4)));
    short inputBvec[blocksize] __attribute__ ((aligned(4)));
    short outputvec[blocksize] __attribute__ ((aligned(4)));
    short goldenvec[blocksize];

    FILE *fptr;
    FILE *gptr;
    FILE *f2ptr;
    const char *srcdir = GETENV_SRCDIR;

    char path_buffer[512];
    sprintf (path_buffer, "%s/input_q15_a.bin", srcdir);
    fptr = fopen(path_buffer, "r");
    if (fptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/input_q15_b.bin", srcdir);
    f2ptr = fopen(path_buffer, "r");
    if (fptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/golden_add_q15.bin", srcdir);
    gptr = fopen(path_buffer, "r");
    if (gptr == NULL)
        return -1;

    test_Fread(&inputAvec, sizeof(q15_t), blocksize, fptr);
    test_Fread(&inputBvec, sizeof(q15_t), blocksize, f2ptr);
    test_Fread(&goldenvec, sizeof(q15_t), blocksize, gptr);

    riscv_dsp_add_q15(inputAvec, inputBvec, outputvec, blocksize);
    dump2_q15(goldenvec, outputvec, blocksize);

    fclose(fptr);
    fclose(f2ptr);
    fclose(gptr);

    float32_t NRMSD;
    float32_t golden_f32[blocksize], output_f32[blocksize];
    riscv_dsp_convert_q15_f32(goldenvec, golden_f32, blocksize);
    riscv_dsp_convert_q15_f32(outputvec, output_f32, blocksize);

    NRMSD = dump_rmsd(golden_f32, output_f32, blocksize);

    if (NRMSD == 0)
        return 0;
    else
        return -1;
}
