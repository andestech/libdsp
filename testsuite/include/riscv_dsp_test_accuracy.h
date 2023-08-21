#ifndef __RISCV_DSP_TEST_ACCURACY_H__
#define __RISCV_DSP_TEST_ACCURACY_H__
/***************************************************************************
 * riscv_dsp_test_accuracy.h                                                   *
 *                                                                         *
 * Copyright (C) 2012-2016 Andes Technology Corporation                    *
 * All rights reserved.                                                    *
 ***************************************************************************/

#include <math.h>
#include <stdlib.h>
#include "riscv_dsp_utils_math.h"

//Default: LOG is disable.
//If you want to enable LOG, please use next line.
#ifdef PRINT_DETAIL
#define LOG printf
#else
#define LOG(...)
#endif

#define PI 3.14159265358979f

#define RES_N (1 << RES_LOGN)
#ifdef TRANSFORM_SAMPLES_LOG
#define TRANSFORM_SAMPLES (1 << TRANSFORM_SAMPLES_LOG)
#endif

#ifdef FOUR_PI_RANGE
// radian [-2 * PI, 2 * PI) is mapped to [-1, 1)
// degree [-360, 360) is mapped to [-1, 1)
#define UNIT_Q31    (1.0f/2147483648.0f)
#define TWO_PI_Q31  INT32_MIN   // 0x80000000
#define PI_Q31      1073741824  // 0x40000000
#define HALF_PI_Q31 536870912   // 0x20000000

#define UNIT_Q15    (1.0f/32768.0f)
#define TWO_PI_Q15  INT16_MIN   // 0x8000
#define PI_Q15      16384       // 0x4000
#define HALF_PI_Q15 8192        // 0x2000

#define UNIT_Q7     (1.0f/128.0f)
#define TWO_PI_Q7   INT8_MIN    // 0x80
#define PI_Q7       64          // 0x40
#define HALF_PI_Q7  32          // 0x20

#else
// radian [-PI, PI) is mapped to [-1, 1)
// degree [-180, 180) is mapped to [-1, 1)
#define UNIT_Q31    (1.0f/2147483648.0f)
#define PI_Q31      INT32_MIN   // 0x80000000
#define HALF_PI_Q31 1073741824  // 0x40000000

#define UNIT_Q15    (1.0f/32768.0f)
#define PI_Q15      INT16_MIN   // 0x8000
#define HALF_PI_Q15 16384       // 0x4000

#define UNIT_Q7     (1.0f/128.0f)
#define PI_Q7       INT8_MIN    // 0x80
#define HALF_PI_Q7  64          // 0x40
#endif // FOUR_PI_RANGE

#define UNIT_U8     (1.0f/256.0f)
#define UNIT_U16    (1.0f/65536.0f)
#define UNIT_U32    (1.0f/4294967296.0f)
#define UNIT_U64    (1.0/18446744073709551616.0)
// Type Conversion
/*
 * nds32_convert_(source type)_to_(destination type)
 *      Comput the element of the source type to destination type.
 */

/**
 * @brief convert_src_to_dst.
 * @param[in]   *src point to the input vector.
 * @param[out]  *dst point to the output vector.
 * @param[in]   size size of the vectors.
 * @return none.
 */

// floating-point to Q(destination type)
static inline q15_t convert_float_to_q15(float x)
{
    q31_t q31;
    q15_t q15;
    x *= 32768.0f;
    x += (x < 0.0f ? -0.5f : 0.5f);
    q31 = (q31_t)x;
    if (q31 >= 32768)
    {
        q15 = INT16_MAX;
    }
    else if (q31 <= -32768)
    {
        q15 = INT16_MIN;
    }
    else
    {
        q15 = (q15_t)q31;
    }
    return q15;
}

#ifdef FOUR_PI_RANGE
// x must be in [-2 * PI, 2 * PI)
#define map_radian_float_to_q15(x)  convert_float_to_q15(x * (0.5f / PI))
#else
// x must be in [-PI, PI)
#define map_radian_float_to_q15(x)  convert_float_to_q15(x * (1.0f / PI))
// x must be in [-180, 180)
#define map_degree_float_to_q15(x)  convert_float_to_q15(x * (1.0f / 180.f))
#endif // FOUR_PI_RANGE

static inline q31_t convert_float_to_q31(float x)
{
    q63_t q63;
    q31_t q31;
    x *= 2147483648.0f;
    x += (x < 0.0f ? -0.5f : 0.5f);
    q63 = (q63_t)x;
    if (q63 >= 2147483648LL)
    {
        q31 = INT32_MAX;
    }
    else if (q63 <= -2147483648LL)
    {
        q31 = INT32_MIN;
    }
    else
    {
        q31 = (q31_t)q63;
    }

    return q31;
}

#ifdef FOUR_PI_RANGE
// x must be in [-2 * PI, 2 * PI)
#define map_radian_float_to_q31(x)  convert_float_to_q31(x * (0.5f / PI))
#else
// x must be in [-PI, PI)
#define map_radian_float_to_q31(x)  convert_float_to_q31(x * (1.0f / PI))
// x must be in [-180, 180)
#define map_degree_float_to_q31(x)  convert_float_to_q31(x * (1.0f / 180.f))
#endif // FOUR_PI_RANGE

static inline q7_t convert_float_to_q7(float x)
{
    q31_t q31;
    q7_t q7;

    x *= 128.0f;
    x += (x < 0.0f ? -0.5f : 0.5f);
    q31 = (q31_t)x;
    if (q31 >= 128)
    {
        q7 = INT8_MAX;
    }
    else if (q31 <= -128)
    {
        q7 = INT8_MIN;
    }
    else
    {
        q7 = (q7_t)q31;
    }

    return q7;
}

