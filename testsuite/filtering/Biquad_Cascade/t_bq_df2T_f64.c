#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <inttypes.h>       //for PRId32
#include "riscv_dsp_filtering_math.h"
#include "riscv_dsp_test_accuracy.h"

#define NUMSTAGE 3
#define BLOCKSIZE 1023

int main(void)
{
    uint8_t stage = NUMSTAGE;
    uint32_t bsize = BLOCKSIZE;
    float64_t *state = (float64_t *)calloc(2 * stage, sizeof(float64_t));
    float64_t *coe = (float64_t *)calloc(5 * stage, sizeof(float64_t));
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

    riscv_dsp_bq_df2T_f64_t Si = {stage, state, coe};

    float64_t input[bsize], output[bsize], golden[bsize], golden_state[2 * stage];

    FILE *fptr;
    FILE *gptr;
    FILE *g2ptr;
    const char *srcdir = GETENV_SRCDIR;

    char path_buffer[512];
    sprintf (path_buffer, "%s/input_f64.bin", srcdir);
    fptr = fopen(path_buffer, "r");
    if (fptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/golden_biquad_cascade_df2T_f64.bin", srcdir);
    gptr = fopen(path_buffer, "r");
    if (gptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/golden_biquad_cascade_df2T_state_f64.bin", srcdir);
    g2ptr = fopen(path_buffer, "r");
    if (g2ptr == NULL)
        return -1;

    test_Fread(&input, sizeof(float64_t), bsize, fptr);
    dump_f64("input", input, bsize);
    test_Fread(&golden, sizeof(float64_t), bsize, gptr);
    test_Fread(&golden_state, sizeof(float64_t), 2 * stage, g2ptr);

    riscv_dsp_bq_df2T_f64(&Si, input, output, bsize);
    dump2_f64(golden, output, bsize);
    float64_t maxdiff = dump2_f64(golden_state, state, 2 * stage);
    if (maxdiff > 0.00000005) return -1;

    fclose(fptr);
    fclose(gptr);
    free(state);
    free(coe);

    float64_t NRMSD;
    NRMSD = dump_rmsd64(golden, output, bsize);

    if (NRMSD <= 0.00000008)
        return 0;
    else
        return -1;
}
