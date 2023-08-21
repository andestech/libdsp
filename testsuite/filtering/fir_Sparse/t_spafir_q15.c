#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <inttypes.h>       //for PRId32
#include "riscv_dsp_filtering_math.h"
#include "riscv_dsp_test_accuracy.h"

#define TAP 16
#define BLOCKSIZE 1023
#define MAXDELAY 3

int main(void)
{
    uint32_t i;
    uint16_t numtap = TAP;
    uint16_t maxdly = MAXDELAY;
    uint32_t bsize = BLOCKSIZE;
    int32_t *tapdly = (int32_t *)calloc(numtap, sizeof(int32_t));
    q15_t *state = (q15_t *)calloc(bsize + maxdly, sizeof(q15_t));
    q15_t *coe = (q15_t *)calloc(numtap, sizeof(q15_t));
    riscv_dsp_spafir_q15_t Si = {numtap, 0, state, coe, maxdly, tapdly};

    q15_t input[bsize] __attribute__ ((aligned(4)));
    q15_t output[bsize] __attribute__ ((aligned(4)));
    q15_t scratchin[bsize] __attribute__ ((aligned(4)));
    q15_t golden[bsize];
    q31_t scratchout[bsize];

    FILE *fptr;
    FILE *gptr;
    const char *srcdir = GETENV_SRCDIR;

    char path_buffer[512];
    sprintf (path_buffer, "%s/input_q15.bin", srcdir);
    fptr = fopen(path_buffer, "r");
    if (fptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/golden_fir_sparse_q15.bin", srcdir);
    gptr = fopen(path_buffer, "r");
    if (gptr == NULL)
        return -1;

    for (i = 0; i < TAP ; i++)
    {
        if (i < 4)
        {
            coe[i] = 0x400;
            tapdly[i] = 3;
        }
        else if (i < 8)
        {
            coe[i] = 0x800;
            tapdly[i] = 2;
        }
        else if (i < 12)
        {
            coe[i] = 0x200;
            tapdly[i] = 0;
        }
        else
        {
            coe[i] = 0x2000;
            tapdly[i] = 1;
        }
    }

    test_Fread(&input, sizeof(q15_t), bsize, fptr);
    test_Fread(&golden, sizeof(q15_t), bsize, gptr);
    riscv_dsp_spafir_q15(&Si, input, output, scratchin, scratchout, bsize);
    dump2_q15(golden, output, bsize);


    free(state);
    free(coe);
    free(tapdly);
    fclose(fptr);
    fclose(gptr);

    float32_t NRMSD;
    float32_t golden_f32[bsize], output_f32[bsize];
    riscv_dsp_convert_q15_f32(golden, golden_f32, bsize);
    riscv_dsp_convert_q15_f32(output, output_f32, bsize);

    NRMSD = dump_rmsd(golden_f32, output_f32, bsize);

    if (NRMSD <= 0.00004)
        return 0;
    else
        return -1;
}