// Q31 to destination type
static inline float convert_q31_to_float(q31_t x)
{
    return x * UNIT_Q31;
}

static inline q15_t convert_q31_to_q15(q31_t x)
{
    return (q15_t)(x >> 16);
}

static inline q7_t convert_q31_to_q7(q31_t x)
{
    return (q7_t)(x >> 24);
}

#ifdef FOUR_PI_RANGE
static inline float map_radian_q31_to_float(q31_t x)
{
    return x * (2.0f * PI * UNIT_Q31);
}
#else
static inline float map_radian_q31_to_float(q31_t x)
{
    return x * (PI * UNIT_Q31);
}

static inline float map_degree_q31_to_float(q31_t x)
{
    return x * (180.f * UNIT_Q31);
}
#endif // FOUR_PI_RANGE

// Q15 to destination type
static inline float convert_q15_to_float(q15_t x)
{
    return x * UNIT_Q15;
}

static inline q31_t convert_q15_to_q31(q15_t x)
{
    return ((q31_t)x) << 16;
}

static inline q7_t convert_q15_to_q7(q15_t x)
{
    return (q7_t)(x >> 8);
}

#ifdef FOUR_PI_RANGE
static inline float map_radian_q15_to_float(q15_t x)
{
    return x * (2.0f * PI * UNIT_Q15);
}
#else
static inline float map_radian_q15_to_float(q15_t x)
{
    return x * (PI * UNIT_Q15);
}

static inline float map_degree_q15_to_float(q15_t x)
{
    return x * (180.f * UNIT_Q15);
}
#endif // FOUR_PI_RANGE

// Q7 to destination type
static inline float convert_q7_to_float(q7_t x)
{
    return x * UNIT_Q7;
}

static inline q31_t convert_q7_to_q31(q7_t x)
{
    return ((q31_t)x) << 24;
}

static inline q15_t convert_q7_to_q15(q7_t x)
{
    return ((q15_t)x) << 8;
}

#include <inttypes.h>


static inline void dump_f64(const char *hdr, float64_t *buf, uint32_t size)
{
    unsigned int i;

    LOG("%s:\n", hdr);
    for (i = 0; i < size; i++)
    {
#ifdef PRINT_DETAIL
        float64_t re = *buf++;
        LOG("%4d: %11.8lf\n", i, re);
#endif
    }
    LOG("------------------\n");
}

#if defined (__riscv_zfh)
static inline void dump_f16(const char *hdr, float16_t *buf, uint32_t size)
{
    unsigned int i;

    LOG("%s:\n", hdr);
    for (i = 0; i < size; i++)
    {
#ifdef PRINT_DETAIL
        float64_t re = *buf++;
        LOG("%4d: %5.6lf\n", i, re);
#endif
    }
    LOG("------------------\n");
}
#endif

static inline void dump_f32(const char *hdr, float32_t *buf, uint32_t size)
{
    unsigned int i;

    LOG("%s:\n", hdr);
    for (i = 0; i < size; i++)
    {
#ifdef PRINT_DETAIL
        float32_t re = *buf++;
        LOG("%4d: %11.8f\n", i, re);
#endif
    }
    LOG("------------------\n");
}

static inline void dump_cmat_f32(float32_t *buf, uint32_t row, uint32_t col)
{
    unsigned int i, j;
    for (i = 0; i < row; i++)
    {
        for (j = 0; j < col; j++)
        {
#ifdef PRINT_DETAIL
        LOG("(%.6f, %.6f), ", buf[2 * j + i * row * 2], buf[2 * j + 1 + i * row * 2]);
#endif
        }
        LOG("\n");
    }
    LOG("------------------\n");
}

static inline void dump_q7(const char *hdr, q7_t *buf, uint32_t size)
{
    unsigned int i;

    LOG("%s:\n", hdr);
    for (i = 0; i < size; i++)
    {
#ifdef PRINT_DETAIL
        q7_t re = *buf++;
        LOG("%4d: 0x%02x [%2.8f]\n", i, re & 0xff, re / 256.0f);
#endif
    }
    LOG("------------------\n");
}

static inline void dump_q15(const char *hdr, q15_t *buf, uint32_t size)
{
    unsigned int i;

    LOG("%s:\n", hdr);
    for (i = 0; i < size; i++)
    {
#ifdef PRINT_DETAIL
        q15_t re = *buf++;
        LOG("%4d: 0x%04x [%2.8f]\n", i, re & 0xffff, re / 32768.0f);
#endif
    }
    LOG("------------------\n");
}

static inline void dump_cmat_q15(q15_t *buf, uint32_t row, uint32_t col)
{
    unsigned int i, j;
    for (i = 0; i < row; i++)
    {
        for (j = 0; j < col; j++)
        {
#ifdef PRINT_DETAIL
        LOG("(0x%04x, 0x%04x), ", buf[2 * j + i * row * 2] & 0xffff, buf[2 * j + 1 + i * row * 2] & 0xffff);
#endif
        }
        LOG("\n");
    }
    LOG("------------------\n");
}

static inline void dump_q31(const char *hdr, q31_t *buf, uint32_t size)
{
    unsigned int i;

    LOG("%s:\n", hdr);
    for (i = 0; i < size; i++)
    {
#ifdef PRINT_DETAIL
        q31_t re = *buf++;
        LOG("%4d: 0x%08" PRIx32 "  [%11.8f]\n", i, re, re / 2147483648.0);
#endif
    }
    LOG("------------------\n");
}

