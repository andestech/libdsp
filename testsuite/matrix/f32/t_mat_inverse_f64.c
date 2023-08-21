#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "riscv_dsp_test_accuracy.h"
#include "riscv_dsp_matrix_math.h"
#include "riscv_dsp_matrix_f64_testdata.h"

void mat_rmsd(float64_t *goldensig, float64_t *testsig, uint32_t size, float64_t result[])
{
    uint32_t i;
    double Sumsquareerr = 0.0f;
    float64_t Sumerr = 0.0f;
    float64_t RMSD;
    float64_t MAE;
    float64_t NRMSD;
    float64_t MAXDIFF = 0.0f;
    float64_t diff;
    float64_t SNR;
    double squaregloden = 0.0f;
    float64_t maxsig = 0.0f, minsig = 0.0f;

    for (i = 0; i < size; i++)
    {
        float32_t testin = *testsig++, golden = *goldensig++;

        maxsig = (golden > maxsig) ? golden : maxsig;
        minsig = (golden < minsig) ? golden : minsig;
        diff  = fabsf(testin - golden);
        Sumerr += diff;

        MAXDIFF = (diff > MAXDIFF) ? diff : MAXDIFF;
        Sumsquareerr += diff * diff;
        squaregloden += golden * golden;
    }

    MAE = Sumerr / size;
    RMSD = sqrtf(Sumsquareerr / size);
    NRMSD = RMSD / (maxsig - minsig);
    if (Sumsquareerr == 0.0)
    {
        SNR = ERROR_EQ_ZERO;
    }
    else
    {
        SNR = 10.0 * log10(squaregloden / Sumsquareerr);
    }

    if (result != NULL)
    {
        result[0] = MAE;
        result[1] = RMSD;
        result[2] = NRMSD;
        result[3] = MAXDIFF;
        result[4] = SNR;
    }
    else
    {
        printf("--------------------\n");
        printf("MAE is %5.10lf, RMSD is %5.10lf, NRMSD is %5.10lf, MAXDIFF is %5.10lf, SNR is %5.10lf\n", MAE, RMSD, NRMSD, MAXDIFF, SNR);
    }
}


void printMat(const char *hdr, float64_t * mat, int row_size)
{
#ifdef PRINT_DETAIL
    int i, j;
    float64_t * temp = mat;

    LOG("%s [%d * %d]:\n", hdr, row_size, row_size);
    for (i = 0; i < row_size; i++)
    {
        for (j = 0; j < row_size; j++)
        {
            LOG("%10.10llf, ", *temp);
            temp++;
        }
        LOG("\n");
    }
    LOG("--------------------\n");
#endif
}

void riscv_dsp_mat_mul_f64(const float64_t *src1, const float64_t *src2,
                           float64_t *dst, uint32_t row, uint32_t col, uint32_t col2)
{
    const float64_t *A = src1;
    const float64_t *B = src2;
    const float64_t *InA = A;
    float64_t *C;
    float64_t sum;
    //double sum;
    uint32_t i, colcnt, col2cnt, rowcnt;

    i = 0u;
    rowcnt = row;
    do
    {
        C = dst + i;
        B = src2;

        /* Dot product of each row in src1 with each column in src2 */
        col2cnt = col2;
        do
        {
            sum = 0.0f;
            //sum = 0.0;
            A = InA;

            /* column loop */
            colcnt = col;
            do
            {
                sum += (*A++) * *B;
                B += col2;
                colcnt--;
            }
            while (colcnt != 0u);

            *C++ = sum;
            //*C++ = (float32_t)sum;
            col2cnt--;
            B = src2 + (col2 - col2cnt);
        }
        while (col2cnt != 0u);

        i = i + col2;
        InA = A;
        rowcnt--;
    }
    while (rowcnt != 0u);
}

/**
 * @} end of matrixmul
 */
int CheckInv(float64_t * A, float64_t * Ainv, uint32_t size, float64_t result[])
{
    uint32_t j = 0, i;
    float64_t checkI[size * size];
    float64_t idenI[size * size];
    riscv_dsp_mat_mul_f64(Ainv, A, checkI, size, size, size);
    for (i = 0; i < size * size; i++)
    {
        if (i == j)
        {
            idenI[i] = 1.0f;
            j += (size + 1);
        }
        else
        {
            idenI[i] = 0.0f;
        }
    }
    printMat("checkI", checkI, size);
    printMat("idenI", idenI, size);

    mat_rmsd(idenI, checkI, size * size, result);

    return 0;
}


int DoMatInv(struMat * instance, float64_t result[])
{
    int ret;
#ifdef PRINT_DETAIL
    static int num = 0;
#endif
    int size = instance->row_size;
    float64_t bak[size * size];
    float64_t invMat[size * size];
    float64_t now_result[5] = {0.0};
    memcpy(bak, instance->mat, sizeof(float64_t) * size * size);

    LOG("--------------------------\n");
    LOG("Case %d:\nThe input matrix is:\n", num);
    printMat("A", instance->mat, size);
    ret = riscv_dsp_mat_inv_f64(instance->mat, invMat, size);
    if (ret != instance->answer)
    {
        LOG("ERROR! The function returns %d but answer is %d\n", ret, instance->answer);
        LOG("--------------------\n");
        LOG("Case %d fail\n", num);
        return -1;
    }
    else
    {
        if (instance->answer == 0)
        {
            LOG("The input matrix is invertable.\n");
            LOG("--------------------\n");
            LOG("The inverse matrix is:\n");
            printMat("Ainv", invMat, size);
            CheckInv(bak, invMat, size, now_result);
            LOG("Case %d pass\n", num);
        }
        else
        {
            LOG("The input matrix is SINGULAR.\n");
            LOG("Case %d pass\n", num);
        }
    }

    if (now_result[0] > result[0])
        result[0] = now_result[0];
    if (now_result[1] > result[1])
        result[1] = now_result[1];
    if (now_result[2] > result[2])
        result[2] = now_result[2];
    if (now_result[3] > result[3])
        result[3] = now_result[3];
    if (now_result[4] > result[4])
        result[4] = now_result[4];
#ifdef PRINT_DETAIL
    num++;
#endif
    return 0;
}

int main(void)
{
    uint32_t i;
    float64_t result[5] = {0.0};
    int status;
    for (i = 0; i < sizeof(matArray) / sizeof(struMat); i++)
    {
        status = DoMatInv(&matArray[i], result);
        if (status != 0)
            return -1;
    }
    for (i = 0; i < sizeof(matArray2) / sizeof(struMat); i++)
    {
        status = DoMatInv(&matArray2[i], result);
        if (status != 0)
            return -1;
    }
    printf("MAE is %5.10lf, RMSD is %5.10lf, NRMSD is %5.10lf, MAXDIFF is %5.10lf, SNR is %5.10lf\n", result[0], result[1], result[2], result[3], result[4]);
    return 0;
}
