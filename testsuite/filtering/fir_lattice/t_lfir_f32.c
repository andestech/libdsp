#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <inttypes.h>       //for PRId32
#include "riscv_dsp_filtering_math.h"
#include "riscv_dsp_test_accuracy.h"

#define STAGE 16
#define BLOCKSIZE 1024

int main(void)
{
    uint32_t i;
    uint16_t stage = STAGE;
    uint32_t bsize = BLOCKSIZE;
    float32_t *state = (float32_t *)calloc(stage, sizeof(float32_t));
    float32_t *coe = (float32_t *)calloc(stage, sizeof(float32_t));
    coe[0] = 0.1;
    coe[1] = 0.2;
    coe[2] = 0.3;
    coe[3] = 0.4;
    coe[4] = 0.5;
    coe[5] = 0.6;

    riscv_dsp_lfir_f32_t Si = {stage, state, coe};

    float32_t input[bsize], output[bsize], golden[bsize], golden_state[stage];

    FILE *fptr;
    FILE *gptr;
    FILE *g2ptr;
    const char *srcdir = GETENV_SRCDIR;

    char path_buffer[512];
    sprintf (path_buffer, "%s/input_f32.bin", srcdir);
    fptr = fopen(path_buffer, "r");
    if (fptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/golden_fir_lattice_f32.bin", srcdir);
    gptr = fopen(path_buffer, "r");
    if (gptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/golden_fir_lattice_state_f32.bin", srcdir);
    g2ptr = fopen(path_buffer, "r");
    if (g2ptr == NULL)
        return -1;

    test_Fread(&input, sizeof(float32_t), bsize, fptr);
    test_Fread(&golden, sizeof(float32_t), bsize, gptr);
    test_Fread(&golden_state, sizeof(float32_t), stage, g2ptr);


    for (i = 0; i < STAGE; i++)
        coe[i] = (float)(i + 1) / 10;

    riscv_dsp_lfir_f32(&Si, input, output, bsize);
    dump2_f32(golden, output, bsize);
    float32_t maxdiff = dump2_f32(golden_state, state, stage);
    if (maxdiff > 0.0003) return -1;

    free(state);
    free(coe);
    fclose(fptr);
    fclose(gptr);

    float32_t NRMSD;
    NRMSD = dump_rmsd(golden, output, bsize);

    if (NRMSD <= 0.00000006)
        return 0;
    else
        return -1;
}