static inline void dump_cmat_q31(q31_t *buf, uint32_t row, uint32_t col)
{
    unsigned int i, j;
    for (i = 0; i < row; i++)
    {
        for (j = 0; j < col; j++)
        {
#ifdef PRINT_DETAIL
        LOG("(0x%08" PRIx32 ", 0x%08" PRIx32 "), ", buf[2 * j + i * row * 2], buf[2 * j + 1 + i * row * 2]);
#endif
        }
        LOG("\n");
    }
    LOG("------------------\n");
}

static inline float32_t dump2_f32(float32_t *buf, float32_t *buf2, uint32_t size)
{
    unsigned int i;
    float32_t maxdiff = 0.0f;

    LOG("-----  output --- | -- golden -- |  abs diff --\n");
    for (i = 0; i < size; i++)
    {
        float32_t golden = *buf++, testout = *buf2++;
        float32_t diff = testout - golden;
        diff = fabsf(diff);
        maxdiff = (diff > maxdiff) ? diff : maxdiff;
        LOG("%4d: %11.8f |  %11.8f | %11.8f\n", i, testout, golden, diff);
    }
    printf("MAXDIFF is %11.8f\n", maxdiff);
    return maxdiff;
}

#if defined (__riscv_zfh)
static inline float16_t dump2_f16(float16_t *buf, float16_t *buf2, uint32_t size)
{
    unsigned int i;
    float16_t maxdiff = 0.0;

    LOG("-----  output --- | -- golden -- |  abs diff --\n");
    for (i = 0; i < size; i++)
    {
        float16_t golden = *buf++, testout = *buf2++;
        float16_t diff = testout - golden;
        diff = fabsf(diff);
        maxdiff = (diff > maxdiff) ? diff : maxdiff;
        LOG("%4d: %8.6f |  %8.6f | %8.6f\n", i, (float32_t)testout, (float32_t)golden, (float32_t)diff);
    }
    printf("MAXDIFF is %8.6f\n", (float32_t)maxdiff);
    return maxdiff;
}
#endif

static inline float64_t dump2_f64(float64_t *buf, float64_t *buf2, uint32_t size)
{
    unsigned int i;
    float64_t maxdiff = 0.0f;

    LOG("-----  output --- | -- golden -- |  abs diff --\n");
    for (i = 0; i < size; i++)
    {
        float64_t golden = *buf++, testout = *buf2++;
        float64_t diff;

        if (golden > testout)
           diff = golden - testout;
        else
           diff = testout - golden;

        maxdiff = (diff > maxdiff) ? diff : maxdiff;
        LOG("%4d: %11.8lf |  %11.8lf | %11.8lf\n", i, testout, golden, diff);
    }
    printf("MAXDIFF is %11.8lf\n", maxdiff);
    return maxdiff;
}

static inline q7_t dump2_q7(q7_t *buf, q7_t *buf2, uint32_t size)
{
    unsigned int i;
    q7_t maxdiff = 0;

    LOG("--- output [floatingout ] | golden [floatinggolden]| --- abs diff ---\n");
    for (i = 0; i < size; i++)
    {
        q7_t golden = *buf++, testout = *buf2++;
        q7_t diff = testout - golden;
        diff = abs(diff);
        maxdiff = (diff > maxdiff) ? diff : maxdiff;
        LOG("%4d: 0x%02x [%11.8f ] | 0x%02x   [%11.8f ]  | 0x%02x [%2.8f]\n", i, testout & 0xff, testout * UNIT_Q7, golden & 0xff, golden * UNIT_Q7, diff & 0xff, diff * UNIT_Q7);
    }
    printf("MAXDIFF is 0x%02x\n", maxdiff);
    return maxdiff;
}

static inline q7_t dump2_u8(uint8_t *buf, uint8_t *buf2, uint32_t size)
{
    unsigned int i;
    uint8_t maxdiff = 0;

    LOG("--- output [floatingout ] | golden [floatinggolden]| --- abs diff ---\n");
    for (i = 0; i < size; i++)
    {
        uint8_t golden = *buf++, testout = *buf2++;
        uint8_t diff;

        if (golden > testout)
           diff = golden - testout;
        else
           diff = testout - golden;

        maxdiff = (diff > maxdiff) ? diff : maxdiff;
        LOG("%4d: 0x%02x [%11.8f ] | 0x%02x   [%11.8f ]  | 0x%02x [%2.8f]\n", i, testout & 0xff, testout * UNIT_U8, golden & 0xff, golden * UNIT_U8, diff & 0xff, diff * UNIT_U8);
    }
    printf("MAXDIFF is 0x%02x\n", maxdiff);
    return maxdiff;
}

static inline q15_t dump2_q15(q15_t *buf, q15_t *buf2, uint32_t size)
{
    unsigned int i;
    q15_t maxdiff = 0;

    LOG("----- output [floatingout] | golden [floatinggolden] | ------ abs diff ------\n");
    for (i = 0; i < size; i++)
    {
        q15_t golden = *buf++, testout = *buf2++;
        q15_t diff = testout - golden;
        diff = abs(diff);
        maxdiff = (diff > maxdiff) ? diff : maxdiff;
        LOG("%4d: 0x%04x [%11.8f] | 0x%04x   [%11.8f]  | 0x%08x [%2.8f]\n", i, testout & 0xffff, testout * UNIT_Q15, golden & 0xffff, golden * UNIT_Q15, diff, diff/32768.0);
    }
    printf("MAXDIFF is 0x%04x\n", maxdiff);
    return maxdiff;
}

