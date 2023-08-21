#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "riscv_dsp_test_accuracy.h"
#include "riscv_dsp_filtering_math.h"

#define BLOCKSIZE 1024
#define NUMSTAGE 15

int main(void)
{
    float32_t state[NUMSTAGE + BLOCKSIZE] = {0.0};
    float32_t coe[NUMSTAGE] = {0.0625, 0.125, 0.03125, 0.015625 , 0.0078125 , 0.0625, 0.125, 0.03125, 0.015625,
                               0.0078125, 0.0625, 0.125, 0.03125, 0.015625, 0.0078125
                              };
    float32_t vcoe[NUMSTAGE + 1] = {0.125, 0.03125, 0.015625, 0.0078125, 0.0625, 0.125, 0.03125, 0.015625, 0.0078125,
                                    0.0625, 0.125, 0.03125, 0.015625, 0.0078125, 0.0625, 0.125
                                   };
    riscv_dsp_liir_f32_t Si = {NUMSTAGE, state, coe, vcoe};

    uint32_t bsize = BLOCKSIZE;
    float32_t input[bsize], output[bsize], golden[bsize];

    FILE *fptr;
    FILE *gptr;
    const char *srcdir = GETENV_SRCDIR;

    char path_buffer[512];
    sprintf (path_buffer, "%s/input_f32.bin", srcdir);
    fptr = fopen(path_buffer, "r");
    if (fptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/golden_iir_f32.bin", srcdir);
    gptr = fopen(path_buffer, "r");
    if (gptr == NULL)
        return -1;

    test_Fread(&input, sizeof(float32_t), bsize, fptr);
    test_Fread(&golden, sizeof(float32_t), bsize, gptr);
    riscv_dsp_liir_f32(&Si, input, output, bsize);
    dump2_f32(golden, output, bsize);

    fclose(fptr);
    fclose(gptr);

    float32_t NRMSD;
    NRMSD = dump_rmsd(golden, output, bsize);

    if (NRMSD <= 0.00000006)
        return 0;
    else
        return -1;
}
