#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "riscv_dsp_basic_math.h"
#include "riscv_dsp_test_accuracy.h"

int main(void)
{
    uint32_t blocksize = 255;
    int8_t shiftval = 2;
    q7_t inputvec[blocksize] __attribute__ ((aligned(4)));
    q7_t outputvec[blocksize] __attribute__ ((aligned(4)));
    q7_t scaleval = 0x8, goldenvec[blocksize];

    FILE *fptr;
    FILE *gptr;
    const char *srcdir = GETENV_SRCDIR;

    char path_buffer[512];
    sprintf (path_buffer, "%s/input_q7.bin", srcdir);
    fptr = fopen(path_buffer, "r");
    if (fptr == NULL)
        return -1;
    sprintf (path_buffer, "%s/golden_scale_q7.bin", srcdir);
    gptr = fopen(path_buffer, "r");
    if (gptr == NULL)
        return -1;

    test_Fread(&inputvec, sizeof(q7_t), blocksize, fptr);
    dump_q7("randomly generated input array", inputvec, blocksize);
    test_Fread(&goldenvec, sizeof(q7_t), blocksize, gptr);
    fclose(fptr);
    fclose(gptr);

    riscv_dsp_scale_q7(inputvec, scaleval, shiftval, outputvec, blocksize);
    dump2_q7(goldenvec, outputvec, blocksize);

    float32_t NRMSD = 0;
    float32_t golden_f32[blocksize], output_f32[blocksize];
    riscv_dsp_convert_q7_f32(goldenvec, golden_f32, blocksize);
    riscv_dsp_convert_q7_f32(outputvec, output_f32, blocksize);

    NRMSD = dump_rmsd(golden_f32, output_f32, blocksize);

    if (NRMSD <= 0.0056)
        return 0;
    else
        return -1;
}
