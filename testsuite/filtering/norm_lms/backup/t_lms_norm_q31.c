#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <inttypes.h>       //for PRId32
#include "nds32_filtering_math.h"
#include "nds32_test_accuracy.h"

#define NUMTAP 5
#define BLOCKSIZE 4
#define MU 0x500

int main(void)
{
    uint32_t i, loop = 110;
    uint16_t numtap = NUMTAP;
    uint32_t bsize = BLOCKSIZE;
    q31_t *state = calloc(bsize + numtap - 1, sizeof(q31_t));
    q31_t *coe = calloc(numtap, sizeof(q31_t));
    q31_t u = MU;
    q31_t ene = 0x20000, x0 = 0x20000;
    uint8_t postshift = 8;
    nds32_nlms_q31_t Si = {numtap, state, coe, u, postshift, ene, x0};
    q31_t intput[bsize], reint[bsize], output[bsize * loop], err[bsize];
    q31_t golden[bsize * loop], g_err[bsize];
    q31_t * tmpout = output;

    FILE *fptr;
    FILE *f2ptr;
    FILE *gptr;
    FILE *g2ptr;
    const char *srcdir = GETENV_SRCDIR;
    char path_buffer[512];

    sprintf (path_buffer, "%s/input_q31.bin", srcdir);
    fptr = fopen(path_buffer, "r");
    if (fptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/input_ref_q31.bin", srcdir);
    f2ptr = fopen(path_buffer, "r");
    if (f2ptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/golden_lms_norm_q31.bin", srcdir);
    gptr = fopen(path_buffer, "r");
    if (gptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/golden_err_norm_q31.bin", srcdir);
    g2ptr = fopen(path_buffer, "r");
    if (g2ptr == NULL)
        return -1;


    test_Fread(&intput, sizeof(q31_t), bsize, fptr);
    test_Fread(&reint, sizeof(q31_t), bsize, f2ptr);

    for (i = 0; i < loop; i++)
    {
        test_Fread(&g_err, sizeof(q31_t), bsize, g2ptr);
        nds32_nlms_q31(&Si, intput, reint, tmpout, err, bsize);
        tmpout += bsize;
    }
    test_Fread(&golden, sizeof(q31_t), bsize * loop, gptr);
    dump2_q31(golden, output, bsize * loop);

    fclose(fptr);
    fclose(f2ptr);
    fclose(gptr);
    fclose(g2ptr);
    free(state);
    free(coe);

    float32_t NRMSD;
    float32_t golden_f32[bsize * loop], output_f32[bsize * loop];
    nds32_convert_q31_f32(golden, golden_f32, bsize * loop);
    nds32_convert_q31_f32(output, output_f32, bsize * loop);

    NRMSD = dump_rmsd(golden_f32, output_f32, bsize * loop);

    if (NRMSD <= 0.00003)
        return 0;
    else
        return -1;
}
