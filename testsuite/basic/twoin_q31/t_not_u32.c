#include <stdio.h>
#include <math.h>
#include <inttypes.h>       //for PRId32
#include "riscv_dsp_basic_math.h"
#include "riscv_dsp_test_accuracy.h"

int main(void)
{
    uint32_t blocksize = 16383;
    u32_t input1[blocksize];
    u32_t outputvec[blocksize], goldenvec[blocksize];

    FILE *fptr;
    FILE *gptr;
    const char *srcdir = GETENV_SRCDIR;

    char path_buffer[512];
    sprintf (path_buffer, "%s/input_dot_a.bin", srcdir);
    fptr = fopen(path_buffer, "r");
    if (fptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/golden_not_u32.bin", srcdir);
    gptr = fopen(path_buffer, "r");
    if (gptr == NULL)
        return -1;

    test_Fread(input1, sizeof(u32_t), blocksize, fptr);
    //dump_u32("rnotomly generated input array", inputvec, blocksize);
    test_Fread(goldenvec, sizeof(u32_t), blocksize, gptr);
    fclose(fptr);
    fclose(gptr);

    riscv_dsp_not_u32(input1, outputvec, blocksize);
#ifdef WRITE_GOLDEN
    FILE *fptr_golden = NULL;
    sprintf (path_buffer, "%s/golden_not_u32_out.bin", srcdir);
    fptr_golden = fopen(path_buffer, "wb");
    fwrite(outputvec, sizeof(u32_t), blocksize, fptr_golden);
    fclose(fptr_golden);
#endif

    dump2_u32(goldenvec, outputvec, blocksize);

    float32_t NRMSD;
    float32_t golden_f32[blocksize], output_f32[blocksize];
    _dsp_convert_u32_f32(goldenvec, golden_f32, blocksize);
    _dsp_convert_u32_f32(outputvec, output_f32, blocksize);

    NRMSD = dump_rmsd(golden_f32, output_f32, blocksize);

    if (NRMSD == 0)
        return 0;
    else
        return -1;
}
