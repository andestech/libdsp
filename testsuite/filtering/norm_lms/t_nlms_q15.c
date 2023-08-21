#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <inttypes.h>       //for PRId32
#include "riscv_dsp_filtering_math.h"
#include "riscv_dsp_test_accuracy.h"

#define BLOCKSIZE 63
#define NUMTAP 63
#define MU 0x668  // 1640
#define POSTSHIFT -2

int main(void)
{
    uint32_t i, loop = 200;
    uint16_t numtap = NUMTAP;
    uint32_t bsize = BLOCKSIZE;
    uint32_t shift = POSTSHIFT;
    q15_t *state = (q15_t *)calloc(bsize + numtap - 1, sizeof(q15_t));
    q15_t *coe = (q15_t *)calloc(numtap, sizeof(q15_t));
    q15_t u = MU, ene = 0, x0 = 0;
    riscv_dsp_nlms_q15_t Si = {numtap, state, coe, u, shift, ene, x0};

    q15_t inq15[bsize] __attribute__ ((aligned(4)));
    q15_t req15[bsize] __attribute__ ((aligned(4)));
    float inf32[bsize] __attribute__ ((aligned(4)));
    float ref32[bsize] __attribute__ ((aligned(4)));
    q15_t outq15[bsize * loop] __attribute__ ((aligned(4)));
    q15_t errq15[bsize * loop] __attribute__ ((aligned(4)));
    float outf32[bsize * loop] __attribute__ ((aligned(4)));
    float errf32[bsize * loop] __attribute__ ((aligned(4)));
    q15_t * tmpout = outq15;
    q15_t * tmperr = errq15;
    float32_t golden[bsize * loop], g_err[bsize * loop];

    FILE *fptr;
    FILE *f2ptr;
    FILE *gptr;
    FILE *g2ptr;
    const char *srcdir = GETENV_SRCDIR;

    char path_buffer[512];
    sprintf (path_buffer, "%s/input_f32.bin", srcdir);
    fptr = fopen(path_buffer, "r");
    if (fptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/input_ref_f32.bin", srcdir);
    f2ptr = fopen(path_buffer, "r");
    if (f2ptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/golden_lms_norm_f32.bin", srcdir);
    gptr = fopen(path_buffer, "r");
    if (gptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/golden_err_norm_f32.bin", srcdir);
    g2ptr = fopen(path_buffer, "r");
    if (g2ptr == NULL)
        return -1;

    for (i = 0; i < loop; i++)
    {
        test_Fread(&inf32, sizeof(float32_t), bsize, fptr);
        test_Fread(&ref32, sizeof(float32_t), bsize, f2ptr);
        riscv_dsp_convert_f32_q15(inf32, inq15, bsize);
        riscv_dsp_convert_f32_q15(ref32, req15, bsize);
        //dump_q15("inq15", inq15, bsize);
        //dump_q15("req15", req15, bsize);
        riscv_dsp_nlms_q15(&Si, inq15, req15, tmpout, tmperr, bsize);

        tmpout += bsize;
        tmperr += bsize;
    }
    riscv_dsp_convert_q15_f32(outq15, outf32, bsize * loop);
    riscv_dsp_convert_q15_f32(errq15, errf32, bsize * loop);

    test_Fread(&golden, sizeof(float32_t), bsize * loop, gptr);
    test_Fread(&g_err, sizeof(float32_t), bsize * loop, g2ptr);


    fclose(fptr);
    fclose(f2ptr);
    fclose(gptr);
    fclose(g2ptr);
    free(state);
    free(coe);

    float maxdiff;

    maxdiff = dump2_f32(golden, outf32, bsize * loop);
    if (maxdiff > 0.2)
    {
        printf("golden fail\n");
        return -1;
    }
    maxdiff = dump2_f32(g_err, errf32, bsize * loop);
    if (maxdiff > 0.2)
    {
        printf("golden error fail\n");
        return -1;
    }

    dump_rmsd_avg(golden, outf32, bsize, loop);

    return 0;
}
