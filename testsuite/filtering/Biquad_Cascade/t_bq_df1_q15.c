#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <inttypes.h>       //for PRId32
#include "riscv_dsp_filtering_math.h"
#include "riscv_dsp_test_accuracy.h"

#define NUMSTAGE 3
#define BLOCKSIZE 1023
#define POSTSHIFT 3

int main(void)
{
    uint8_t stage = NUMSTAGE;
    uint32_t bsize = BLOCKSIZE;
    int8_t shift = POSTSHIFT;
    q15_t *state = (q15_t *)calloc(4 * stage, sizeof(q15_t));
    q15_t *coe = (q15_t *)calloc(6 * stage, sizeof(q15_t));

#ifdef NDS_BQ_DF1_Q15_USE_SIMD
    //For DSP ext. configuration
    coe[0] = 0x200;
    coe[1] = 0;           //SHOULD be zero
    coe[2] = 0;
    coe[3] = 0x200;
    coe[4] = 0x200;
    coe[5] = -0x800;

    coe[6] = -0x800;
    coe[7] = 0;           //SHOULD be zero
    coe[8] = 0x200;
    coe[9] = 0;
    coe[10] = 0x200;
    coe[11] = 0x200;

    coe[12] = -0x800;
    coe[13] = 0;           //SHOULD be zero
    coe[14] = -0x800;
    coe[15] = 0x200;
    coe[16] = 0;
    coe[17] = 0x200;
#else
    //For normal configuration
    coe[0] = 0x200;

    coe[1] = 0;
    coe[2] = 0x200;
    coe[3] = 0x200;
    coe[4] = -0x800;

    coe[5] = -0x800;

    coe[6] = 0x200;
    coe[7] = 0;
    coe[8] = 0x200;
    coe[9] = 0x200;

    coe[10] = -0x800;
    coe[11] = -0x800;
    coe[12] = 0x200;
    coe[13] = 0;
    coe[14] = 0x200;
    // coe[15] = 0x200;
    // coe[16] = -0x800;
    // coe[17] = -0x800;
#endif

    riscv_dsp_bq_df1_q15_t Si = {stage, state, coe, shift};

    q15_t input[bsize] __attribute__ ((aligned(4)));
    q15_t output[bsize] __attribute__ ((aligned(4)));
    q15_t golden[bsize], golden_state[4 * stage];

    FILE *fptr;
    FILE *gptr;
    FILE *g2ptr;
    const char *srcdir = GETENV_SRCDIR;

    char path_buffer[512];
    sprintf (path_buffer, "%s/input_q15.bin", srcdir);
    fptr = fopen(path_buffer, "r");
    if (fptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/golden_biquad_cascade_df1_q15.bin", srcdir);
    gptr = fopen(path_buffer, "r");
    if (gptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/golden_biquad_cascade_df1_state_q15.bin", srcdir);
    g2ptr = fopen(path_buffer, "r");
    if (g2ptr == NULL)
        return -1;

    test_Fread(&input, sizeof(q15_t), bsize, fptr);
    dump_q15("input", input, bsize);
    test_Fread(&golden, sizeof(q15_t), bsize, gptr);
    test_Fread(&golden_state, sizeof(q15_t), 4 * stage, g2ptr);

    riscv_dsp_bq_df1_q15(&Si, input, output, bsize);
    dump2_q15(golden, output, bsize);
    q15_t maxdiff = dump2_q15(golden_state, state, 4 * stage);
    if (maxdiff > 0x0001) return -1;

    fclose(fptr);
    fclose(gptr);
    free(state);
    free(coe);

    float32_t NRMSD;
    float32_t golden_f32[bsize], output_f32[bsize];
    riscv_dsp_convert_q15_f32(golden, golden_f32, bsize);
    riscv_dsp_convert_q15_f32(output, output_f32, bsize);

    NRMSD = dump_rmsd(golden_f32, output_f32, bsize);

    if (NRMSD <= 0.0002)
        return 0;
    else
        return -1;
}
