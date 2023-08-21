#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <inttypes.h>       //for PRId32
#include "riscv_dsp_filtering_math.h"
#include "riscv_dsp_test_accuracy.h"

#define NUMTAP 16
#define BLOCKSIZE 16
#define MU 0.5

int main(void)
{
    uint32_t i, loop = 625;
    uint16_t numtap = NUMTAP;
    uint32_t bsize = BLOCKSIZE;
    float32_t *state = (float32_t *)calloc(bsize + numtap - 1, sizeof(float32_t));
    float32_t *coe = (float32_t *)calloc(numtap, sizeof(float32_t));
    float32_t u = MU;
    riscv_dsp_lms_f32_t Si = {numtap, state, coe, u};
    float32_t intput[bsize], reint[bsize], output[bsize * loop], err[bsize];
    float32_t golden[bsize * loop], g_err[bsize];
    float32_t * tmpout = output;

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

    sprintf (path_buffer, "%s/golden_lms_f32.bin", srcdir);
    gptr = fopen(path_buffer, "r");
    if (gptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/golden_err_f32.bin", srcdir);
    g2ptr = fopen(path_buffer, "r");
    if (g2ptr == NULL)
        return -1;

    test_Fread(&intput, sizeof(float32_t), bsize, fptr);
    test_Fread(&reint, sizeof(float32_t), bsize, f2ptr);

    for (i = 0; i < loop; i++)
    {
        test_Fread(&g_err, sizeof(float32_t), bsize, g2ptr);
        riscv_dsp_lms_f32(&Si, intput, reint, tmpout, err, bsize);
        tmpout += bsize;
    }
    test_Fread(&golden, sizeof(float32_t), bsize * loop, gptr);
    dump2_f32(golden, output, bsize * loop);

    fclose(fptr);
    fclose(f2ptr);
    fclose(gptr);
    fclose(g2ptr);
    free(state);
    free(coe);

    float32_t NRMSD;
    NRMSD = dump_rmsd_avg(golden, output, bsize, loop);

    if (NRMSD <= 0.0000008)
        return 0;
    else
        return -1;
}
