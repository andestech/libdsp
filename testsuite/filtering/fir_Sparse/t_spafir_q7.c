#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>       //for PRId32
#include "riscv_dsp_test_accuracy.h"
#include "riscv_dsp_filtering_math.h"

#define TAP 17
#define BLOCKSIZE 1023
#define MAXDELAY 3

int main(void)
{
    uint32_t i;
    uint16_t numtap = TAP;
    uint16_t maxdly = MAXDELAY;
    uint32_t bsize = BLOCKSIZE;
    int32_t *tapdly = (int32_t *)calloc(numtap, sizeof(int32_t));
    q7_t *state = (q7_t *)calloc(bsize + maxdly, sizeof(q7_t));
    q7_t *coe = (q7_t *)calloc(numtap, sizeof(q7_t));
    riscv_dsp_spafir_q7_t Si = {numtap, 0, state, coe, maxdly, tapdly};

    q7_t input[bsize] __attribute__ ((aligned(4)));
    q7_t output[bsize] __attribute__ ((aligned(4)));
    q7_t scratchin[bsize] __attribute__ ((aligned(4)));
    q7_t golden[bsize];
    q31_t scratchout[bsize];

    FILE *fptr;
    FILE *gptr;
    const char *srcdir = GETENV_SRCDIR;

    char path_buffer[512];
    sprintf (path_buffer, "%s/input_q7.bin", srcdir);
    fptr = fopen(path_buffer, "r");
    if (fptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/golden_fir_sparse_q7.bin", srcdir);
    gptr = fopen(path_buffer, "r");
    if (gptr == NULL)
        return -1;

    for (i = 0; i < TAP ; i++)
    {
        if (i < 4)
        {
            coe[i] = 0x4;
            tapdly[i] = 3;
        }
        else if (i < 8)
        {
            coe[i] = 0x8;
            tapdly[i] = 2;
        }
        else if (i < 12)
        {
            coe[i] = 0x2;
            tapdly[i] = 0;
        }
        else
        {
            coe[i] = 0x20;
            tapdly[i] = 1;
        }
    }

    test_Fread(&input, sizeof(q7_t), bsize, fptr);
    test_Fread(&golden, sizeof(q7_t), bsize, gptr);
    riscv_dsp_spafir_q7(&Si, input, output, scratchin, scratchout, bsize);
    dump2_q7(golden, output, bsize);

    free(state);
    free(coe);
    free(tapdly);
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
