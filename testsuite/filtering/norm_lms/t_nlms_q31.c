#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <inttypes.h>       //for PRId32
#include "riscv_dsp_filtering_math.h"
#include "riscv_dsp_test_accuracy.h"

#define BLOCKSIZE 63
#define NUMTAP 63
#define MU 0x6680000
#define POSTSHIFT 0

int main(void)
{
    uint32_t i, loop = 64;
    uint16_t numtap = NUMTAP;
    uint32_t bsize = BLOCKSIZE;
    q31_t *state = (q31_t *)calloc(bsize + numtap - 1, sizeof(q31_t));
    q31_t *coe = (q31_t *)calloc(numtap, sizeof(q31_t));
    q31_t u = MU;
    q31_t ene = 0x0, x0 = 0x0;
    uint8_t postshift = POSTSHIFT;
    riscv_dsp_nlms_q31_t Si = {numtap, state, coe, u, postshift, ene, x0};
    q31_t inq31[bsize] __attribute__ ((aligned(4)));
    q31_t req31[bsize] __attribute__ ((aligned(4)));
    float inf32[bsize] __attribute__ ((aligned(4)));
    float ref32[bsize] __attribute__ ((aligned(4)));
    q31_t outq31[bsize * loop] __attribute__ ((aligned(4)));
    q31_t errq31[bsize * loop] __attribute__ ((aligned(4)));
    float outf32[bsize * loop] __attribute__ ((aligned(4)));
    float errf32[bsize * loop] __attribute__ ((aligned(4)));
    q31_t * tmpout = outq31;
    q31_t * tmperr = errq31;
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
        riscv_dsp_convert_f32_q31(inf32, inq31, bsize);
        riscv_dsp_convert_f32_q31(ref32, req31, bsize);
        //dump_q31("inq31", inq31, bsize);
        //dump_q31("req31", req31, bsize);
        riscv_dsp_nlms_q31(&Si, inq31, req31, tmpout, tmperr, bsize);
        tmpout += bsize;
        tmperr += bsize;
    }
    riscv_dsp_convert_q31_f32(outq31, outf32, bsize * loop);
    riscv_dsp_convert_q31_f32(errq31, errf32, bsize * loop);

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
    if (maxdiff > 0.001)
    {
        printf("golden fail\n");
        return -1;
    }
    maxdiff = dump2_f32(g_err, errf32, bsize * loop);
    if (maxdiff > 0.001)
    {
        printf("golden error fail\n");
        return -1;
    }

    dump_rmsd_avg(golden, outf32, bsize, loop);

    return 0;
}
