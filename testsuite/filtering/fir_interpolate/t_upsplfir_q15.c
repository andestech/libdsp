#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <inttypes.h>       //for PRId32
#include "riscv_dsp_filtering_math.h"
#include "riscv_dsp_test_accuracy.h"

#define TAP 15
#define BLOCKSIZE 1024
#define L 3
#define PLEN (TAP/L)

int main(void)
{
    uint32_t i;
    uint8_t lnum = L;
    uint16_t phaselen = PLEN;
    uint32_t bsize = BLOCKSIZE;
    q15_t *state = (q15_t *)calloc(phaselen + bsize - 1, sizeof(q15_t));
    q15_t *coe = (q15_t *)calloc(lnum * phaselen, sizeof(q15_t));
    riscv_dsp_upsplfir_q15_t Si = {lnum, phaselen, coe, state};

    q15_t input[bsize] __attribute__ ((aligned(4)));
    q15_t output[bsize * lnum] __attribute__ ((aligned(4)));
    q15_t golden[bsize * lnum];
    q15_t golden_state[phaselen + bsize - 1];

    FILE *fptr;
    FILE *gptr;
    FILE *g2ptr;
    const char *srcdir = GETENV_SRCDIR;

    char path_buffer[512];
    sprintf (path_buffer, "%s/input_q15.bin", srcdir);
    fptr = fopen(path_buffer, "r");
    if (fptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/golden_fir_interpolate_q15.bin", srcdir);
    gptr = fopen(path_buffer, "r");
    if (gptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/golden_fir_interpolate_state_q15.bin", srcdir);
    g2ptr = fopen(path_buffer, "r");
    if (g2ptr == NULL)
        return -1;

    for (i = 0; i < TAP; i++)
        coe[i] = 0x800;

    test_Fread(&input, sizeof(q15_t), bsize, fptr);
    test_Fread(&golden, sizeof(q15_t), bsize * lnum, gptr);
    test_Fread(&golden_state, sizeof(q15_t), phaselen + bsize - 1, g2ptr);
    riscv_dsp_upsplfir_q15(&Si, input, output, bsize);
    dump2_q15(golden, output, bsize * lnum);
    q15_t maxdiff = dump2_q15(golden_state, state, phaselen + bsize - 1);
    if (maxdiff > 0x0000) return -1;

    free(state);
    free(coe);
    fclose(fptr);
    fclose(gptr);

    float32_t NRMSD;
    float32_t golden_f32[bsize * lnum], output_f32[bsize * lnum];
    riscv_dsp_convert_q15_f32(golden, golden_f32, bsize * lnum);
    riscv_dsp_convert_q15_f32(output, output_f32, bsize * lnum);

    NRMSD = dump_rmsd(golden_f32, output_f32, bsize * lnum);

    if (NRMSD <= 0.00008)
        return 0;
    else
        return -1;
}
