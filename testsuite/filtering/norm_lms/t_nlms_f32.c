#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <inttypes.h>       //for PRId32
#include "riscv_dsp_filtering_math.h"
#include "riscv_dsp_test_accuracy.h"

#define BLOCKSIZE 63
#define NUMTAP 63
#define MU 0.0500488281250000

int main(void)
{
    uint32_t i, loop = 200;
    uint16_t numtap = NUMTAP;
    uint32_t bsize = BLOCKSIZE;
    float32_t *state = (float32_t *)calloc(bsize + numtap - 1, sizeof(float32_t));
    float32_t *coe = (float32_t *)calloc(numtap, sizeof(float32_t));
    float32_t u = MU;
    float32_t ene = 0.0f;
    riscv_dsp_nlms_f32_t Si = {numtap, state, coe, u, ene};
    float32_t intput[bsize], reint[bsize], output[bsize * loop], err[bsize * loop];
    float32_t golden[bsize * loop], g_err[bsize * loop];
    float32_t * tmpout = output;
    float32_t * tmperr = err;

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
        test_Fread(&intput, sizeof(float32_t), bsize, fptr);
        test_Fread(&reint, sizeof(float32_t), bsize, f2ptr);
        riscv_dsp_nlms_f32(&Si, intput, reint, tmpout, tmperr, bsize);
        tmpout += bsize;
        tmperr += bsize;
    }
    test_Fread(&golden, sizeof(float32_t), bsize * loop, gptr);
    test_Fread(&g_err, sizeof(float32_t), bsize * loop, g2ptr);

    fclose(fptr);
    fclose(f2ptr);
    fclose(gptr);
    fclose(g2ptr);
    free(state);
    free(coe);

    float maxdiff;

    maxdiff = dump2_f32(golden, output, bsize * loop);
    if (maxdiff > 0.2)
    {
        printf("golden fail\n");
        return -1;
    }
    maxdiff = dump2_f32(g_err, err, bsize * loop);
    if (maxdiff > 0.2)
    {
        printf("golden error fail\n");
        return -1;
    }

    float32_t NRMSD;
    NRMSD = dump_rmsd_avg(golden, output, bsize, loop);

    if (NRMSD <= 0.0000001)
        return 0;
    else
        return -1;
}
