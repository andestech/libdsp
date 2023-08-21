#include <stdio.h>
#include <math.h>
#include "riscv_dsp_test_accuracy.h"
#include "riscv_dsp_statistics_math.h"

int main(void)
{
    uint32_t i;
    int datasize = 30000;
    uint32_t blocksize = 15;
    uint32_t loop = datasize / blocksize;
    q15_t input[blocksize] __attribute__ ((aligned(4)));
    q15_t outputda[loop], golden[loop];

    FILE *fptr;
    fptr = fopen("input_sat_q15.bin", "r");
    FILE *gptr;
    gptr = fopen("golden_rms_q15.bin", "r");
    const char *srcdir = GETENV_SRCDIR;

    char path_buffer[512];
    sprintf (path_buffer, "%s/input_sat_q15.bin", srcdir);
    fptr = fopen(path_buffer, "r");
    if (fptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/golden_rms_q15.bin", srcdir);
    gptr = fopen(path_buffer, "r");
    if (gptr == NULL)
        return -1;

    for (i = 0; i < loop; i++)
    {
        test_Fread(&input, sizeof(q15_t), blocksize, fptr);
        outputda[i] = riscv_dsp_rms_q15(input, blocksize);
    }
    test_Fread(&golden, sizeof(q15_t), loop, gptr);
    dump2_q15(golden, outputda, loop);

    fclose(fptr);
    fclose(gptr);

    float32_t NRMSD;
    float32_t golden_f32[loop], output_f32[loop];
    riscv_dsp_convert_q15_f32(golden, golden_f32, loop);
    riscv_dsp_convert_q15_f32(outputda, output_f32, loop);

    NRMSD = dump_rmsd(golden_f32, output_f32, loop);

    if (NRMSD <= 0.0002)
        return 0;
    else
        return -1;
}
