#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "riscv_dsp_basic_math.h"
#include "riscv_dsp_utils_math.h"
#include "riscv_dsp_test_accuracy.h"


int main(void)
{
    int i;
    FILE *fptr;
    FILE *gptr;
    int inputx, inputy;
    int datasize = 30000;

    float fz[datasize], f2[datasize];
    const char *srcdir = GETENV_SRCDIR;

    char path_buffer[512];
    sprintf (path_buffer, "%s/input_q31_y.bin", srcdir);
    fptr = fopen(path_buffer, "r");
    if (fptr == NULL)
        return -1;

    sprintf (path_buffer, "%s/input_q31_x.bin", srcdir);
    gptr = fopen(path_buffer, "r");
    if (gptr == NULL)
        return -1;


    for (i = 0; i < datasize; i++)
    {
        test_Fread(&inputy, sizeof(int), 1, fptr);
        test_Fread(&inputx, sizeof(int), 1, gptr);

        float fy = convert_q31_to_float(inputy);
        float fx = convert_q31_to_float(inputx);

        fz[i] = atan2f(fy, fx);

        /* ------------------performance calc.---------------- */
        q31_t q2 = riscv_dsp_atan2_q31(inputy, inputx);
        /* ------------------performance calc.---------------- */

        f2[i] = convert_q31_to_float(q2) * PI;
    }

    q31_t q3 = riscv_dsp_atan2_q31(0, inputx);
    if (q3 != 0) return -1;
    dump2_f32(fz, f2, datasize);

    fclose(fptr);
    fclose(gptr);

    float32_t NRMSD;
    NRMSD = dump_rmsd(fz, f2, datasize);

    if (NRMSD <= 0.00009)
        return 0;
    else
        return -1;
}