static inline uint16_t dump2_u16(uint16_t *buf, uint16_t *buf2, uint32_t size)
{
    unsigned int i;
    uint16_t maxdiff = 0;

    LOG("----- output [floatingout] | golden [floatinggolden] | ------ abs diff ------\n");
    for (i = 0; i < size; i++)
    {
        uint16_t golden = *buf++, testout = *buf2++;
        uint16_t diff;

        if (golden > testout)
           diff = golden - testout;
        else
           diff = testout - golden;

        maxdiff = (diff > maxdiff) ? diff : maxdiff;
        LOG("%4d: 0x%04x [%11.8f] | 0x%04x   [%11.8f]  | 0x%08x [%2.8f]\n", i, testout & 0xffff, testout * UNIT_U16, golden & 0xffff, golden * UNIT_U16, diff, diff/65536.0);
    }
    printf("MAXDIFF is 0x%04x\n", maxdiff);
    return maxdiff;
}

static inline q31_t dump2_q31(q31_t *buf, q31_t *buf2, uint32_t size)
{
    unsigned int i;
    q31_t maxdiff = 0;

    LOG("----- output ---- [floatingout] | golden ------ [floatinggolden] | ------ abs diff ------\n");
    for (i = 0; i < size; i++)
    {
        q31_t golden = *buf++, testout = *buf2++;
        q31_t diff = golden - testout;
        diff = abs(diff);
        maxdiff = (diff > maxdiff) ? diff : maxdiff;
        LOG("%4d: 0x%08" PRIx32 "  [%11.8f] | 0x%08" PRIx32 "     [%11.8f]   | 0x%08" PRIx32 "  [%11.8f]\n", i, testout, testout / 2147483648.0, golden, golden / 2147483648.0, diff, diff / 2147483648.0);
    }
    printf("MAXDIFF is 0x%08" PRIx32 "\n", maxdiff);
    return maxdiff;
}

static inline uint32_t dump2_u32(uint32_t *buf, uint32_t *buf2, uint32_t size)
{
    unsigned int i;
    uint32_t maxdiff = 0;

    LOG("----- output ---- [floatingout] | golden ------ [floatinggolden] | ------ abs diff ------\n");
    for (i = 0; i < size; i++)
    {
        uint32_t golden = *buf++, testout = *buf2++;
        uint32_t diff;

        if (golden > testout)
           diff = golden - testout;
        else
           diff = testout - golden;

        maxdiff = (diff > maxdiff) ? diff : maxdiff;
        LOG("%4d: 0x%08" PRIx32 "  [%11.8f] | 0x%08" PRIx32 "     [%11.8f]   | 0x%08" PRIx32 "  [%11.8f]\n", i, testout, testout / 2147483648.0, golden, golden / 2147483648.0, diff, diff / 2147483648.0);
    }
    printf("MAXDIFF is 0x%08" PRIx32 "\n", maxdiff);
    return maxdiff;
}

static inline uint64_t dump2_u64(uint64_t *buf, uint64_t *buf2, uint32_t size)
{
    unsigned int i;
    uint64_t maxdiff = 0;

    LOG("----- output ---- [floatingout] | golden ------ [floatinggolden] | ------ abs diff ------\n");
    for (i = 0; i < size; i++)
    {
        uint64_t golden = *buf++, testout = *buf2++;
        uint64_t diff;

        if (golden > testout)
           diff = golden - testout;
        else
           diff = testout - golden;

        maxdiff = (diff > maxdiff) ? diff : maxdiff;
        LOG("%4d: 0x%08" PRIx64 "  [%11.8f] | 0x%08" PRIx64 "     [%11.8f]   | 0x%08" PRIx64 "  [%11.8f]\n", i, testout, testout / 9223372036854775808.0, golden, golden / 9223372036854775808.0, diff, diff / 9223372036854775808.0);
    }
    printf("MAXDIFF is 0x%08" PRIx64 "\n", maxdiff);
    return maxdiff;
}

typedef union
{
    int64_t d0;
    struct
    {
#if (defined(NDS32_EB) || defined(__NDS32_EB__))
        int32_t w1;
        uint32_t w0;
#else
        uint32_t w0;
        int32_t w1;
#endif
    } b32;
} union64_t;

static inline q63_t dump2_q63(q63_t* buf, q63_t* buf2, uint32_t size)
{
    unsigned int i;
    union64_t gtemp, otemp, maxtemp;
    q63_t maxdiff = 0;


    LOG("----- output ---- [floatingout] | golden ------ [floatinggolden] | ------ abs diff ------\n");
    for (i = 0; i < size; i++)
    {
        gtemp.d0 = *buf++;
        otemp.d0 = *buf2++;
        q63_t diff;

        if (gtemp.d0 > otemp.d0)
           diff = gtemp.d0 - otemp.d0;
        else
           diff = otemp.d0 - gtemp.d0;

        maxdiff = (diff > maxdiff) ? diff : maxdiff;
#ifdef PRINT_DETAIL
        union64_t dtemp;
        dtemp.d0 = diff;
        LOG("%4d: %08x %08x | %08x %08x | %08x %08x\n", i, (unsigned int)otemp.b32.w1,(int)otemp.b32.w0, (unsigned int)gtemp.b32.w1, (int)gtemp.b32.w0, (unsigned int)dtemp.b32.w1, (int)dtemp.b32.w0);
#endif
    }
    maxtemp.d0 = maxdiff;
    printf("MAXDIFF is 0x%08x %08x\n", (unsigned int)maxtemp.b32.w1, (unsigned int)(int)maxtemp.b32.w0);
    return maxdiff;
}

#define ERROR_EQ_ZERO (-1.0f)

