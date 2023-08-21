#include <stdlib.h>
#include <stdio.h>
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
    q31_t *state = (q31_t *)calloc(phaselen + bsize - 1, sizeof(q31_t));
    q31_t *coe = (q31_t *)calloc(lnum * phaselen, sizeof(q31_t));
    riscv_dsp_upsplfir_q31_t Si = {lnum, phaselen, coe, state};

    q31_t input[bsize], output[bsize * lnum], golden[bsize * lnum], golden_state[phaselen + bsize - 1];

    FILE *fptr;
    FILE *gptr;
    FILE *g2ptr;
    const char *srcdir = GETENV_SRCDIR;

    char path_buffer[512];
    sprintf (path_buffer, "%s/input_q31.bin", srcdir);
    fptr = fopen(path_buffer, "r");
    if (fptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/golden_fir_interpolate_q31.bin", srcdir);
    gptr = fopen(path_buffer, "r");
    if (gptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/golden_fir_interpolate_state_q31.bin", srcdir);
    g2ptr = fopen(path_buffer, "r");
    if (g2ptr == NULL)
        return -1;

    for (i = 0; i < TAP; i++)
        coe[i] = 0x8000000;


    test_Fread(&input, sizeof(q31_t), bsize, fptr);
    test_Fread(&golden, sizeof(q31_t), bsize * lnum, gptr);
    test_Fread(&golden_state, sizeof(q31_t), phaselen + bsize - 1, g2ptr);
    riscv_dsp_upsplfir_q31(&Si, input, output, bsize);
    dump2_q31(golden, output, bsize * lnum);
    q31_t maxdiff = dump2_q31(golden_state, state, phaselen + bsize - 1);
    if (maxdiff > 0x00000000) return -1;

    free(state);
    free(coe);
    fclose(fptr);
    fclose(gptr);

    float32_t NRMSD;
    float32_t golden_f32[bsize * lnum], output_f32[bsize * lnum];
    riscv_dsp_convert_q31_f32(golden, golden_f32, bsize * lnum);
    riscv_dsp_convert_q31_f32(output, output_f32, bsize * lnum);

    NRMSD = dump_rmsd(golden_f32, output_f32, bsize * lnum);

    if (NRMSD <= 0.00000001)
        return 0;
    else
        return -1;
}
