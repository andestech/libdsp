#include <stdio.h>
#include <math.h>
#include "riscv_dsp_basic_math.h"
#include "riscv_dsp_test_accuracy.h"

int main(void)
{
    uint32_t i;
    const uint32_t totalsize = 5000;       //already over the capacity of $LC.
    const uint32_t testtimes = 2;
    const uint32_t blocksize = totalsize / testtimes;
    q15_t inputvec[blocksize] __attribute__ ((aligned(4)));
    q15_t outputvec[totalsize] __attribute__ ((aligned(4)));
    q15_t goldenvec[totalsize];

    q15_t scaleval = 100;
    int8_t shiftval = 4;
    q15_t *pgolden = goldenvec;
    q15_t *pout = outputvec;

    FILE *fptr;
    FILE *gptr;
    const char *srcdir = GETENV_SRCDIR;

    char path_buffer[512];
    sprintf (path_buffer, "%s/input_q15_a.bin", srcdir);
    fptr = fopen(path_buffer, "r");
    if (fptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/golden_scale_q15.bin", srcdir);
    gptr = fopen(path_buffer, "r");
    if (gptr == NULL)
        return -1;

    for (i = 0; i < testtimes; i++)
    {
        LOG("Test accuracy [%d/%d] ... \n", (int)i, (int)testtimes);

        test_Fread(pgolden, sizeof(q15_t), blocksize, gptr);
        test_Fread(inputvec, sizeof(q15_t), blocksize, fptr);
        //dump_q15("randomly generated input array", inputvec, blocksize);

        riscv_dsp_scale_q15(inputvec, scaleval, shiftval, pout, blocksize);
        dump2_q15(pgolden, pout, blocksize);

        pgolden += blocksize;
        pout += blocksize;
    }

    // unit case
    q15_t in[1] = {0x4000}, out;
    q15_t scaleval2 = 4;
    riscv_dsp_scale_q15(in, scaleval2, shiftval, &out, 1);
    if (out != 32) return -1;

    fclose(fptr);
    fclose(gptr);

    float32_t NRMSD;
    float32_t golden_f32[totalsize], output_f32[totalsize];
    riscv_dsp_convert_q15_f32(goldenvec, golden_f32, totalsize);
    riscv_dsp_convert_q15_f32(outputvec, output_f32, totalsize);

    NRMSD = dump_rmsd(golden_f32, output_f32, totalsize);

    if (NRMSD <= 0.0003)
        return 0;
    else
        return -1;
}
