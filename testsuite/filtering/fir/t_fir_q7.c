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
    q7_t *state = (q7_t *)calloc(bsize + numtap - 1, sizeof(q7_t));
    q7_t *coe = (q7_t *)calloc(numtap, sizeof(q7_t));
    riscv_dsp_fir_q7_t Si = {numtap, state, coe};
    q7_t input[bsize] __attribute__ ((aligned(4)));
    q7_t output[bsize] __attribute__ ((aligned(4)));
    q7_t golden[bsize];
    q7_t golden_state[bsize + numtap - 1];

    FILE *fptr;
    FILE *gptr;
    FILE *g2ptr;
    const char *srcdir = GETENV_SRCDIR;

    char path_buffer[512];
    sprintf (path_buffer, "%s/input_q7.bin", srcdir);
    fptr = fopen(path_buffer, "r");
    if (fptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/golden_fir_q7.bin", srcdir);
    gptr = fopen(path_buffer, "r");
    if (gptr == NULL)
        return -1;
    sprintf (path_buffer, "%s/golden_fir_state_q7.bin", srcdir);
    g2ptr = fopen(path_buffer, "r");
    if (g2ptr == NULL)
        return -1;

    for (i = 0; i < TAP ; i++)
    {
        coe[i] = 0x4;
    }
    test_Fread(&input, sizeof(q7_t), bsize, fptr);
    test_Fread(&golden, sizeof(q7_t), bsize, gptr);
    test_Fread(&golden_state, sizeof(q7_t), bsize + numtap - 1, g2ptr);
    riscv_dsp_fir_q7(&Si, input, output, bsize);
    q7_t maxdiff = dump2_q7(golden_state, state, bsize + numtap - 1);
    if (maxdiff > 0x01) return -1;
    dump2_q7(golden, output, bsize);

    free(state);
    free(coe);
    fclose(fptr);
    fclose(gptr);

    float32_t NRMSD;
    float32_t golden_f32[bsize], output_f32[bsize];
    riscv_dsp_convert_q7_f32(golden, golden_f32, bsize);
    riscv_dsp_convert_q7_f32(output, output_f32, bsize);

    NRMSD = dump_rmsd(golden_f32, output_f32, bsize);

    if (NRMSD <= 0.03)
        return 0;
    else
        return -1;
}
