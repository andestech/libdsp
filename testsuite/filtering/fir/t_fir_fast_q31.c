#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <inttypes.h>       //for PRId32
#include "riscv_dsp_filtering_math.h"
#include "riscv_dsp_test_accuracy.h"

#define TAP 15
#define BLOCKSIZE 1023

int main(void)
{
    uint32_t i;
    uint16_t numtap = TAP;
    uint32_t bsize = BLOCKSIZE;
    q31_t *state = (q31_t *)calloc(bsize + numtap - 1, sizeof(q31_t));
    q31_t *coe = (q31_t *)calloc(numtap, sizeof(q31_t));
    riscv_dsp_fir_q31_t Si = {numtap, state, coe};
    q31_t input[bsize], output[bsize], golden[bsize], golden_state[bsize + numtap - 1];

    FILE *fptr;
    FILE *gptr;
    FILE *g2ptr;
    const char *srcdir = GETENV_SRCDIR;

    char path_buffer[512];
    sprintf (path_buffer, "%s/input_q31.bin", srcdir);
    fptr = fopen(path_buffer, "r");
    if (fptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/golden_fir_q31.bin", srcdir);
    gptr = fopen(path_buffer, "r");
    if (gptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/golden_fir_state_q31.bin", srcdir);
    g2ptr = fopen(path_buffer, "r");
    if (g2ptr == NULL)
        return -1;

    for (i = 0; i < TAP ; i++)
    {
        if (i < 4)
            coe[i] = 0x4000000;
        else if (i < 8)
            coe[i] = 0x8000000;
        else if (i < 12)
            coe[i] = 0x2000000;
        else
            coe[i] = 0x20000000;
    }

    test_Fread(&input, sizeof(q31_t), bsize, fptr);
    test_Fread(&golden, sizeof(q31_t), bsize, gptr);
    test_Fread(&golden_state, sizeof(q31_t), bsize + numtap - 1, g2ptr);
    riscv_dsp_fir_fast_q31(&Si, input, output, bsize);

    // check state
    for (i = 0; i < bsize + numtap - 1; i++)
    {
        //if (state[i] != golden_state[i])
        //return -1;
    }
    dump2_q31(golden, output, bsize);

    free(state);
    free(coe);
    fclose(fptr);
    fclose(gptr);

    float32_t NRMSD;
    float32_t golden_f32[bsize], output_f32[bsize];
    riscv_dsp_convert_q31_f32(golden, golden_f32, bsize);
    riscv_dsp_convert_q31_f32(output, output_f32, bsize);

    NRMSD = dump_rmsd(golden_f32, output_f32, bsize);

    if (NRMSD <= 0.00000001)
        return 0;
    else
        return -1;
}
