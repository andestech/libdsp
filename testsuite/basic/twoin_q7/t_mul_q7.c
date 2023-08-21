#include <stdio.h>
#include <math.h>
#include "riscv_dsp_test_accuracy.h"
#include "riscv_dsp_basic_math.h"

double dec2float(q7_t in);
int main(void)
{
    uint32_t blocksize = 255;
    q7_t inputAvec[blocksize] __attribute__ ((aligned(4)));
    q7_t inputBvec[blocksize] __attribute__ ((aligned(4)));
    q7_t outputvec[blocksize] __attribute__ ((aligned(4)));
    q7_t goldenvec[blocksize];

    FILE *fptr;
    FILE *f2ptr;
    FILE *gptr;
    const char *srcdir = GETENV_SRCDIR;

    char path_buffer[512];
    sprintf (path_buffer, "%s/input_mult_q7_a.bin", srcdir);
    fptr = fopen(path_buffer, "r");
    if (fptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/input_mult_q7_b.bin", srcdir);
    f2ptr = fopen(path_buffer, "r");
    if (fptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/golden_mult_q7.bin", srcdir);
    gptr = fopen(path_buffer, "r");
    if (gptr == NULL)
        return -1;

    test_Fread(&inputAvec, sizeof(q7_t), blocksize, fptr);
    test_Fread(&inputBvec, sizeof(q7_t), blocksize, f2ptr);
    test_Fread(&goldenvec, sizeof(q7_t), blocksize, gptr);

    riscv_dsp_mul_q7(inputAvec, inputBvec, outputvec, blocksize);
    dump2_q7(goldenvec, outputvec, blocksize);

    fclose(fptr);
    fclose(f2ptr);
    fclose(gptr);

    float32_t NRMSD;
    float32_t golden_f32[blocksize], output_f32[blocksize];
    riscv_dsp_convert_q7_f32(goldenvec, golden_f32, blocksize);
    riscv_dsp_convert_q7_f32(outputvec, output_f32, blocksize);

    NRMSD = dump_rmsd(golden_f32, output_f32, blocksize);

    if (NRMSD <= 0.0007)
        return 0;
    else
        return -1;
}
