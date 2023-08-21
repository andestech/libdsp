#include <stdio.h>
#include <math.h>
#include "riscv_dsp_test_accuracy.h"
#include "riscv_dsp_basic_math.h"

int main(void)
{
    uint32_t blocksize = 65535;
    q15_t inputvec[blocksize] __attribute__ ((aligned(4)));
    q15_t outputvec[blocksize] __attribute__ ((aligned(4)));
    q15_t goldenvec[blocksize];

    FILE *fptr;
    FILE *gptr;
    const char *srcdir = GETENV_SRCDIR;

    char path_buffer[512];
    sprintf (path_buffer, "%s/input_q15-1.bin", srcdir);
    fptr = fopen(path_buffer, "r");
    if (fptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/golden_abs_q15.bin", srcdir);
    gptr = fopen(path_buffer, "r");
    if (gptr == NULL)
        return -1;

    test_Fread(inputvec, sizeof(q15_t), blocksize, fptr);
    dump_q15("randomly generated input array", inputvec, blocksize);
    test_Fread(goldenvec, sizeof(q15_t), blocksize, gptr);
    fclose(fptr);
    fclose(gptr);

    riscv_dsp_abs_q15(inputvec, outputvec, blocksize);
    dump2_q15(goldenvec, outputvec, blocksize);

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
