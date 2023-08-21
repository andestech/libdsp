#include "stdio.h"
#include "riscv_dsp_test_accuracy.h"
#include "riscv_dsp_test_data.h"

#define xstr(s) str(s)
#define str(s) #s

void main(void)
{
    int i;
    int fftlogn = GETENV_LOG;
    int fftlen = (1 << GETENV_LOG);
    printf("in: m = $d, n = %d\n", fftlogn, fftlen);
    int datasize = 2 * fftlen;
    double val;

    char path_buffer[1024];
    sprintf (path_buffer, "input_f64_%s.bin", xstr(GETENV_LOG));
    FILE *fp = fopen(path_buffer, "w");

    int count = 1;

    for (i = 0; i < datasize; i++)
    {
        val = (double)test_fft_Input[i];
        printf("%f\n", val);
        fwrite(&val, sizeof(double), 1, fp);
        count++;
    }
    fclose(fp);
}