static inline float32_t dump_rmsd2(float32_t *goldensig, float32_t *testsig, uint32_t size, float32_t result[])
{
    uint32_t i;
    double Sumsquareerr = 0.0f;
    float32_t Sumerr = 0.0f;
    float32_t RMSD;
    float32_t MAE;
    float32_t NRMSD;
    float32_t MAXDIFF = 0.0f;
    float32_t diff;
    float32_t SNR;
    double squaregloden = 0.0f;
    float32_t maxsig = 0.0f, minsig = 0.0f;
    int temp;
    int *test;

    for (i = 0; i < size; i++)
    {
        float32_t testin = *testsig++, golden = *goldensig++;
        test =   (int *)(&testin);
        temp =  *test;

        if (temp == 0x7FC00000)
        {
            return(-1.0f);
        }
        maxsig = (golden > maxsig) ? golden : maxsig;
        minsig = (golden < minsig) ? golden : minsig;
        diff  = fabsf(testin - golden);
        Sumerr += diff;

        MAXDIFF = (diff > MAXDIFF) ? diff : MAXDIFF;
        Sumsquareerr += diff * diff;
        squaregloden += golden * golden;
    }
    test =   (int *)(&Sumsquareerr);
    temp =  *test;

    if (temp == 0x7FC00000)
    {
        return(-1.0f);
    }

    MAE = Sumerr / size;
    RMSD = sqrt(Sumsquareerr / size);
    NRMSD = RMSD / (maxsig - minsig);
    if (Sumsquareerr == 0.0f)
    {
        SNR = ERROR_EQ_ZERO;
    }
    else
    {
        SNR = 10.0 * log10f(squaregloden / Sumsquareerr);
    }

    if (result != NULL)
    {
        result[0] = MAE;
        result[1] = RMSD;
        result[2] = MAXDIFF;
        result[3] = SNR;
    }
    else
    {
        printf("--------------------\n");
        printf("MAE is %5.10f, RMSD is %5.10f, NRMSD is %5.10f, MAXDIFF is %5.10f, SNR is %5.10f\n", MAE, RMSD, NRMSD, MAXDIFF, SNR);
    }

    return NRMSD;
}

#define dump_rmsd(go, te, s) dump_rmsd2(go, te, s, NULL);

// dump transform function diff with golden results.
// type: 1 for dct, cfft_rd2, cfft_rd4;  0 for dct4, rfft.
static inline float32_t dump2_goldendiff_q15(const char *hdr, float32_t *buf, q15_t *buf2, uint32_t size, uint8_t type)
{
    unsigned int i;
    float32_t maxdiff = 0.0;
    q15_t maxdiff_q15 = 0;
    float32_t testoutf32[size];
    float32_t golden_f32[size];

    LOG("----- output [scaleingout] | -- [golden] - | ---- abs diff ----\n");
    for (i = 0; i < size; i++)
    {
        golden_f32[i] = *buf++;
        q15_t testout = *buf2++;
        if (type == 0)
        {
            testoutf32[i] = testout / 32768.0 * (4 * size);
        }
        else
        {
            testoutf32[i] = testout / 32768.0 * (2 * size);
        }
        float32_t diff = golden_f32[i] - testoutf32[i];
        diff = fabsf(diff);
#ifdef PRINT_DETAIL
        q15_t diff_q15 = diff * 32768.0;
        LOG("%4d: 0x%04x [%11.8f] | [%11.8f] | 0x%04x [%2.8f]\n", i, testout & 0xffff, testoutf32[i], golden_f32[i], diff_q15 & 0xffff, diff);
#endif
        maxdiff = diff > maxdiff ? diff : maxdiff;
        maxdiff_q15 = (q15_t) (maxdiff * 32768.0);
    }
    printf("%s, maxdiff= 0x%04x [%2.8f]\n", hdr, maxdiff_q15, maxdiff);
    float32_t NRMSD;
    NRMSD = dump_rmsd(golden_f32, testoutf32, size);

    return NRMSD;
}
static inline float32_t dump2_goldendiff2_q15(const char *hdr, float32_t *buf, q15_t *buf2, uint32_t size, uint8_t type)
{
    unsigned int i;
    float32_t maxdiff = 0.0;
    q15_t maxdiff_q15 = 0;
    float32_t testoutf32[size];
    float32_t golden_f32[size];

    LOG("----- output [scaleingout] | -- [golden] - | ---- abs diff ----\n");
    for (i = 0; i < size; i++)
    {
        golden_f32[i] = *buf++;
        q15_t testout = *buf2++;
        if (type == 0)
        {
            testoutf32[i] = testout / 32768.0 * (2 * size);
        }
        else
        {
            testoutf32[i] = testout / 32768.0 * (size);
        }
        float32_t diff = golden_f32[i] - testoutf32[i];
        diff = fabsf(diff);
#ifdef PRINT_DETAIL
        q15_t diff_q15 = diff * 32768.0;
        LOG("%4d: 0x%04x [%11.8f] | [%11.8f] | 0x%04x [%2.8f]\n", i, testout & 0xffff, testoutf32[i], golden_f32[i], diff_q15 & 0xffff, diff);
#endif
        maxdiff = diff > maxdiff ? diff : maxdiff;
        maxdiff_q15 = (q15_t) (maxdiff * 32768.0);
    }
    printf("%s, maxdiff= 0x%04x [%2.8f]\n", hdr, maxdiff_q15, maxdiff);
    float32_t NRMSD;
    NRMSD = dump_rmsd(golden_f32, testoutf32, size);

    return NRMSD;
}

