#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <inttypes.h>       //for PRId32
#include "riscv_dsp_filtering_math.h"
#include "riscv_dsp_test_accuracy.h"

#define TAP 15
#define BLOCKSIZE 1022
#define M 2

int main(void)
{
    uint32_t i;
    uint16_t numtap = TAP;
    uint32_t bsize = BLOCKSIZE;
    uint8_t m = M;
    uint32_t outsize = BLOCKSIZE / M;
    q15_t state [BLOCKSIZE + TAP - 1] = {0};
    q15_t coe[TAP] = {0};
    riscv_dsp_dcmfir_q15_t Si = {m, numtap, coe, state};

    q15_t input[bsize], output[outsize], golden[outsize], golden_state[BLOCKSIZE + TAP - 1];

    FILE *fptr;
    FILE *gptr;
    FILE *g2ptr;
    const char *srcdir = GETENV_SRCDIR;

    char path_buffer[512];
    sprintf (path_buffer, "%s/input_q15.bin", srcdir);
    fptr = fopen(path_buffer, "r");
    if (fptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/golden_fir_decimate_q15.bin", srcdir);
    gptr = fopen(path_buffer, "r");
    if (gptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/golden_fir_decimate_state_q15.bin", srcdir);
    g2ptr = fopen(path_buffer, "r");
    if (g2ptr == NULL)
        return -1;

    for (i = 0; i < TAP ; i++)
    {
        coe[i] = 0x2000;
    }
    test_Fread(&input, sizeof(q15_t), bsize, fptr);
    test_Fread(&golden, sizeof(q15_t), outsize, gptr);
    test_Fread(&golden_state, sizeof(q15_t), BLOCKSIZE + TAP - 1, g2ptr);
    riscv_dsp_dcmfir_fast_q15(&Si, input, output, bsize);
    dump2_q15(golden, output, outsize);
    q15_t maxdiff = dump2_q15(golden_state, state, BLOCKSIZE + TAP - 1);
    if (maxdiff != 0) return -1;

    fclose(fptr);
    fclose(gptr);

    float32_t NRMSD;
    float32_t golden_f32[outsize], output_f32[outsize];
    riscv_dsp_convert_q15_f32(golden, golden_f32, outsize);
    riscv_dsp_convert_q15_f32(output, output_f32, outsize);

    NRMSD = dump_rmsd(golden_f32, output_f32, outsize);

    if (NRMSD <= 0.00005)
        return 0;
    else
        return -1;
}
