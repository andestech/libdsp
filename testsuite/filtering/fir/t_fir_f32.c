#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <inttypes.h>       //for PRId32
#include "riscv_dsp_filtering_math.h"
#include "riscv_dsp_test_accuracy.h"

#define TAP 16
#define BLOCKSIZE 1024

int main(void)
{
    uint32_t i;
    uint16_t numtap = TAP;
    uint32_t bsize = BLOCKSIZE;
    float32_t *state = (float32_t *)calloc(bsize + numtap - 1, sizeof(float32_t));
    float32_t *coe = (float32_t *)calloc(numtap, sizeof(float32_t));
    riscv_dsp_fir_f32_t Si = {numtap, state, coe};
    float32_t input[bsize], output[bsize], golden[bsize], golden_state[bsize + numtap - 1];
    FILE *fptr;
    FILE *gptr;
    FILE *g2ptr;

    const char *srcdir = GETENV_SRCDIR;

    char path_buffer[512];
    sprintf (path_buffer, "%s/input_f32.bin", srcdir);
    fptr = fopen(path_buffer, "r");
    if (fptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/golden_fir_f32.bin", srcdir);
    gptr = fopen(path_buffer, "r");
    if (gptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/golden_fir_state_f32.bin", srcdir);
    g2ptr = fopen(path_buffer, "r");
    if (gptr == NULL)
        return -1;

    for (i = 0; i < TAP ; i++)
    {
        coe[i] = 0.4;
    }
    test_Fread(&input, sizeof(float32_t), bsize, fptr);
    test_Fread(&golden, sizeof(float32_t), bsize, gptr);
    test_Fread(&golden_state, sizeof(float32_t), bsize + numtap - 1, g2ptr);


    riscv_dsp_fir_f32(&Si, input, output, bsize);

    // check state
    for (i = 0; i < bsize + numtap - 1; i++)
    {
        if (state[i] != golden_state[i])
            return -1;
    }
    dump2_f32(golden, output, bsize);
    // dump2_f32(golden_state, state, bsize + numtap - 1);

    free(state);
    free(coe);
    fclose(fptr);
    fclose(gptr);

    float32_t NRMSD;
    NRMSD = dump_rmsd(golden, output, bsize);

    if (NRMSD <= 0.00000005)
        return 0;
    else
        return -1;
}