static inline float32_t dump2_inv_goldendiff2_q15(const char *hdr, float32_t *buf, q15_t *buf2, uint32_t size)
{
    unsigned int i;
    float32_t maxdiff = 0.0;
    q15_t maxdiff_q15 = 0;
    float32_t testoutf32[size];
    float32_t golden_f32[size];

    LOG("----- output [scaleingout] | -- [golden] - | ---- abs diff ----\n");
    for (i = 0; i < size; i++)
    {
        golden_f32[i] = *buf++;
        q15_t testout = *buf2++;
        testoutf32[i] = testout / 32768.0;
        float32_t diff = golden_f32[i] - testoutf32[i];
        diff = fabsf(diff);
#ifdef PRINT_DETAIL
        q15_t diff_q15 = diff * 32768.0;
        LOG("%4d: 0x%04x [%11.8f] | [%11.8f] | 0x%04x [%2.8f]\n", i, testout & 0xffff, testoutf32[i], golden_f32[i], diff_q15 & 0xffff, diff);
#endif
        maxdiff = diff > maxdiff ? diff : maxdiff;
        maxdiff_q15 = (q15_t) (maxdiff * 32768.0);
    }
    printf("%s, maxdiff= 0x%04x [%2.8f]\n", hdr, maxdiff_q15, maxdiff);

    float32_t NRMSD;
    NRMSD = dump_rmsd(golden_f32, testoutf32, size);

    return NRMSD;
}

// type: 1 for dct, cfft_rd2, cfft_rd4;  0 for dct4, rfft.

static inline float32_t dump2_inv_goldendiff_q15(const char *hdr, float32_t *buf, q15_t *buf2, uint32_t size)
{
    unsigned int i;
    float32_t maxdiff = 0.0;
    q15_t maxdiff_q15 = 0;
    float32_t testoutf32[size];
    float32_t golden_f32[size];

    LOG("----- output [scaleingout] | -- [golden] - | ---- abs diff ----\n");
    for (i = 0; i < size; i++)
    {
        golden_f32[i] = *buf++;
        q15_t testout = *buf2++;
        testoutf32[i] = testout / 32768.0 * 2;
        float32_t diff = golden_f32[i] - testoutf32[i];
        diff = fabsf(diff);
#ifdef PRINT_DETAIL
        q15_t diff_q15 = diff * 32768.0;
        LOG("%4d: 0x%04x [%11.8f] | [%11.8f] | 0x%04x [%2.8f]\n", i, testout & 0xffff, testoutf32[i], golden_f32[i], diff_q15 & 0xffff, diff);
#endif
        maxdiff = diff > maxdiff ? diff : maxdiff;
        maxdiff_q15 = (q15_t) (maxdiff * 32768.0);
    }
    printf("%s, maxdiff= 0x%04x [%2.8f]\n", hdr, maxdiff_q15, maxdiff);

    float32_t NRMSD;
    NRMSD = dump_rmsd(golden_f32, testoutf32, size);

    return NRMSD;
}

// type: 1 for dct, cfft_rd2, cfft_rd4;  0 for dct4, rfft.
static inline float32_t dump2_goldendiff_q31(const char *hdr, float32_t *buf, q31_t *buf2, uint32_t size, uint8_t type)
{
    unsigned int i;
    float32_t maxdiff = 0.0;
    q31_t maxdiff_q31 = 0;
    float32_t testoutf32[size];
    float32_t golden_f32[size];

    LOG("----- output --- [scaleingout] | -- [golden] - | ------ abs diff -------\n");
    for (i = 0; i < size; i++)
    {
        golden_f32[i] = *buf++;
        q31_t testout = *buf2++;
        if (type == 0)
        {
            testoutf32[i] = testout / 2147483648.0 * (4 * size);
        }
        else
        {
            testoutf32[i] = testout / 2147483648.0 * (2 * size);
        }
        float32_t diff = golden_f32[i] - testoutf32[i];
        diff = fabsf(diff);
#ifdef PRINT_DETAIL
        q31_t diff_q31 = diff * 2147483648.0;
        LOG("%4d: 0x%08" PRIx32 " [%11.8f] | [%11.8f] | 0x%08" PRIx32 " [%2.8f]\n", i, testout, testoutf32[i], golden_f32[i], diff_q31, diff);
#endif
        maxdiff = diff > maxdiff ? diff : maxdiff;
        maxdiff_q31 = (q15_t) (maxdiff * 2147483648.0);
    }
    printf("%s, maxdiff= 0x%08" PRIx32 " [%2.8f]\n", hdr, maxdiff_q31, maxdiff);
    float32_t NRMSD;
    NRMSD = dump_rmsd(golden_f32, testoutf32, size);

    return NRMSD;
}
static inline float32_t dump2_goldendiff2_q31(const char *hdr, float32_t *buf, q31_t *buf2, uint32_t size, uint8_t type)
{
    unsigned int i;
    float32_t maxdiff = 0.0;
    q31_t maxdiff_q31 = 0;
    float32_t testoutf32[size];
    float32_t golden_f32[size];

    LOG("----- output --- [scaleingout] | -- [golden] - | ------ abs diff -------\n");
    for (i = 0; i < size; i++)
    {
        golden_f32[i] = *buf++;
        q31_t testout = *buf2++;
        if (type == 0)
        {
            testoutf32[i] = testout / 2147483648.0 * (2 * size);
        }
        else
        {
            testoutf32[i] = testout / 2147483648.0 * (size);
        }
        float32_t diff = golden_f32[i] - testoutf32[i];
        diff = fabsf(diff);
#ifdef PRINT_DETAIL
        q31_t diff_q31 = diff * 2147483648.0;
        LOG("%4d: 0x%08" PRIx32 " [%11.8f] | [%11.8f] | 0x%08" PRIx32 " [%2.8f]\n", i, testout, testoutf32[i], golden_f32[i], diff_q31, diff);
#endif
        maxdiff = diff > maxdiff ? diff : maxdiff;
        maxdiff_q31 = (q15_t) (maxdiff * 2147483648.0);
    }
    printf("%s, maxdiff= 0x%08" PRIx32 " [%2.8f]\n", hdr, maxdiff_q31, maxdiff);
    float32_t NRMSD;
    NRMSD = dump_rmsd(golden_f32, testoutf32, size);

    return NRMSD;
}

