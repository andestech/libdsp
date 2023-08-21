#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <inttypes.h>       //for PRId32
#include "riscv_dsp_filtering_math.h"
#include "riscv_dsp_test_accuracy.h"

#define TAP 15
#define BLOCKSIZE 1024
#define M 2

int main(void)
{
    uint32_t i;
    uint16_t numtap = TAP;
    uint32_t bsize = BLOCKSIZE;
    uint8_t m = M;
    uint32_t outsize = BLOCKSIZE / M;
    float32_t state [BLOCKSIZE + TAP - 1] = {0};
    float32_t coe[TAP] = {0};
    riscv_dsp_dcmfir_f32_t Si = {m, numtap, coe, state};
    float32_t input[bsize], output[outsize], golden[outsize], golden_state[BLOCKSIZE + TAP - 1];

    FILE *fptr;
    FILE *gptr;
    FILE *g2ptr;
    const char *srcdir = GETENV_SRCDIR;

    char path_buffer[512];
    sprintf (path_buffer, "%s/input_f32.bin", srcdir);
    fptr = fopen(path_buffer, "r");
    if (fptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/golden_fir_decimate_f32.bin", srcdir);
    gptr = fopen(path_buffer, "r");
    if (gptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/golden_fir_decimate_state_f32.bin", srcdir);
    g2ptr = fopen(path_buffer, "r");
    if (g2ptr == NULL)
        return -1;

    for (i = 0; i < TAP ; i++)
    {
        if (i < 4)
            coe[i] = 0.1;
        else if (i < 8)
            coe[i] = 0.2;
        else if (i < 12)
            coe[i] = 0.3;
        else
            coe[i] = 0.4;
    }
    test_Fread(&input, sizeof(float32_t), bsize, fptr);
    test_Fread(&golden, sizeof(float32_t), outsize, gptr);
    test_Fread(&golden_state, sizeof(float32_t), BLOCKSIZE + TAP - 1, g2ptr);
    riscv_dsp_dcmfir_f32(&Si, input, output, bsize);
    dump2_f32(golden, output, outsize);
    float32_t maxdiff = dump2_f32(golden_state, state, BLOCKSIZE + TAP - 1);
    if (maxdiff != 0) return -1;

    fclose(fptr);
    fclose(gptr);

    float32_t NRMSD;
    NRMSD = dump_rmsd(golden, output, outsize);

    if (NRMSD <= 0.00000005)
        return 0;
    else
        return -1;
}
