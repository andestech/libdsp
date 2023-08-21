/* --------- Include header --------------*/
#include "stdlib.h"            /* rand */

#define SIZE 1024

//global variable for getting average cycles.
volatile int total_samples = (int)SIZE;

#if defined(__NDS32__) || defined(__riscv)
#include "nds32_pfcounter.h"   /* performance counter */
#endif

#define PI 3.14159265358979f
#define RES_N 1024

typedef union
{
    float32_t f;
    unsigned int u;
} union32_t;

#define SIZE_F32 131072
#define BUF_SIZE_F32 1024
#define TEST_LOOP_F32 (SIZE_F32/BUF_SIZE_F32)
#define SAMPLES 32768 /* --------- for sqrt, atan, sin and cos  --------*/

#define LOG_SIZE 10
float32_t interval_q =  (((PI) / SAMPLES) / PI); // divide by PI: transform radian to Q15 and Q31
float32_t interval_f =  ((2 * PI) / SIZE_F32);

#define FUNC_ATTR_ALIGN_4   __attribute__ ((aligned(4)))
#define FUNC_ATTR_ALIGN_8   __attribute__ ((aligned(8)))
#define FUNC_ATTR_ALIGN_16  __attribute__ ((aligned(16)))

#define FUNC_ATTR_ALIGN FUNC_ATTR_ALIGN_8    //After can defined for different cpu type.

/* --------- filtering --------*/
#define BLOCKSIZE 6
#define NUM_TAP 4

/* --------- conv, corr --------*/
#define BLOCKSIZE_2 16
#define CONV_LEN (SIZE + BLOCKSIZE_2 - 1) /* len1 + len2 - 1 */
#define STARTINDEX 100
#define CONVSIZE 512

/* --------- fir, dcmfir --------------*/
#define DCM_FACTOR      2
#define FIR_NUM_TAP     16
#define FIR_STATE_LEN (BUF_SIZE_F32 + FIR_NUM_TAP - 1)

/* --------- spafir --------------*/
#define MAX_DELAY 3
#define SPAFIR_STATE_LEN (BLOCKSIZE + MAX_DELAY)

/* --------- biquad_df1 --------------*/
#define BIQUAD_STAGE 3
#define POST_SHIFT 3
#define STATE_LEN (4 * BIQUAD_STAGE)

#ifdef NDS_BQ_DF1_Q15_USE_SIMD
#define COE_LEN (6 * BIQUAD_STAGE)
#else
#define COE_LEN (5 * BIQUAD_STAGE)
#endif

/* --------- biquad_df2T --------------*/
#define BIQUAD_DF2T_STATE_LEN (2 * BIQUAD_STAGE)


/* --------- upsplfir --------------*/
#define UPSPL_FACTOR 2
#define PHASE_LEN 16
#define UPSPL_STATE_LEN (BUF_SIZE_F32 + PHASE_LEN - 1)
#define UPSPL_COE_LEN (UPSPL_FACTOR * PHASE_LEN)

/* --------- lfir --------------*/
#define LFIR_STATE_LEN 16

/* --------- liir --------------*/
#define LIIR_STAGE 16
#define LIIR_STATE_LEN (LIIR_STAGE + BUF_SIZE_F32)
#define V_COE_LEN (LIIR_STAGE + 1)

/* --------- lms --------------*/
#define LMS_SHIFT 4
#define MU 0.5

/* --------- Printf definitions ---------*/
#define PRINTF_F32(X)   printf("%f\n", (float)(X))
#define PRINTF_INT32(X) printf("%08x\n", (int)(X))
#define PRINTF_INT64(X) printf("%08x %08x\n", (int)(X>>32), (int)(X))

/* --------- matrix ------------*/
#define ROW 16
#define COL 16
#define MAT_SIZE (ROW * COL)
#define ROW_F32 64
#define COL_F32 64
#define MAT_SIZE_F32 (ROW_F32 * COL_F32)
#define INV_ROW 16
#define INV_COL INV_ROW
#define INV_SIZE (INV_ROW * INV_COL)
#define ROW_F64 32
#define COL_F64 32
#define MAT_SIZE_F64 (ROW_F64 * COL_F64)

/* --------- mat_mul ------------*/
#define COL2 16
#define MAT2_SIZE (COL * COL2)
#define MAT_MUL_OUT (ROW * COL2)

/* --------- mat_mul ------------*/
#define MXV_ROW 64
#define MXV_COL 64

/* --------- mat_trans ------------*/
#define MAT_TRANS_OUT (COL * ROW)

/* --- cfft_radix2, cfft_radix4 --- */
#define FFT_LOGN 8
#define FFT_N (1 << FFT_LOGN)

/* --- dct, dct4, rfft --- */
#define FFT2_LOGN 7
#define FFT2_N (1 << FFT2_LOGN)

/* --- shift --- */
#define SHIFT_BITS 5

/* --- PID CONTROL --- */
#define KP_VAL 0.02
#define KI_VAL 0.03
#define KD_VAL 0.014
#define KP_VAL_Q 293
#define KI_VAL_Q 111
#define KD_VAL_Q 20000
#define RESET 1

/* --- barycenter --- */
#define VECDIM 8
#define VECNUM 128

/* --- gaussian_naive_bayes_predict --- */
#define NUMOFDIM  32
#define NUMOFCLASS  16
#define ADDVAR  0.3

/* --- SVM --- */
#define SVMDIM   32
#define SVMVEC   32
#define GAMMA    0.3
#define COEF0    0.2
#define EXPONENT 3
#define INTERCEPT -0.1

/* --- distance --- */
#define ORDER 3

// generate random float [-2.0, 2,0)
static inline float gen_frandom(void)
{
    short a = rand() & 0xff;
    short signbit = a & 0x1;
    float out = ((float)a / 128);
    if (signbit == 0x1)
        return out;
    else
        return -out;
}