static inline float32_t dump2_inv_goldendiff_q31(const char *hdr, float32_t *buf, q31_t *buf2, uint32_t size)
{
    unsigned int i;
    float32_t maxdiff = 0.0;
    q31_t maxdiff_q31 = 0;
    float32_t testoutf32[size];
    float32_t golden_f32[size];

    LOG("----- output --- [scaleingout] | -- [golden] - | ------ abs diff -------\n");
    for (i = 0; i < size; i++)
    {
        golden_f32[i] = *buf++;
        q31_t testout = *buf2++;
        testoutf32[i] = testout / 2147483648.0 * 2;
        float32_t diff = golden_f32[i] - testoutf32[i];
        diff = fabsf(diff);
#ifdef PRINT_DETAIL
        q31_t diff_q31 = diff * 2147483648.0;
        LOG("%4d: 0x%08" PRIx32 " [%11.8f] | [%11.8f] | 0x%08" PRIx32 " [%2.8f]\n", i, testout, testoutf32[i], golden_f32[i], diff_q31, diff);
#endif
        maxdiff = diff > maxdiff ? diff : maxdiff;
        maxdiff_q31 = (q31_t) (maxdiff * 2147483648.0);
    }
    printf("%s, maxdiff= 0x%08" PRIx32 " [%2.8f]\n", hdr, maxdiff_q31, maxdiff);
    float32_t NRMSD;
    NRMSD = dump_rmsd(golden_f32, testoutf32, size);

    return NRMSD;
}
static inline float32_t dump2_inv_goldendiff2_q31(const char *hdr, float32_t *buf, q31_t *buf2, uint32_t size)
{
    unsigned int i;
    float32_t maxdiff = 0.0;
    q31_t maxdiff_q31 = 0;
    float32_t testoutf32[size];
    float32_t golden_f32[size];

    LOG("----- output --- [scaleingout] | -- [golden] - | ------ abs diff -------\n");
    for (i = 0; i < size; i++)
    {
        golden_f32[i] = *buf++;
        q31_t testout = *buf2++;
        testoutf32[i] = testout / 2147483648.0;
        float32_t diff = golden_f32[i] - testoutf32[i];
        diff = fabsf(diff);
#ifdef PRINT_DETAIL
        q31_t diff_q31 = diff * 2147483648.0;
        LOG("%4d: 0x%08" PRIx32 " [%11.8f] | [%11.8f] | 0x%08" PRIx32 " [%2.8f]\n", i, testout, testoutf32[i], golden_f32[i], diff_q31, diff);
#endif
        maxdiff = diff > maxdiff ? diff : maxdiff;
        maxdiff_q31 = (q31_t) (maxdiff * 2147483648.0);
    }
    printf("%s, maxdiff= 0x%08" PRIx32 " [%2.8f]\n", hdr, maxdiff_q31, maxdiff);
    float32_t NRMSD;
    NRMSD = dump_rmsd(golden_f32, testoutf32, size);

    return NRMSD;
}

#define UNIT_Q63    (1.0f/9223372036854775808.0f)

static inline void _dsp_convert_q63_f64(q63_t *src, double *dst, uint32_t size)
{
    while (size != 0u)
    {
        *dst++ = (*src++) * UNIT_Q63;
        size--;
    }
}

static inline float32_t dump_rmsd64(double *goldensig, double *testsig, uint32_t size)
{
    uint32_t i;
    double Sumsquareerr = 0.0f;
    float32_t Sumerr = 0.0f;
    float32_t RMSD;
    float32_t MAE;
    float32_t NRMSD;
    float32_t MAXDIFF = 0.0f;
    float32_t diff;
    float32_t SNR;
    double squaregloden = 0.0f;
    float32_t maxsig = 0.0f, minsig = 0.0f;

    for (i = 0; i < size; i++)
    {
        double testin = *testsig++, golden = *goldensig++;
        maxsig = (golden > maxsig) ? golden : maxsig;
        minsig = (golden < minsig) ? golden : minsig;

        if (testin > golden)
           diff = testin - golden;
        else
           diff = golden - testin;

        Sumerr += diff;

        MAXDIFF = (diff > MAXDIFF) ? diff : MAXDIFF;
        Sumsquareerr += diff * diff;
        squaregloden += golden * golden;
    }
    MAE = Sumerr / size;
    RMSD = sqrt(Sumsquareerr / size);
    NRMSD = RMSD / (maxsig - minsig);
    if (Sumsquareerr == 0.0f)
    {
        SNR = ERROR_EQ_ZERO;
    }
    else
    {
        SNR = 10.0 * log10f(squaregloden / Sumsquareerr);
    }
    printf("--------------------\n");
    printf("MAE is %5.10f, RMSD is %5.10f, NRMSD is %5.10f, MAXDIFF is %5.10f, SNR is %5.5f\n", MAE, RMSD, NRMSD, MAXDIFF, SNR);

    return NRMSD;
}

