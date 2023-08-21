#include <stdio.h>
#include <inttypes.h>       //for PRIx32
#include "riscv_dsp_distance_math.h"
#include "riscv_dsp_test_accuracy.h"

int main(void)
{
    uint32_t i;
    uint32_t totalsize = 9500;
    uint32_t blocksize = 95;
    uint32_t loop = totalsize / blocksize;
    uint32_t inputAvec[blocksize], inputBvec[blocksize];
    float32_t outputvec[loop], goldenvec[loop];

    FILE *fptr;
    FILE *gptr;
    FILE *f2ptr;
    const char *srcdir = GETENV_SRCDIR;

    char path_buffer[512];
    sprintf (path_buffer, "%s/input_bool_a.bin", srcdir);
    fptr = fopen(path_buffer, "r");
    if (fptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/input_bool_b.bin", srcdir);
    f2ptr = fopen(path_buffer, "r");
    if (fptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/golden_sokalmichener_dist.bin", srcdir);
    gptr = fopen(path_buffer, "r");
    if (gptr == NULL)
        return -1;

    test_Fread(&goldenvec, sizeof(float32_t), loop, gptr);

    for (i = 0; i < loop; i++)
    {
        test_Fread(&inputAvec, sizeof(uint32_t), blocksize, fptr);
        test_Fread(&inputBvec, sizeof(uint32_t), blocksize, f2ptr);
        outputvec[i] = riscv_dsp_bdist_sokal_michener_u32_f32(inputAvec, inputBvec, blocksize);
    }
    float32_t maxdiff = dump2_f32(goldenvec, outputvec, loop);

    fclose(fptr);
    fclose(f2ptr);
    fclose(gptr);

    dump_rmsd(goldenvec, outputvec, loop);

    if (maxdiff <= 0.00000001)
        return 0;
    else
        return -1;
}
