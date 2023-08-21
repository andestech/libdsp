#include <stdio.h>
#include <math.h>
#include "riscv_dsp_test_accuracy.h"
#include "riscv_dsp_svm_math.h"

#define NUMDIM 10
#define NUMVEC 3
#define INTERCEPT 0.4
#define GAMMA 0.2


const float32_t dualcoe[NUMVEC] =
{
    0.3, -0.2, -0.5
};
const float32_t vec[NUMVEC * NUMDIM] =
{
    0.1, 0.2, -0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 0.1,
    -0.1, -0.2, 0.3, -0.4, -0.5, 0.6, -0.7, -0.8, 0.9, 0.1,
    0.1, 0.2, -0.3, 0.4, 0.5, 0.6, -0.7, 0.8, 0.9, 0.1,
};
int32_t pclass[2] =
{
    3, 7
};

int main(void)
{
    uint32_t i;
    int datasize = 300;
    uint32_t blocksize = NUMDIM;
    uint32_t loop = datasize / blocksize;
    float32_t input[blocksize];
    int32_t outputda[loop], golden[loop];

    riscv_dsp_svm_rbf_f32_t Si = {NUMVEC, NUMDIM, INTERCEPT, dualcoe, vec, pclass, GAMMA};

    FILE *fptr;
    FILE *gptr;
    const char *srcdir = GETENV_SRCDIR;

    char path_buffer[512];
    sprintf (path_buffer, "%s/input_f32.bin", srcdir);
    fptr = fopen(path_buffer, "r");
    if (fptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/golden_svm_rbf_predict_f32.bin", srcdir);
    gptr = fopen(path_buffer, "r");
    if (gptr == NULL)
        return -1;

    test_Fread(&golden, sizeof(int32_t), loop, gptr);
    for (i = 0; i < loop; i++)
    {
        test_Fread(&input, sizeof(float32_t), blocksize, fptr);
        riscv_dsp_svm_rbf_est_f32(&Si, input, &outputda[i]);
    }
#ifdef PRINT_DETAIL
    dump2_q31(golden, outputda, loop);
#endif
    for (i = 0; i < loop; i++)
    {
        if (outputda[i] != golden[i])
        {
            printf("checking FAIL\n");
            return -1;
        }
    }
    printf("checking PASS\n");

    fclose(fptr);
    fclose(gptr);
    return 0;

}
