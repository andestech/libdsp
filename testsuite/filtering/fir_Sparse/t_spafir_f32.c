#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <inttypes.h>       //for PRId32
#include "riscv_dsp_filtering_math.h"
#include "riscv_dsp_test_accuracy.h"

#define TAP 16
#define BLOCKSIZE 1024
#define MAXDELAY 3

int main(void)
{
    uint32_t i;
    uint16_t numtap = TAP;
    uint16_t maxdly = MAXDELAY;
    uint32_t bsize = BLOCKSIZE;
    int32_t *tapdly = (int32_t *)calloc(numtap, sizeof(int32_t));
    float32_t *state = (float32_t *)calloc(bsize + maxdly, sizeof(float32_t));
    float32_t *coe = (float32_t *)calloc(numtap, sizeof(float32_t));
    riscv_dsp_spafir_f32_t Si = {numtap, 0, state, coe, maxdly, tapdly};
    float32_t input[bsize], output[bsize], golden[bsize], scratch[bsize];
    FILE *fptr;
    FILE *gptr;
    const char *srcdir = GETENV_SRCDIR;
    char path_buffer[512];
    sprintf (path_buffer, "%s/input_f32.bin", srcdir);
    fptr = fopen(path_buffer, "r");
    if (fptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/golden_fir_sparse_f32.bin", srcdir);
    gptr = fopen(path_buffer, "r");
    if (gptr == NULL)
        return -1;
    for (i = 0; i < TAP ; i++)
    {
        if (i < 4)
        {
            coe[i] = 0.1;
            tapdly[i] = 3;
        }
        else if (i < 8)
        {
            coe[i] = 0.2;
            tapdly[i] = 2;
        }
        else if (i < 12)
        {
            coe[i] = 0.3;
            tapdly[i] = 0;
        }
        else
        {
            coe[i] = 0.4;
            tapdly[i] = 1;
        }
    }

    test_Fread(&input, sizeof(float32_t), bsize, fptr);
    test_Fread(&golden, sizeof(float32_t), bsize, gptr);
    riscv_dsp_spafir_f32(&Si, input, output, scratch, bsize);
    dump2_f32(golden, output, bsize);

    free(state);
    free(coe);
    free(tapdly);
    fclose(fptr);
    fclose(gptr);

    float32_t NRMSD;
    NRMSD = dump_rmsd(golden, output, bsize);

    if (NRMSD <= 0.00000005)
        return 0;
    else
        return -1;
}
