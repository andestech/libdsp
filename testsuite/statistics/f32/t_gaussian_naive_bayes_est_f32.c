#include <stdio.h>
#include <math.h>
#include "riscv_dsp_test_accuracy.h"
#include "riscv_dsp_statistics_math.h"

#define NUMDIM 3
#define NUMCLASS 3
#define ADDVAR 0.3

const float32_t mean[NUMCLASS * NUMDIM] =
{
    0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9
};
const float32_t var[NUMCLASS * NUMDIM] =
{
    0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9
};
const float32_t classprior[NUMCLASS] =
{
    0.1, 0.2, 0.3
};

int main(void)
{
    uint32_t i;
    int datasize = 300;
    uint32_t blocksize = NUMCLASS;
    uint32_t loop = datasize / blocksize;
    float32_t input[NUMDIM], buffer[NUMCLASS];
    float32_t outputda[loop], golden[loop];

    riscv_dsp_gaussian_naivebayes_f32_t Si = {NUMDIM, NUMCLASS, mean, var, classprior, ADDVAR};

    FILE *fptr;
    FILE *gptr;
    const char *srcdir = GETENV_SRCDIR;

    char path_buffer[512];
    sprintf (path_buffer, "%s/input_f32.bin", srcdir);
    fptr = fopen(path_buffer, "r");
    if (fptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/golden_gaussian_naive_bayes_predict_f32.bin", srcdir);
    gptr = fopen(path_buffer, "r");
    if (gptr == NULL)
        return -1;


    for (i = 0; i < loop; i++)
    {
        test_Fread(&input, sizeof(float32_t), NUMDIM, fptr);
        outputda[i] = riscv_dsp_gaussian_naive_bayes_est_f32(&Si, input, buffer);
    }
    test_Fread(&golden, sizeof(float32_t), loop, gptr);
    dump2_f32(golden, outputda, loop);

    fclose(fptr);
    fclose(gptr);

    float32_t NRMSD;
    NRMSD = dump_rmsd(golden, outputda, loop);

    if (NRMSD <= 0.0000001)
        return 0;
    else
        return -1;
}