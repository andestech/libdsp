#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <inttypes.h>       //for PRId32
#include "riscv_dsp_filtering_math.h"
#include "riscv_dsp_test_accuracy.h"

#define NUMSTAGE 3
#define BLOCKSIZE 1024
#define CHANNEL 2

int main(void)
{
    uint8_t stage = NUMSTAGE;
    uint32_t bsize = BLOCKSIZE;
    uint32_t channel = CHANNEL;
    uint32_t totalsize = bsize * channel;
    uint32_t statesize =  4 * stage;
    float32_t *state = (float32_t *)calloc(statesize, sizeof(float32_t));
    float32_t *coe = (float32_t *)calloc(5 * stage, sizeof(float32_t));
    coe[0] = 0.1;
    coe[4] = -0.1;
    coe[8] = -0.1;
    coe[12] = 0.1;
    coe[1] = 0.2;
    coe[5] = 0.1;
    coe[9] = -0.1;
    coe[13] = -0.1;
    coe[2] = 0.3;
    coe[6] = 0.1;
    coe[10] = 0.1;
    coe[14] = -0.1;
    coe[3] = -0.4;
    coe[7] = 0.1;
    coe[11] = 0.1;

    riscv_dsp_bq_stereo_df2T_f32_t Si = {stage, state, coe};

    float32_t input[totalsize], output[totalsize], golden[totalsize], golden_state[statesize];

    FILE *fptr;
    FILE *gptr;
    FILE *g2ptr;
    const char *srcdir = GETENV_SRCDIR;

    char path_buffer[512];
    sprintf (path_buffer, "%s/input_stereo_f32.bin", srcdir);
    fptr = fopen(path_buffer, "r");
    if (fptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/golden_biquad_cascade_stereo_df2T_f32.bin", srcdir);
    gptr = fopen(path_buffer, "r");
    if (gptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/golden_biquad_cascade_stereo_df2T_state_f32.bin", srcdir);
    g2ptr = fopen(path_buffer, "r");
    if (g2ptr == NULL)
        return -1;

    test_Fread(&input, sizeof(float32_t), totalsize, fptr);
    dump_f32("input", input, totalsize);
    test_Fread(&golden, sizeof(float32_t), totalsize, gptr);
    test_Fread(&golden_state, sizeof(float32_t), statesize, g2ptr);

    riscv_dsp_bq_stereo_df2T_f32(&Si, input, output, bsize);
    dump2_f32(golden, output, totalsize);
    float32_t maxdiff = dump2_f32(golden_state, state, statesize);
    if (maxdiff > 0.00000005) return -1;

    fclose(fptr);
    fclose(gptr);
    free(state);
    free(coe);

    float32_t NRMSD;
    NRMSD = dump_rmsd(golden, output, totalsize);

    if (NRMSD <= 0.00000008)
        return 0;
    else
        return -1;
}
