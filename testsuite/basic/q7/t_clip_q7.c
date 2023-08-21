#include <stdio.h>
#include <math.h>
#include <inttypes.h>       //for PRId32
#include "riscv_dsp_basic_math.h"
#include "riscv_dsp_test_accuracy.h"

int main(void)
{
    uint32_t blocksize = 255;
    q7_t inputvec[blocksize], outputvec[blocksize], goldenvec[blocksize];
    q7_t high = 0x4f;
    q7_t low = 0xa0;

    FILE *fptr;
    FILE *gptr;
    const char *srcdir = GETENV_SRCDIR;

    char path_buffer[512];
    sprintf (path_buffer, "%s/input_q7.bin", srcdir);
    fptr = fopen(path_buffer, "r");
    if (fptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/golden_clip_q7.bin", srcdir);
    gptr = fopen(path_buffer, "r");
    if (gptr == NULL)
        return -1;

    test_Fread(inputvec, sizeof(q7_t), blocksize, fptr);
    //dump_q7("randomly generated input array", inputvec, blocksize);
    test_Fread(goldenvec, sizeof(q7_t), blocksize, gptr);

    fclose(fptr);
    fclose(gptr);

    riscv_dsp_clip_q7(inputvec, outputvec, low, high, blocksize);
#if 0
    FILE *fptr_golden = NULL;
    sprintf (path_buffer, "%s/golden_clip_q7_out.bin", srcdir);
    fptr_golden = fopen(path_buffer, "wb");
    fwrite(outputvec, sizeof(q7_t), blocksize, fptr_golden);
    fclose(fptr_golden);
#endif

    dump2_q7(goldenvec, outputvec, blocksize);

    float32_t NRMSD;
    float32_t golden_f32[blocksize], output_f32[blocksize];
    riscv_dsp_convert_q7_f32(goldenvec, golden_f32, blocksize);
    riscv_dsp_convert_q7_f32(outputvec, output_f32, blocksize);

    NRMSD = dump_rmsd(golden_f32, output_f32, blocksize);

    if (NRMSD == 0)
        return 0;
    else
        return -1;
}
