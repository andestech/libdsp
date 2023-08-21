#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <inttypes.h>       //for PRId32
#include "riscv_dsp_filtering_math.h"
#include "riscv_dsp_test_accuracy.h"

#define BLOCKSIZE 16
#define NUMSTAGE 15
int main(void)
{
    uint32_t i;
    q31_t state[NUMSTAGE + BLOCKSIZE] = {0};
    uint32_t loop = 64;
    q31_t coe[NUMSTAGE] = {0x8000000, 0x10000000, 0x4000000, 0x2000000, 0x1000000, 0x8000000, 0x10000000, 0x4000000, 0x2000000, 0x1000000, 0x8000000, 0x10000000,
                           0x4000000, 0x2000000, 0x1000000
                          };
    q31_t vcoe[NUMSTAGE + 1] = {0x10000000, 0x4000000, 0x2000000, 0x1000000, 0x8000000, 0x10000000, 0x4000000, 0x2000000, 0x1000000, 0x8000000, 0x10000000, 0x4000000,
                                0x2000000, 0x1000000, 0x8000000, 0x10000000
                               };
    riscv_dsp_liir_q31_t Si = {NUMSTAGE, state, coe, vcoe};

    uint32_t bsize = BLOCKSIZE;
    q31_t input[bsize], output[bsize * loop], golden[bsize * loop];
    q31_t * tmpout = output;

    FILE *fptr;
    FILE *gptr;
    const char *srcdir = GETENV_SRCDIR;

    char path_buffer[512];
    sprintf (path_buffer, "%s/input_q31.bin", srcdir);
    fptr = fopen(path_buffer, "r");
    if (fptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/golden_iir_q31.bin", srcdir);
    gptr = fopen(path_buffer, "r");
    if (gptr == NULL)
        return -1;

    for (i = 0; i < loop; i++)
    {
        test_Fread(&input, sizeof(q31_t), bsize, fptr);
        riscv_dsp_liir_q31(&Si, input, tmpout, bsize);
        tmpout += bsize;
    }
    test_Fread(&golden, sizeof(q31_t), bsize * loop, gptr);
    dump2_q31(golden, output, bsize * loop);

    fclose(fptr);
    fclose(gptr);

    float32_t NRMSD;
    float32_t golden_f32[bsize * loop], output_f32[bsize * loop];
    riscv_dsp_convert_q31_f32(golden, golden_f32, bsize * loop);
    riscv_dsp_convert_q31_f32(output, output_f32, bsize * loop);

    NRMSD = dump_rmsd_avg(golden_f32, output_f32, bsize, loop);

    if (NRMSD <= 0.00000002)
        return 0;
    else
        return -1;
}
