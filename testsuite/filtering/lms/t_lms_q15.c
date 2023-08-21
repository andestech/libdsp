#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <inttypes.h>       //for PRId32
#include "riscv_dsp_filtering_math.h"
#include "riscv_dsp_test_accuracy.h"

#define NUMTAP 5
#define BLOCKSIZE 12
#define MU 0x400
#define POSTSHIFT 2

int main(void)
{
    uint32_t i, loop = 250;
    uint16_t numtap = NUMTAP;
    uint32_t bsize = BLOCKSIZE;
    uint32_t shift = POSTSHIFT;
    q15_t *state = (q15_t *)calloc(bsize + numtap - 1, sizeof(q15_t));
    q15_t *coe = (q15_t *)calloc(numtap, sizeof(q15_t));
    q15_t u = MU;
    riscv_dsp_lms_q15_t Si = {numtap, state, coe, u, shift};
    q15_t intput[bsize] __attribute__ ((aligned(4)));
    q15_t reint[bsize] __attribute__ ((aligned(4)));
    q15_t output[bsize * loop] __attribute__ ((aligned(4)));
    q15_t err[bsize] __attribute__ ((aligned(4)));
    q15_t golden[bsize * loop], g_err[bsize];
    q15_t * tmpout = output;

    FILE *fptr;
    FILE *f2ptr;
    FILE *gptr;
    FILE *g2ptr;
    const char *srcdir = GETENV_SRCDIR;

    char path_buffer[512];
    sprintf (path_buffer, "%s/input_q15.bin", srcdir);
    fptr = fopen(path_buffer, "r");
    if (fptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/input_ref_q15.bin", srcdir);
    f2ptr = fopen(path_buffer, "r");
    if (f2ptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/golden_lms_q15.bin", srcdir);
    gptr = fopen(path_buffer, "r");
    if (gptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/golden_err_q15.bin", srcdir);
    g2ptr = fopen(path_buffer, "r");
    if (g2ptr == NULL)
        return -1;


    test_Fread(&intput, sizeof(q15_t), bsize, fptr);
    test_Fread(&reint, sizeof(q15_t), bsize, f2ptr);

    for (i = 0; i < loop; i++)
    {
        test_Fread(&g_err, sizeof(q15_t), bsize, g2ptr);
        riscv_dsp_lms_q15(&Si, intput, reint, tmpout, err, bsize);
        tmpout += bsize;
    }
    test_Fread(&golden, sizeof(q15_t), bsize * loop, gptr);
    dump2_q15(golden, output, bsize * loop);

    fclose(fptr);
    fclose(f2ptr);
    fclose(gptr);
    fclose(g2ptr);
    free(state);
    free(coe);

    float32_t NRMSD;
    float32_t golden_f32[bsize * loop], output_f32[bsize * loop];
    riscv_dsp_convert_q15_f32(golden, golden_f32, bsize * loop);
    riscv_dsp_convert_q15_f32(output, output_f32, bsize * loop);

    NRMSD = dump_rmsd_avg(golden_f32, output_f32, bsize, loop);

    if (NRMSD <= 0.003)
        return 0;
    else
        return -1;
}
