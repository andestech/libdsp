#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "riscv_dsp_utils_math.h"
#include "riscv_dsp_test_accuracy.h"

#define TEST_SIZE 16384

#define CONST_PI (3.14159265358979323846)
#define INTER_PI (4 * CONST_PI / TEST_SIZE)

float16_t src[TEST_SIZE]  ;
float16_t out[TEST_SIZE]  ;
float32_t golden[TEST_SIZE]  ;
float32_t srcf32[TEST_SIZE]  ;
float32_t outf32[TEST_SIZE]  ;

int main(void)
{
    uint32_t buff_size = TEST_SIZE;

    FILE *fptr;
    FILE *gptr;
    const char *srcdir = GETENV_SRCDIR;

    char path_buffer[512];
    sprintf (path_buffer, "%s/input_sin_f32.bin", srcdir);
    fptr = fopen(path_buffer, "r");
    if (fptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/golden_sin_f32.bin", srcdir);
    gptr = fopen(path_buffer, "r");
    if (gptr == NULL)
        return -1;

    test_Fread(&srcf32, sizeof(float32_t), buff_size, fptr);
    test_Fread(&golden, sizeof(float32_t), buff_size, gptr);

    for (int j = 0; j < buff_size; j++)
    {
        src[j] = (float16_t)srcf32[j];
        out[j] = riscv_dsp_sin_f16(src[j]);
        outf32[j] = (float32_t)out[j];
    }

    dump2_f32(golden, outf32, buff_size);

    fclose(fptr);
    fclose(gptr);

    float32_t NRMSD = dump_rmsd(golden, outf32, buff_size);

    if (NRMSD <= 0.0003)
        return 0;
    else
        return -1;
}
