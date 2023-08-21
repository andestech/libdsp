#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "riscv_dsp_test_accuracy.h"
#include "riscv_dsp_filtering_math.h"

#define BLOCKSIZE 16
#define NUMSTAGE 15

int main(void)
{
    uint32_t i;
    uint32_t loop = 64;
    q15_t state[NUMSTAGE + BLOCKSIZE] = {0.0};
    q15_t coe[NUMSTAGE] = {0x800, 0x1000, 0x400, 0x200, 0x100, 0x800, 0x1000, 0x400, 0x200, 0x100, 0x800, 0x1000,
                           0x400, 0x200, 0x100
                          };
    q15_t vcoe[NUMSTAGE + 1] = {0x1000, 0x400, 0x200, 0x100, 0x800, 0x1000, 0x400, 0x200, 0x100, 0x800, 0x1000, 0x400,
                                0x200, 0x100, 0x800, 0x1000
                               };
    riscv_dsp_liir_q15_t Si = {NUMSTAGE, state, coe, vcoe};

    uint32_t bsize = BLOCKSIZE;
    q15_t input[bsize] __attribute__ ((aligned(4)));
    q15_t output[bsize * loop] __attribute__ ((aligned(4)));
    q15_t golden[bsize * loop];
    q15_t * tmpout = output;

    FILE *fptr;
    FILE *gptr;
    const char *srcdir = GETENV_SRCDIR;

    char path_buffer[512];
    sprintf (path_buffer, "%s/input_q15.bin", srcdir);
    fptr = fopen(path_buffer, "r");
    if (fptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/golden_iir_q15.bin", srcdir);
    gptr = fopen(path_buffer, "r");
    if (gptr == NULL)
        return -1;


    for (i = 0; i < loop; i++)
    {
        test_Fread(&input, sizeof(q15_t), bsize, fptr);
        riscv_dsp_liir_q15(&Si, input, tmpout, bsize);
        tmpout += bsize;
    }
    test_Fread(&golden, sizeof(q15_t), bsize * loop, gptr);
    dump2_q15(golden, output, bsize * loop);

    fclose(fptr);
    fclose(gptr);

    float32_t NRMSD;
    float32_t golden_f32[bsize * loop], output_f32[bsize * loop];
    riscv_dsp_convert_q15_f32(golden, golden_f32, bsize * loop);
    riscv_dsp_convert_q15_f32(output, output_f32, bsize * loop);

    NRMSD = dump_rmsd_avg(golden_f32, output_f32, bsize, loop);

    if (NRMSD <= 0.0008)
        return 0;
    else
        return -1;
}
