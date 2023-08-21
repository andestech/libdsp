#include <stdio.h>
#include <math.h>
#include <inttypes.h>       //for PRId32
#include "riscv_dsp_statistics_math.h"
#include "riscv_dsp_test_accuracy.h"

int main(void)
{
    uint32_t i;
    int datasize = 9900;
    uint32_t blocksize = 99;
    uint32_t loop = datasize / blocksize;
    q15_t input1[blocksize], input2[blocksize];
    q15_t result;
    q15_t outputda[loop], golden[loop];

    FILE *fptr;
    FILE *f2ptr;
    FILE *gptr;
    const char *srcdir = GETENV_SRCDIR;

    char path_buffer[512];
    sprintf (path_buffer, "%s/input_sat_q15.bin", srcdir);
    fptr = fopen(path_buffer, "r");
    if (fptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/input_q15.bin", srcdir);
    f2ptr = fopen(path_buffer, "r");
    if (f2ptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/golden_mse_q15.bin", srcdir);
    gptr = fopen(path_buffer, "r");
    if (gptr == NULL)
        return -1;

    for (i = 0; i < loop; i++)
    {
        test_Fread(&input1, sizeof(q15_t), blocksize, fptr);
        test_Fread(&input2, sizeof(q15_t), blocksize, f2ptr);
        riscv_dsp_mse_q15(input1, input2, blocksize, &result);
        outputda[i] = result;
    }
    test_Fread(&golden, sizeof(q15_t), loop, gptr);
    dump2_q15(golden, outputda, loop);

    fclose(fptr);
    fclose(gptr);

    float32_t golden_f32[loop], output_f32[loop];
    riscv_dsp_convert_q15_f32(golden, golden_f32, loop);
    riscv_dsp_convert_q15_f32(outputda, output_f32, loop);

    float32_t NRMSD;
    NRMSD = dump_rmsd(golden_f32, output_f32, loop);

    if (NRMSD < 0.00004)
        return 0;
    else
        return -1;
}
