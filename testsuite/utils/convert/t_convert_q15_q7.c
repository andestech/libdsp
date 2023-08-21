#include <stdio.h>
#include <math.h>
#include "riscv_dsp_utils_math.h"
#include "riscv_dsp_test_accuracy.h"

int main(void)
{
    uint32_t i;
    const uint32_t totalsize = 65530;       //already over the capacity of $LC.
    const uint32_t testtimes = 2;
    const uint32_t blocksize = totalsize / testtimes;
    FILE *fptr, *gptr;
    q15_t input[blocksize];
    q7_t golden[totalsize], out[totalsize];
    q7_t *pgolden = golden;
    q7_t *pout = out;

    const char *srcdir = GETENV_SRCDIR;
    char path_buffer[512];
    sprintf (path_buffer, "%s/input_q15.bin", srcdir);
    fptr = fopen(path_buffer, "r");
    if (fptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/golden_q15_to_q7.bin", srcdir);
    gptr = fopen(path_buffer, "r");
    if (gptr == NULL)
        return -1;

    for (i = 0; i < testtimes; i++)
    {
        LOG("Test accuracy [%d/%d] ... \n", (int)i, (int)testtimes);

        test_Fread(input, sizeof(q15_t), blocksize, fptr);
        test_Fread(pgolden, sizeof(q7_t), blocksize, gptr);
        riscv_dsp_convert_q15_q7(input, pout, blocksize);
        dump2_q7(pgolden, pout, blocksize);

        pgolden += blocksize;
        pout += blocksize;
    }

    fclose(fptr);
    fclose(gptr);

    float32_t NRMSD;
    float32_t golden_f32[totalsize], output_f32[totalsize];
    riscv_dsp_convert_q7_f32(golden, golden_f32, totalsize);
    riscv_dsp_convert_q7_f32(out, output_f32, totalsize);

    NRMSD = dump_rmsd(golden_f32, output_f32, totalsize);

    if (NRMSD <= 0.003)
        return 0;
    else
        return -1;
}
