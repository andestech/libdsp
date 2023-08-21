#include <stdio.h>
#include <math.h>
#include <inttypes.h>       //for PRId32
#include "riscv_dsp_basic_math.h"
#include "riscv_dsp_test_accuracy.h"

int main(void)
{
    uint32_t blocksize = 32767;
    q31_t inputvec[blocksize], outputvec[blocksize], goldenvec[blocksize];

    FILE *fptr;
    FILE *gptr;
    const char *srcdir = GETENV_SRCDIR;

    char path_buffer[512];
    sprintf (path_buffer, "%s/input_q31.bin", srcdir);
    fptr = fopen(path_buffer, "r");
    if (fptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/golden_abs_q31.bin", srcdir);
    gptr = fopen(path_buffer, "r");
    if (gptr == NULL)
        return -1;

    test_Fread(inputvec, sizeof(q31_t), blocksize, fptr);
    dump_q31("randomly generated input array", inputvec, blocksize);
    test_Fread(goldenvec, sizeof(q31_t), blocksize, gptr);
    fclose(fptr);
    fclose(gptr);

    riscv_dsp_abs_q31(inputvec, outputvec, blocksize);
    dump2_q31(goldenvec, outputvec, blocksize);

    float32_t NRMSD;
    float32_t golden_f32[blocksize], output_f32[blocksize];
    riscv_dsp_convert_q31_f32(goldenvec, golden_f32, blocksize);
    riscv_dsp_convert_q31_f32(outputvec, output_f32, blocksize);

    NRMSD = dump_rmsd(golden_f32, output_f32, blocksize);

    if (NRMSD == 0)
        return 0;
    else
        return -1;
}
