#include <stdio.h>
#include <math.h>
#include "riscv_dsp_test_accuracy.h"
#include "riscv_dsp_basic_math.h"

int main(void)
{
    uint32_t i;
    uint32_t blocksize = 15;
    uint32_t loop = 15000 / blocksize;
    unsigned char inputAvec[blocksize] __attribute__ ((aligned(4)));
    short inputBvec[blocksize] __attribute__ ((aligned(4)));

    q31_t output[loop], golden[loop];

    FILE *fptr;
    FILE *f2ptr;
    FILE *gptr;
    const char *srcdir = GETENV_SRCDIR;

    char path_buffer[512];
    sprintf (path_buffer, "%s/input_u8.bin", srcdir);
    fptr = fopen(path_buffer, "r");
    if (fptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/input_dot_q15_a.bin", srcdir);
    f2ptr = fopen(path_buffer, "r");
    if (fptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/golden_dot_prod_u8xq15.bin", srcdir);
    gptr = fopen(path_buffer, "r");
    if (gptr == NULL)
        return -1;

    for (i = 0; i < loop; i++)
    {
        test_Fread(inputAvec, sizeof(unsigned char), blocksize, fptr);
        test_Fread(inputBvec, sizeof(short), blocksize, f2ptr);
        output[i] = riscv_dsp_dprod_u8xq15(inputAvec, inputBvec, blocksize);
    }
    //for (i = 0; i < 5; i++)
    //{
    //    printf("%d %d \n", inputAvec[i], inputBvec[i]);
    //}

    test_Fread(&golden, sizeof(q31_t), loop, gptr);
    dump2_q31(golden, output, loop);

    fclose(fptr);
    fclose(f2ptr);
    fclose(gptr);

    float32_t NRMSD;
    float32_t golden_f32[loop], output_f32[loop];
    riscv_dsp_convert_q31_f32(golden, golden_f32, loop);
    riscv_dsp_convert_q31_f32(output, output_f32, loop);

    NRMSD = dump_rmsd(golden_f32, output_f32, loop);

    if (NRMSD == 0)
        return 0;
    else
        return -1;
}
