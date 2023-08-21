#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "riscv_dsp_basic_math.h"
#include <inttypes.h>       //for PRIx32
#include "riscv_dsp_test_accuracy.h"

double dec2float(q31_t in);
int main(void)
{
    uint32_t i;
    uint32_t blocksize = 1000;

    uint64_t inputAvec;
    uint32_t inputBvec;
    q31_t outputvec[blocksize], goldenvec[blocksize];
    FILE *fptr;
    FILE *f2ptr;
    FILE *gptr;
    const char *srcdir = GETENV_SRCDIR;

    char path_buffer[512];
    sprintf (path_buffer, "%s/input_u64.bin", srcdir);
    fptr = fopen(path_buffer, "r");
    if (fptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/input_u64_u32.bin", srcdir);
    f2ptr = fopen(path_buffer, "r");
    if (fptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/golden_div_u64.bin", srcdir);
    gptr = fopen(path_buffer, "r");
    if (gptr == NULL)
        return -1;

    for (i = 0; i < blocksize; i++)
    {
        test_Fread(&inputAvec, sizeof(uint64_t), 1, fptr);
        test_Fread(&inputBvec, sizeof(uint32_t), 1, f2ptr);
        outputvec[i] = riscv_dsp_div_u64_u32(inputAvec, inputBvec);
    }
    test_Fread(&goldenvec, sizeof(q31_t), blocksize, gptr);
    dump2_q31(goldenvec, outputvec, blocksize);

    fclose(fptr);
    fclose(f2ptr);
    fclose(gptr);

    float32_t NRMSD;
    float32_t golden_f32[blocksize], output_f32[blocksize];
    riscv_dsp_convert_q31_f32(goldenvec, golden_f32, blocksize);
    riscv_dsp_convert_q31_f32(outputvec, output_f32, blocksize);

    NRMSD = dump_rmsd(golden_f32, output_f32, blocksize);

    if (NRMSD <= 0.00000002)
        return 0;
    else
        return -1;
}