#if defined (__riscv_zfh)
static inline float32_t dump_rmsd16(float16_t *goldensig, float16_t *testsig, uint32_t size)
{
    uint32_t i;
    double Sumsquareerr = 0.0f;
    float32_t Sumerr = 0.0f;
    float32_t RMSD;
    float32_t MAE;
    float32_t NRMSD;
    float32_t MAXDIFF = 0.0f;
    float32_t diff;
    float32_t SNR;
    double squaregloden = 0.0f;
    float32_t maxsig = 0.0f, minsig = 0.0f;

    for (i = 0; i < size; i++)
    {
        float16_t testin = *testsig++, golden = *goldensig++;
        maxsig = (golden > maxsig) ? golden : maxsig;
        minsig = (golden < minsig) ? golden : minsig;

        if (testin > golden)
           diff = testin - golden;
        else
           diff = golden - testin;

        Sumerr += diff;

        MAXDIFF = (diff > MAXDIFF) ? diff : MAXDIFF;
        Sumsquareerr += diff * diff;
        squaregloden += golden * golden;
    }
    MAE = Sumerr / size;
    RMSD = sqrt(Sumsquareerr / size);
    NRMSD = RMSD / (maxsig - minsig);
    if (Sumsquareerr == 0.0f)
    {
        SNR = ERROR_EQ_ZERO;
    }
    else
    {
        SNR = 10.0 * log10f(squaregloden / Sumsquareerr);
    }
    printf("--------------------\n");
    printf("MAE is %5.10f, RMSD is %5.10f, NRMSD is %5.10f, MAXDIFF is %5.10f, SNR is %5.10f\n", MAE, RMSD, NRMSD, MAXDIFF, SNR);

    return NRMSD;
}
#endif

static inline float32_t dump_rmsd_avg(float32_t *goldensig, float32_t *testsig, uint32_t size, uint32_t loop)
{
    uint32_t i;
    float32_t NRMSD = 0.0f;
    float32_t tmpNRMSD = 0.0f;
    float32_t tmpMAE = 0.0f;
    float32_t tmpRMSD = 0.0f;
    float32_t tmpMAXDIFF = 0.0f;
    float32_t tmpSNR = 0.0f;
    float32_t * now_testin = testsig;
    float32_t * now_golden = goldensig;

    for (i = 0; i < loop; i++)
    {
        float32_t now_result[4] = {0.0};

        tmpNRMSD = dump_rmsd2(now_golden, now_testin, size, now_result);
        NRMSD += tmpNRMSD;
        tmpMAE += now_result[0];
        tmpRMSD += now_result[1];
        tmpMAXDIFF += now_result[2];
        tmpSNR += now_result[3];

        now_testin += size;
        now_golden += size;
    }

    NRMSD /= loop;
    tmpMAE /= loop;
    tmpRMSD /= loop;
    tmpMAXDIFF /= loop;
    tmpSNR /= loop;

    printf("--------------------\n");
    printf("MAE is %5.10f, RMSD is %5.10f, NRMSD is %5.10f, MAXDIFF is %5.10f, SNR is %5.10f\n", tmpMAE, tmpRMSD, NRMSD, tmpMAXDIFF, tmpSNR);

    return NRMSD;
}

//-------------------------for uint8 funciton testing ---- need removed ------//
static inline void _dsp_convert_u8_f32(uint8_t *src, float *dst, uint32_t size)
{
    while (size != 0u)
    {
            *dst++ = (*src++) * UNIT_U8;
            size--;
    }
}

static inline void _dsp_convert_u16_f32(uint16_t *src, float *dst, uint32_t size)
{
    while (size != 0u)
    {
        *dst++ = (*src++) * UNIT_U16;
        size--;
    }
}

static inline void _dsp_convert_u32_f32(uint32_t *src, float *dst, uint32_t size)
{
    while (size != 0u)
    {
        *dst++ = (*src++) * UNIT_U32;
        size--;
    }
}

static inline void _dsp_convert_u64_f64(uint64_t *src, float64_t *dst, uint32_t size)
{
    while (size != 0u)
    {
        *dst++ = (*src++) * UNIT_U64;
        size--;
    }
}

// Test the sum diff of cfft_rd4
#if TRANSFORM_SAMPLES_LOG == 4
#define CIFFT_SUMOUT 0xfffc0
#elif TRANSFORM_SAMPLES_LOG == 6
#define CIFFT_SUMOUT 0x40ff00
#elif TRANSFORM_SAMPLES_LOG == 8
#define CIFFT_SUMOUT 0x100fc00
#elif TRANSFORM_SAMPLES_LOG == 10
#define CIFFT_SUMOUT 0x402f000
#else
#define CIFFT_SUMOUT 0x402f000
#endif
static inline int32_t _dsp_sumdiff_q15(q15_t *src, uint32_t size, q63_t golden)
{
    q63_t sum = 0;
    while (size != 0u)
    {
        sum += (unsigned short)(*src++);
        size--;
    }
    if (sum != golden)
    {
        printf("sum: 0x%08lx %08lx is different from golden: 0x%08lx %08lx\n", (unsigned long)(sum>>32), (unsigned long)(sum), (unsigned long)(golden>>32), (unsigned long)(golden));
        return -1;
    }
    return 0;
}

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

#define xstr(s) str(s)
#define str(s) #s
static inline uint32_t test_Fopen(char *fName, char *mode, FILE **fp)
{
#if 1  // move *.bin to bin/ folder.
    char path_buffer[1024];
    sprintf (path_buffer, "%s/bin/%s", xstr(GETENV_TESTDIR), fName);
    *fp = fopen(path_buffer, mode);
#else
    *fp = fopen(fName, mode);
#endif
    if(*fp == NULL)
    {
        printf("Error on openning %s!!!\n", fName);
        return -1;
    }
    return 0;
}


#endif
