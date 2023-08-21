#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <inttypes.h>       //for PRId32
#include "riscv_dsp_filtering_math.h"
#include "riscv_dsp_test_accuracy.h"

#define NUMSTAGE 3
#define BLOCKSIZE 1023
#define POSTSHIFT 4

int main(void)
{
    uint8_t stage = NUMSTAGE;
    uint32_t bsize = BLOCKSIZE;
    int8_t shift = POSTSHIFT;
    q63_t *state = (q63_t *)calloc(4 * stage, sizeof(q63_t));
    q31_t *coe = (q31_t *)calloc(5 * stage, sizeof(q31_t));
    coe[0] = 0x2000000;
    coe[6] = 0x2000000;
    coe[12] = 0x2000000;
    coe[1] = 0x2000000;
    coe[7] = 0x2000000;
    coe[13] = -0x8000000;
    coe[2] = 0x2000000;
    coe[8] = -0x2000000;
    coe[14] = -0x8000000;
    coe[3] = -0x2000000;
    coe[9] = -0x2000000;
    coe[4] = -0x8000000;
    coe[10] = -0x8000000;
    coe[5] = -0x8000000;
    coe[11] = 0x8000000;

    riscv_dsp_bq_df1_32x64_q31_t Si = {stage, state, coe, shift};

    q31_t input[bsize], output[bsize], golden[bsize];

    FILE *fptr;
    FILE *gptr;
    const char *srcdir = GETENV_SRCDIR;

    char path_buffer[512];
    sprintf (path_buffer, "%s/input_q31.bin", srcdir);
    fptr = fopen(path_buffer, "r");
    if (fptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/golden_biquad_cascade_df1_q31.bin", srcdir);
    gptr = fopen(path_buffer, "r");
    if (gptr == NULL)
        return -1;

    test_Fread(&input, sizeof(q31_t), bsize, fptr);
    dump_q31("input", input, bsize);
    test_Fread(&golden, sizeof(q31_t), bsize, gptr);

    riscv_dsp_bq_df1_32x64_q31(&Si, input, output, bsize);
    dump2_q31(golden, output, bsize);

    fclose(fptr);
    fclose(gptr);
    free(state);
    free(coe);

    float32_t NRMSD;
    float32_t golden_f32[bsize], output_f32[bsize];
    riscv_dsp_convert_q31_f32(golden, golden_f32, bsize);
    riscv_dsp_convert_q31_f32(output, output_f32, bsize);

    NRMSD = dump_rmsd(golden_f32, output_f32, bsize);

    if (NRMSD <= 0.00000003)
        return 0;
    else
        return -1;
}
