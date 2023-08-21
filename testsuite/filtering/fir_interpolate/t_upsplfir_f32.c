#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <inttypes.h>       //for PRId32
#include "riscv_dsp_filtering_math.h"
#include "riscv_dsp_test_accuracy.h"

#define TAP 16
#define BLOCKSIZE 1024
#define L 4
#define PLEN (TAP/L)

int main(void)
{
    uint32_t i;
    uint8_t lnum = L;
    uint16_t phaselen = PLEN;
    uint32_t bsize = BLOCKSIZE;
    float32_t *state = (float32_t *)calloc(phaselen + bsize - 1, sizeof(float32_t));
    float32_t *coe = (float32_t *)calloc(lnum * phaselen, sizeof(float32_t));
    riscv_dsp_upsplfir_f32_t Si = {lnum, phaselen, coe, state};
    float32_t input[bsize], output[bsize * lnum], golden[bsize * lnum], golden_state[phaselen + bsize - 1];

    FILE *fptr;
    FILE *gptr;
    FILE *g2ptr;
    const char *srcdir = GETENV_SRCDIR;

    char path_buffer[512];
    sprintf (path_buffer, "%s/input_f32.bin", srcdir);
    fptr = fopen(path_buffer, "r");
    if (fptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/golden_fir_interpolate_f32.bin", srcdir);
    gptr = fopen(path_buffer, "r");
    if (gptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/golden_fir_interpolate_state_f32.bin", srcdir);
    g2ptr = fopen(path_buffer, "r");
    if (g2ptr == NULL)
        return -1;

    for (i = 0; i < TAP; i++)
        coe[i] = 1;

    test_Fread(&input, sizeof(float32_t), bsize, fptr);
    test_Fread(&golden, sizeof(float32_t), bsize * lnum, gptr);
    test_Fread(&golden_state, sizeof(float32_t), phaselen + bsize - 1, g2ptr);
    riscv_dsp_upsplfir_f32(&Si, input, output, bsize);
    dump2_f32(golden, output, bsize * lnum);
    float32_t maxdiff = dump2_f32(golden_state, state, phaselen + bsize - 1);
    if (maxdiff > 0.00000005) return -1;
    free(state);
    free(coe);
    fclose(fptr);
    fclose(gptr);

    float32_t NRMSD;
    NRMSD = dump_rmsd(golden, output, bsize * lnum);

    if (NRMSD <= 0.00000005)
        return 0;
    else
        return -1;
}
