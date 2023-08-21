#define SUCCESS (0L)
#define FAIL    (-1L)

typedef struct
{
    float32_t * mat;
    int row_size;
    int answer;
} struMat;

// ------------------------
// special testcase

float32_t matCase0[] =
{
    0.200000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000,
    0.000000, 0.200000, 0.000000, 0.000000, 0.000000, 0.000000,
    0.000000, 0.000000, 0.200000, 0.000000, 0.000000, 0.000000,
    0.000000, 0.000000, 0.000000, 0.568200, 0.465200, 0.000000,
    0.000000, 0.000000, 0.000000, 0.465200, 0.569200, 0.000000,
    0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.200000
};

float32_t matCase1[] =
{
    0.0, 1.0, 0.0,
    1.0, 0.0, 1.0,
    2.0, 0.0, 1.0
};


float32_t matCase2[] =
{
    1.0, 0.0, 0.0,
    0.0, 1.0, 0.0,
    0.0, 0.0, 1.0
};


float32_t matCase3[] =
{
    0.0, 0.0, 0.0,
    0.0, 1.0, 0.0,
    0.0, 0.0, 1.0
};


float32_t matCase4[] =
{
    2, 5,
    1, 3
};


float32_t matCase5[] =
{
    1, -1, 1,
    2, -1, 0,
    1, -2, 2
};


float32_t matCase6[] =
{
    1, 2,
    -1, 3
};


float32_t matCase7[] =
{
    1, 2, 3,
    1, 1, 2,
    0, 1, 2
};


float32_t matCase8[] =
{
    1, 0, 1,
    0, 2, 1,
    -1, 3, 1
};


float32_t matCase9[] =
{
    1, 2, 3,
    4, 5, 6,
    7, 8, 9
};


float32_t matCase10[] =
{
    2, 3, 4,
    2, 1, 1,
    -1, 1, 2
};


float32_t matCase11[] =
{
    2, 3,
    3, 5
};


float32_t matCase12[] =
{
    1, 1, 1,
    0, 2, 3,
    3, 2, 2
};


float32_t matCase13[] =
{
    1, 1, 1,
    0, 1, 1,
    1, 2, 1
};


float32_t matCase14[] =
{
    1, 1, 1, 1,
    0, 1, 1, 1,
    0, 0, 1, 3,
    0, 0, 1, 4
};


float32_t matCase15[] =
{
    0, 0, 0,
    0, 0, 0,
    0, 0, 0
};


float32_t matCase16[] =
{
    0, 3, 3,
    3, 0, 3,
    3, 3, 0
};


float32_t matCase17[] =
{
    3, 0, 0,
    3, 3, 0,
    3, 3, 3
};


float32_t matCase18[] =
{
    0, 0, 0,
    3, 3, 0,
    3, 3, 3
};


float32_t matCase19[] =
{
    0, 7, 7,
    0, 0, 7,
    0, 0, 0
};


float32_t matCase20[] =
{
    1, 0, 0, 1,
    1, -1, 0, 0,
    0, 0, 1, 1,
    0, 1, -1, 0
};


float32_t matCase21[] =
{
    1, 2, 3,
    2, 5, 3,
    1, 0, 8
};


float32_t matCase22[] =
{
    -1, 1, 16, 2,
    0, 0, 1, 4,
    0, 0, 1, 6,
    0, 1, 1, 2
};


float32_t matCase23[] =
{
    1, -3, 1, 2,
    1, -2, 2, 4,
    2, -8, -1, 0,
    3, -9, 4, 0
};


float32_t matCase24[] =
{
    1, 0, -2, 3,
    2, 2, 0, 4,
    2, 0, -4, 6,
    1, 1, 1, 1
};


float32_t matCase25[] =
{
    3, 5, -4,
    -3, -2, 4,
    6, 1, -8
};


float32_t matCase26[] =
{
    1, 2, -7,
    -2, -3, 9,
    0, -2, 10
};


float32_t matCase27[] =
{
    -1, -2, 2,
    2, 1, -1,
    3, -4, 2
};


float32_t matCase28[] =
{
    0, 1, 0,
    1, 0, 0,
    0, 0, 1
};


float32_t matCase29[] =
{
    8, 1, 5,
    2, -7, -1,
    3, 4, 1
};


float32_t matCase30[] =
{
    3, 4, 1,
    2, -7, -1,
    8, 1, 5
};


float32_t matCase31[] =
{
    0, 1, 1, 1,
    1, 1, 1, 1,
    1, 1, 0, 1,
    0, 0, 0, 1
};


float32_t matCase32[] =
{
    0, 1, 1, 1,
    1, 1, 1, 1,
    1, 1, 0, 1,
    0, 0, 0, 0
};


float32_t matCase33[] =
{
    0, 1, 1, 1,
    1, 1, 1, 1,
    1, 1, 0, 1,
    0, 0, 1, 0
};


float32_t matCase34[] =
{
    0, 2, 3, 4,
    5, 12, 2, 5,
    1, 1, 0, 1,
    7, 1, 0, 0
};

struMat matArray[] =
{
    {matCase0, 6, SUCCESS},
    {matCase1, 3, SUCCESS},
    {matCase2, 3, SUCCESS},
    {matCase3, 3, FAIL},
    {matCase4, 2, SUCCESS},
    {matCase5, 3, SUCCESS},
    {matCase6, 2, SUCCESS},
    {matCase7, 3, SUCCESS},
    {matCase8, 3, SUCCESS},
    {matCase9, 3, FAIL},
    {matCase10, 3, SUCCESS},
    {matCase11, 2, SUCCESS},
    {matCase12, 3, SUCCESS},
    {matCase13, 3, SUCCESS},
    {matCase14, 4, SUCCESS},
    {matCase15, 3, FAIL},
    {matCase16, 3, SUCCESS},
    {matCase17, 3, SUCCESS},
    {matCase18, 3, FAIL},
    {matCase19, 3, FAIL},
    {matCase20, 4, FAIL},
    {matCase21, 3, SUCCESS},
    {matCase22, 4, SUCCESS},
    {matCase23, 4, SUCCESS},
    {matCase24, 4, FAIL},
    //{matCase25, 3, FAIL},
    {matCase26, 3, FAIL},
    {matCase27, 3, SUCCESS},
    {matCase28, 3, SUCCESS},
    {matCase29, 3, SUCCESS},
    {matCase30, 3, SUCCESS},
    {matCase31, 4, SUCCESS},
    {matCase32, 4, FAIL},
    {matCase33, 4, FAIL},
    {matCase34, 4, SUCCESS},
    {matCase34, 0, FAIL},
};


// ----------------------------------------
// matrix_3by3

//Matrix 0
//
float32_t matCase3by3_0[] = {
    0, 0, 0,
    0, 0, 0,
    0, 0, 0
};

//Matrix 1
//1
float32_t matCase3by3_1[] = {
    1, 0, 0,
    0, 0, 0,
    0, 0, 0
};

//Matrix 2
//2
float32_t matCase3by3_2[] = {
    0, 1, 0,
    0, 0, 0,
    0, 0, 0
};

//Matrix 3
//3
float32_t matCase3by3_3[] = {
    0, 0, 1,
    0, 0, 0,
    0, 0, 0
};

//Matrix 4
//4
float32_t matCase3by3_4[] = {
    0, 0, 0,
    1, 0, 0,
    0, 0, 0
};

//Matrix 5
//5
float32_t matCase3by3_5[] = {
    0, 0, 0,
    0, 1, 0,
    0, 0, 0
};

//Matrix 6
//6
float32_t matCase3by3_6[] = {
    0, 0, 0,
    0, 0, 1,
    0, 0, 0
};

//Matrix 7
//7
float32_t matCase3by3_7[] = {
    0, 0, 0,
    0, 0, 0,
    1, 0, 0
};

//Matrix 8
//8
float32_t matCase3by3_8[] = {
    0, 0, 0,
    0, 0, 0,
    0, 1, 0,
};

//Matrix 9
//9
float32_t matCase3by3_9[] = {
    0, 0, 0,
    0, 0, 0,
    0, 0, 1,
};

//Matrix 10
//1 2
float32_t matCase3by3_10[] = {
    1, 1, 0,
    0, 0, 0,
    0, 0, 0,
};

//Matrix 11
//1 3
float32_t matCase3by3_11[] = {
    1, 0, 1,
    0, 0, 0,
    0, 0, 0,
};

//Matrix 12
//1 4
float32_t matCase3by3_12[] = {
    1, 0, 0,
    1, 0, 0,
    0, 0, 0,
};

//Matrix 13
//1 5
float32_t matCase3by3_13[] = {
    1, 0, 0,
    0, 1, 0,
    0, 0, 0,
};

//Matrix 14
//1 6
float32_t matCase3by3_14[] = {
    1, 0, 0,
    0, 0, 1,
    0, 0, 0,
};

//Matrix 15
//1 7
float32_t matCase3by3_15[] = {
    1, 0, 0,
    0, 0, 0,
    1, 0, 0,
};

//Matrix 16
//1 8
float32_t matCase3by3_16[] = {
    1, 0, 0,
    0, 0, 0,
    0, 1, 0,
};

//Matrix 17
//1 9
float32_t matCase3by3_17[] = {
    1, 0, 0,
    0, 0, 0,
    0, 0, 1,
};

//Matrix 18
//2 3
float32_t matCase3by3_18[] = {
    0, 1, 1,
    0, 0, 0,
    0, 0, 0,
};

//Matrix 19
//2 4
float32_t matCase3by3_19[] = {
    0, 1, 0,
    1, 0, 0,
    0, 0, 0,
};

//Matrix 20
//2 5
float32_t matCase3by3_20[] = {
    0, 1, 0,
    0, 1, 0,
    0, 0, 0,
};

//Matrix 21
//2 6
float32_t matCase3by3_21[] = {
    0, 1, 0,
    0, 0, 1,
    0, 0, 0,
};

//Matrix 22
//2 7
float32_t matCase3by3_22[] = {
    0, 1, 0,
    0, 0, 0,
    1, 0, 0,
};

//Matrix 23
//2 8
float32_t matCase3by3_23[] = {
    0, 1, 0,
    0, 0, 0,
    0, 1, 0,
};

//Matrix 24
//2 9
float32_t matCase3by3_24[] = {
    0, 1, 0,
    0, 0, 0,
    0, 0, 1,
};

//Matrix 25
//3 4
float32_t matCase3by3_25[] = {
    0, 0, 1,
    1, 0, 0,
    0, 0, 0,
};

//Matrix 26
//3 5
float32_t matCase3by3_26[] = {
    0, 0, 1,
    0, 1, 0,
    0, 0, 0,
};

//Matrix 27
//3 6
float32_t matCase3by3_27[] = {
    0, 0, 1,
    0, 0, 1,
    0, 0, 0,
};

//Matrix 28
//3 7
float32_t matCase3by3_28[] = {
    0, 0, 1,
    0, 0, 0,
    1, 0, 0,
};

//Matrix 29
//3 8
float32_t matCase3by3_29[] = {
    0, 0, 1,
    0, 0, 0,
    0, 1, 0,
};

//Matrix 30
//3 9
float32_t matCase3by3_30[] = {
    0, 0, 1,
    0, 0, 0,
    0, 0, 1,
};

//Matrix 31
//4 5
float32_t matCase3by3_31[] = {
    0, 0, 0,
    1, 1, 0,
    0, 0, 0,
};

//Matrix 32
//4 6
float32_t matCase3by3_32[] = {
    0, 0, 0,
    1, 0, 1,
    0, 0, 0,
};

//Matrix 33
//4 7
float32_t matCase3by3_33[] = {
    0, 0, 0,
    1, 0, 0,
    1, 0, 0,
};

//Matrix 34
//4 8
float32_t matCase3by3_34[] = {
    0, 0, 0,
    1, 0, 0,
    0, 1, 0,
};

//Matrix 35
//4 9
float32_t matCase3by3_35[] = {
    0, 0, 0,
    1, 0, 0,
    0, 0, 1,
};

//Matrix 36
//5 6
float32_t matCase3by3_36[] = {
    0, 0, 0,
    0, 1, 1,
    0, 0, 0,
};

//Matrix 37
//5 7
float32_t matCase3by3_37[] = {
    0, 0, 0,
    0, 1, 0,
    1, 0, 0,
};

//Matrix 38
//5 8
float32_t matCase3by3_38[] = {
    0, 0, 0,
    0, 1, 0,
    0, 1, 0,
};

//Matrix 39
//5 9
float32_t matCase3by3_39[] = {
    0, 0, 0,
    0, 1, 0,
    0, 0, 1,
};

//Matrix 40
//6 7
float32_t matCase3by3_40[] = {
    0, 0, 0,
    0, 0, 1,
    1, 0, 0,
};

//Matrix 41
//6 8
float32_t matCase3by3_41[] = {
    0, 0, 0,
    0, 0, 1,
    0, 1, 0,
};

//Matrix 42
//6 9
float32_t matCase3by3_42[] = {
    0, 0, 0,
    0, 0, 1,
    0, 0, 1,
};

//Matrix 43
//7 8
float32_t matCase3by3_43[] = {
    0, 0, 0,
    0, 0, 0,
    1, 1, 0,
};

//Matrix 44
//7 9
float32_t matCase3by3_44[] = {
    0, 0, 0,
    0, 0, 0,
    1, 0, 1,
};

//Matrix 45
//8 9
float32_t matCase3by3_45[] = {
    0, 0, 0,
    0, 0, 0,
    0, 1, 1,
};

//Matrix 46
//1 2 3
float32_t matCase3by3_46[] = {
    1, 1, 1,
    0, 0, 0,
    0, 0, 0,
};

//Matrix 47
//1 2 4
float32_t matCase3by3_47[] = {
    1, 1, 0,
    1, 0, 0,
    0, 0, 0,
};

//Matrix 48
//1 2 5
float32_t matCase3by3_48[] = {
    1, 1, 0,
    0, 1, 0,
    0, 0, 0,
};

//Matrix 49
//1 2 6
float32_t matCase3by3_49[] = {
    1, 1, 0,
    0, 0, 1,
    0, 0, 0,
};

//Matrix 50
//1 2 7
float32_t matCase3by3_50[] = {
    1, 1, 0,
    0, 0, 0,
    1, 0, 0,
};

//Matrix 51
//1 2 8
float32_t matCase3by3_51[] = {
    1, 1, 0,
    0, 0, 0,
    0, 1, 0,
};

//Matrix 52
//1 2 9
float32_t matCase3by3_52[] = {
    1, 1, 0,
    0, 0, 0,
    0, 0, 1,
};

//Matrix 53
//1 3 4
float32_t matCase3by3_53[] = {
    1, 0, 1,
    1, 0, 0,
    0, 0, 0,
};

//Matrix 54
//1 3 5
float32_t matCase3by3_54[] = {
    1, 0, 1,
    0, 1, 0,
    0, 0, 0,
};

//Matrix 55
//1 3 6
float32_t matCase3by3_55[] = {
    1, 0, 1,
    0, 0, 1,
    0, 0, 0,
};

//Matrix 56
//1 3 7
float32_t matCase3by3_56[] = {
    1, 0, 1,
    0, 0, 0,
    1, 0, 0,
};

//Matrix 57
//1 3 8
float32_t matCase3by3_57[] = {
    1, 0, 1,
    0, 0, 0,
    0, 1, 0,
};

//Matrix 58
//1 3 9
float32_t matCase3by3_58[] = {
    1, 0, 1,
    0, 0, 0,
    0, 0, 1,
};

//Matrix 59
//1 4 5
float32_t matCase3by3_59[] = {
    1, 0, 0,
    1, 1, 0,
    0, 0, 0,
};

//Matrix 60
//1 4 6
float32_t matCase3by3_60[] = {
    1, 0, 0,
    1, 0, 1,
    0, 0, 0,
};

//Matrix 61
//1 4 7
float32_t matCase3by3_61[] = {
    1, 0, 0,
    1, 0, 0,
    1, 0, 0,
};

//Matrix 62
//1 4 8
float32_t matCase3by3_62[] = {
    1, 0, 0,
    1, 0, 0,
    0, 1, 0,
};

//Matrix 63
//1 4 9
float32_t matCase3by3_63[] = {
    1, 0, 0,
    1, 0, 0,
    0, 0, 1,
};

//Matrix 64
//1 5 6
float32_t matCase3by3_64[] = {
    1, 0, 0,
    0, 1, 1,
    0, 0, 0,
};

//Matrix 65
//1 5 7
float32_t matCase3by3_65[] = {
    1, 0, 0,
    0, 1, 0,
    1, 0, 0,
};

//Matrix 66
//1 5 8
float32_t matCase3by3_66[] = {
    1, 0, 0,
    0, 1, 0,
    0, 1, 0,
};

//Matrix 67
//1 5 9
float32_t matCase3by3_67[] = {
    1, 0, 0,
    0, 1, 0,
    0, 0, 1,
};

//Matrix 68
//1 6 7
float32_t matCase3by3_68[] = {
    1, 0, 0,
    0, 0, 1,
    1, 0, 0,
};

//Matrix 69
//1 6 8
float32_t matCase3by3_69[] = {
    1, 0, 0,
    0, 0, 1,
    0, 1, 0,
};

//Matrix 70
//1 6 9
float32_t matCase3by3_70[] = {
    1, 0, 0,
    0, 0, 1,
    0, 0, 1,
};

//Matrix 71
//1 7 8
float32_t matCase3by3_71[] = {
    1, 0, 0,
    0, 0, 0,
    1, 1, 0,
};

//Matrix 72
//1 7 9
float32_t matCase3by3_72[] = {
    1, 0, 0,
    0, 0, 0,
    1, 0, 1,
};

//Matrix 73
//1 8 9
float32_t matCase3by3_73[] = {
    1, 0, 0,
    0, 0, 0,
    0, 1, 1,
};

//Matrix 74
//2 3 4
float32_t matCase3by3_74[] = {
    0, 1, 1,
    1, 0, 0,
    0, 0, 0,
};

//Matrix 75
//2 3 5
float32_t matCase3by3_75[] = {
    0, 1, 1,
    0, 1, 0,
    0, 0, 0,
};

//Matrix 76
//2 3 6
float32_t matCase3by3_76[] = {
    0, 1, 1,
    0, 0, 1,
    0, 0, 0,
};

//Matrix 77
//2 3 7
float32_t matCase3by3_77[] = {
    0, 1, 1,
    0, 0, 0,
    1, 0, 0,
};

//Matrix 78
//2 3 8
float32_t matCase3by3_78[] = {
    0, 1, 1,
    0, 0, 0,
    0, 1, 0,
};

//Matrix 79
//2 3 9
float32_t matCase3by3_79[] = {
    0, 1, 1,
    0, 0, 0,
    0, 0, 1,
};

//Matrix 80
//2 4 5
float32_t matCase3by3_80[] = {
    0, 1, 0,
    1, 1, 0,
    0, 0, 0,
};

//Matrix 81
//2 4 6
float32_t matCase3by3_81[] = {
    0, 1, 0,
    1, 0, 1,
    0, 0, 0,
};

//Matrix 82
//2 4 7
float32_t matCase3by3_82[] = {
    0, 1, 0,
    1, 0, 0,
    1, 0, 0,
};

//Matrix 83
//2 4 8
float32_t matCase3by3_83[] = {
    0, 1, 0,
    1, 0, 0,
    0, 1, 0,
};

//Matrix 84
//2 4 9
float32_t matCase3by3_84[] = {
    0, 1, 0,
    1, 0, 0,
    0, 0, 1,
};

//Matrix 85
//2 5 6
float32_t matCase3by3_85[] = {
    0, 1, 0,
    0, 1, 1,
    0, 0, 0,
};

//Matrix 86
//2 5 7
float32_t matCase3by3_86[] = {
    0, 1, 0,
    0, 1, 0,
    1, 0, 0,
};

//Matrix 87
//2 5 8
float32_t matCase3by3_87[] = {
    0, 1, 0,
    0, 1, 0,
    0, 1, 0,
};

//Matrix 88
//2 5 9
float32_t matCase3by3_88[] = {
    0, 1, 0,
    0, 1, 0,
    0, 0, 1,
};

//Matrix 89
//2 6 7
float32_t matCase3by3_89[] = {
    0, 1, 0,
    0, 0, 1,
    1, 0, 0,
};

//Matrix 90
//2 6 8
float32_t matCase3by3_90[] = {
    0, 1, 0,
    0, 0, 1,
    0, 1, 0,
};

//Matrix 91
//2 6 9
float32_t matCase3by3_91[] = {
    0, 1, 0,
    0, 0, 1,
    0, 0, 1,
};

//Matrix 92
//2 7 8
float32_t matCase3by3_92[] = {
    0, 1, 0,
    0, 0, 0,
    1, 1, 0,
};

//Matrix 93
//2 7 9
float32_t matCase3by3_93[] = {
    0, 1, 0,
    0, 0, 0,
    1, 0, 1,
};

//Matrix 94
//2 8 9
float32_t matCase3by3_94[] = {
    0, 1, 0,
    0, 0, 0,
    0, 1, 1,
};

//Matrix 95
//3 4 5
float32_t matCase3by3_95[] = {
    0, 0, 1,
    1, 1, 0,
    0, 0, 0,
};

//Matrix 96
//3 4 6
float32_t matCase3by3_96[] = {
    0, 0, 1,
    1, 0, 1,
    0, 0, 0,
};

//Matrix 97
//3 4 7
float32_t matCase3by3_97[] = {
    0, 0, 1,
    1, 0, 0,
    1, 0, 0,
};

//Matrix 98
//3 4 8
float32_t matCase3by3_98[] = {
    0, 0, 1,
    1, 0, 0,
    0, 1, 0,
};

//Matrix 99
//3 4 9
float32_t matCase3by3_99[] = {
    0, 0, 1,
    1, 0, 0,
    0, 0, 1,
};

//Matrix 100
//3 5 6
float32_t matCase3by3_100[] = {
    0, 0, 1,
    0, 1, 1,
    0, 0, 0,
};

//Matrix 101
//3 5 7
float32_t matCase3by3_101[] = {
    0, 0, 1,
    0, 1, 0,
    1, 0, 0,
};

//Matrix 102
//3 5 8
float32_t matCase3by3_102[] = {
    0, 0, 1,
    0, 1, 0,
    0, 1, 0,
};

//Matrix 103
//3 5 9
float32_t matCase3by3_103[] = {
    0, 0, 1,
    0, 1, 0,
    0, 0, 1,
};

//Matrix 104
//3 6 7
float32_t matCase3by3_104[] = {
    0, 0, 1,
    0, 0, 1,
    1, 0, 0,
};

//Matrix 105
//3 6 8
float32_t matCase3by3_105[] = {
    0, 0, 1,
    0, 0, 1,
    0, 1, 0,
};

//Matrix 106
//3 6 9
float32_t matCase3by3_106[] = {
    0, 0, 1,
    0, 0, 1,
    0, 0, 1,
};

//Matrix 107
//3 7 8
float32_t matCase3by3_107[] = {
    0, 0, 1,
    0, 0, 0,
    1, 1, 0,
};

//Matrix 108
//3 7 9
float32_t matCase3by3_108[] = {
    0, 0, 1,
    0, 0, 0,
    1, 0, 1,
};

//Matrix 109
//3 8 9
float32_t matCase3by3_109[] = {
    0, 0, 1,
    0, 0, 0,
    0, 1, 1,
};

//Matrix 110
//4 5 6
float32_t matCase3by3_110[] = {
    0, 0, 0,
    1, 1, 1,
    0, 0, 0,
};

//Matrix 111
//4 5 7
float32_t matCase3by3_111[] = {
    0, 0, 0,
    1, 1, 0,
    1, 0, 0,
};

//Matrix 112
//4 5 8
float32_t matCase3by3_112[] = {
    0, 0, 0,
    1, 1, 0,
    0, 1, 0,
};

//Matrix 113
//4 5 9
float32_t matCase3by3_113[] = {
    0, 0, 0,
    1, 1, 0,
    0, 0, 1,
};

//Matrix 114
//4 6 7
float32_t matCase3by3_114[] = {
    0, 0, 0,
    1, 0, 1,
    1, 0, 0,
};

//Matrix 115
//4 6 8
float32_t matCase3by3_115[] = {
    0, 0, 0,
    1, 0, 1,
    0, 1, 0,
};

//Matrix 116
//4 6 9
float32_t matCase3by3_116[] = {
    0, 0, 0,
    1, 0, 1,
    0, 0, 1,
};

//Matrix 117
//4 7 8
float32_t matCase3by3_117[] = {
    0, 0, 0,
    1, 0, 0,
    1, 1, 0,
};

//Matrix 118
//4 7 9
float32_t matCase3by3_118[] = {
    0, 0, 0,
    1, 0, 0,
    1, 0, 1,
};

//Matrix 119
//4 8 9
float32_t matCase3by3_119[] = {
    0, 0, 0,
    1, 0, 0,
    0, 1, 1,
};

//Matrix 120
//5 6 7
float32_t matCase3by3_120[] = {
    0, 0, 0,
    0, 1, 1,
    1, 0, 0,
};

//Matrix 121
//5 6 8
float32_t matCase3by3_121[] = {
    0, 0, 0,
    0, 1, 1,
    0, 1, 0,
};

//Matrix 122
//5 6 9
float32_t matCase3by3_122[] = {
    0, 0, 0,
    0, 1, 1,
    0, 0, 1,
};

//Matrix 123
//5 7 8
float32_t matCase3by3_123[] = {
    0, 0, 0,
    0, 1, 0,
    1, 1, 0,
};

//Matrix 124
//5 7 9
float32_t matCase3by3_124[] = {
    0, 0, 0,
    0, 1, 0,
    1, 0, 1,
};

//Matrix 125
//5 8 9
float32_t matCase3by3_125[] = {
    0, 0, 0,
    0, 1, 0,
    0, 1, 1,
};

//Matrix 126
//6 7 8
float32_t matCase3by3_126[] = {
    0, 0, 0,
    0, 0, 1,
    1, 1, 0,
};

//Matrix 127
//6 7 9
float32_t matCase3by3_127[] = {
    0, 0, 0,
    0, 0, 1,
    1, 0, 1,
};

//Matrix 128
//6 8 9
float32_t matCase3by3_128[] = {
    0, 0, 0,
    0, 0, 1,
    0, 1, 1,
};

//Matrix 129
//7 8 9
float32_t matCase3by3_129[] = {
    0, 0, 0,
    0, 0, 0,
    1, 1, 1,
};

//Matrix 130
//1 2 3 4
float32_t matCase3by3_130[] = {
    1, 1, 1,
    1, 0, 0,
    0, 0, 0,
};

//Matrix 131
//1 2 3 5
float32_t matCase3by3_131[] = {
    1, 1, 1,
    0, 1, 0,
    0, 0, 0,
};

//Matrix 132
//1 2 3 6
float32_t matCase3by3_132[] = {
    1, 1, 1,
    0, 0, 1,
    0, 0, 0,
};

//Matrix 133
//1 2 3 7
float32_t matCase3by3_133[] = {
    1, 1, 1,
    0, 0, 0,
    1, 0, 0,
};

//Matrix 134
//1 2 3 8
float32_t matCase3by3_134[] = {
    1, 1, 1,
    0, 0, 0,
    0, 1, 0,
};

//Matrix 135
//1 2 3 9
float32_t matCase3by3_135[] = {
    1, 1, 1,
    0, 0, 0,
    0, 0, 1,
};

//Matrix 136
//1 2 4 5
float32_t matCase3by3_136[] = {
    1, 1, 0,
    1, 1, 0,
    0, 0, 0,
};

//Matrix 137
//1 2 4 6
float32_t matCase3by3_137[] = {
    1, 1, 0,
    1, 0, 1,
    0, 0, 0,
};

//Matrix 138
//1 2 4 7
float32_t matCase3by3_138[] = {
    1, 1, 0,
    1, 0, 0,
    1, 0, 0,
};

//Matrix 139
//1 2 4 8
float32_t matCase3by3_139[] = {
    1, 1, 0,
    1, 0, 0,
    0, 1, 0,
};

//Matrix 140
//1 2 4 9
float32_t matCase3by3_140[] = {
    1, 1, 0,
    1, 0, 0,
    0, 0, 1,
};

//Matrix 141
//1 2 5 6
float32_t matCase3by3_141[] = {
    1, 1, 0,
    0, 1, 1,
    0, 0, 0,
};

//Matrix 142
//1 2 5 7
float32_t matCase3by3_142[] = {
    1, 1, 0,
    0, 1, 0,
    1, 0, 0,
};

//Matrix 143
//1 2 5 8
float32_t matCase3by3_143[] = {
    1, 1, 0,
    0, 1, 0,
    0, 1, 0,
};

//Matrix 144
//1 2 5 9
float32_t matCase3by3_144[] = {
    1, 1, 0,
    0, 1, 0,
    0, 0, 1,
};

//Matrix 145
//1 2 6 7
float32_t matCase3by3_145[] = {
    1, 1, 0,
    0, 0, 1,
    1, 0, 0,
};

//Matrix 146
//1 2 6 8
float32_t matCase3by3_146[] = {
    1, 1, 0,
    0, 0, 1,
    0, 1, 0,
};

//Matrix 147
//1 2 6 9
float32_t matCase3by3_147[] = {
    1, 1, 0,
    0, 0, 1,
    0, 0, 1,
};

//Matrix 148
//1 2 7 8
float32_t matCase3by3_148[] = {
    1, 1, 0,
    0, 0, 0,
    1, 1, 0,
};

//Matrix 149
//1 2 7 9
float32_t matCase3by3_149[] = {
    1, 1, 0,
    0, 0, 0,
    1, 0, 1,
};

//Matrix 150
//1 2 8 9
float32_t matCase3by3_150[] = {
    1, 1, 0,
    0, 0, 0,
    0, 1, 1,
};

//Matrix 151
//1 3 4 5
float32_t matCase3by3_151[] = {
    1, 0, 1,
    1, 1, 0,
    0, 0, 0,
};

//Matrix 152
//1 3 4 6
float32_t matCase3by3_152[] = {
    1, 0, 1,
    1, 0, 1,
    0, 0, 0,
};

//Matrix 153
//1 3 4 7
float32_t matCase3by3_153[] = {
    1, 0, 1,
    1, 0, 0,
    1, 0, 0,
};

//Matrix 154
//1 3 4 8
float32_t matCase3by3_154[] = {
    1, 0, 1,
    1, 0, 0,
    0, 1, 0,
};

//Matrix 155
//1 3 4 9
float32_t matCase3by3_155[] = {
    1, 0, 1,
    1, 0, 0,
    0, 0, 1,
};

//Matrix 156
//1 3 5 6
float32_t matCase3by3_156[] = {
    1, 0, 1,
    0, 1, 1,
    0, 0, 0,
};

//Matrix 157
//1 3 5 7
float32_t matCase3by3_157[] = {
    1, 0, 1,
    0, 1, 0,
    1, 0, 0,
};

//Matrix 158
//1 3 5 8
float32_t matCase3by3_158[] = {
    1, 0, 1,
    0, 1, 0,
    0, 1, 0,
};

//Matrix 159
//1 3 5 9
float32_t matCase3by3_159[] = {
    1, 0, 1,
    0, 1, 0,
    0, 0, 1,
};

//Matrix 160
//1 3 6 7
float32_t matCase3by3_160[] = {
    1, 0, 1,
    0, 0, 1,
    1, 0, 0,
};

//Matrix 161
//1 3 6 8
float32_t matCase3by3_161[] = {
    1, 0, 1,
    0, 0, 1,
    0, 1, 0,
};

//Matrix 162
//1 3 6 9
float32_t matCase3by3_162[] = {
    1, 0, 1,
    0, 0, 1,
    0, 0, 1,
};

//Matrix 163
//1 3 7 8
float32_t matCase3by3_163[] = {
    1, 0, 1,
    0, 0, 0,
    1, 1, 0,
};

//Matrix 164
//1 3 7 9
float32_t matCase3by3_164[] = {
    1, 0, 1,
    0, 0, 0,
    1, 0, 1,
};

//Matrix 165
//1 3 8 9
float32_t matCase3by3_165[] = {
    1, 0, 1,
    0, 0, 0,
    0, 1, 1,
};

//Matrix 166
//1 4 5 6
float32_t matCase3by3_166[] = {
    1, 0, 0,
    1, 1, 1,
    0, 0, 0,
};

//Matrix 167
//1 4 5 7
float32_t matCase3by3_167[] = {
    1, 0, 0,
    1, 1, 0,
    1, 0, 0,
};

//Matrix 168
//1 4 5 8
float32_t matCase3by3_168[] = {
    1, 0, 0,
    1, 1, 0,
    0, 1, 0,
};

//Matrix 169
//1 4 5 9
float32_t matCase3by3_169[] = {
    1, 0, 0,
    1, 1, 0,
    0, 0, 1,
};

//Matrix 170
//1 4 6 7
float32_t matCase3by3_170[] = {
    1, 0, 0,
    1, 0, 1,
    1, 0, 0,
};

//Matrix 171
//1 4 6 8
float32_t matCase3by3_171[] = {
    1, 0, 0,
    1, 0, 1,
    0, 1, 0,
};

//Matrix 172
//1 4 6 9
float32_t matCase3by3_172[] = {
    1, 0, 0,
    1, 0, 1,
    0, 0, 1,
};

//Matrix 173
//1 4 7 8
float32_t matCase3by3_173[] = {
    1, 0, 0,
    1, 0, 0,
    1, 1, 0,
};

//Matrix 174
//1 4 7 9
float32_t matCase3by3_174[] = {
    1, 0, 0,
    1, 0, 0,
    1, 0, 1,
};

//Matrix 175
//1 4 8 9
float32_t matCase3by3_175[] = {
    1, 0, 0,
    1, 0, 0,
    0, 1, 1,
};

//Matrix 176
//1 5 6 7
float32_t matCase3by3_176[] = {
    1, 0, 0,
    0, 1, 1,
    1, 0, 0,
};

//Matrix 177
//1 5 6 8
float32_t matCase3by3_177[] = {
    1, 0, 0,
    0, 1, 1,
    0, 1, 0,
};

//Matrix 178
//1 5 6 9
float32_t matCase3by3_178[] = {
    1, 0, 0,
    0, 1, 1,
    0, 0, 1,
};

//Matrix 179
//1 5 7 8
float32_t matCase3by3_179[] = {
    1, 0, 0,
    0, 1, 0,
    1, 1, 0,
};

//Matrix 180
//1 5 7 9
float32_t matCase3by3_180[] = {
    1, 0, 0,
    0, 1, 0,
    1, 0, 1,
};

//Matrix 181
//1 5 8 9
float32_t matCase3by3_181[] = {
    1, 0, 0,
    0, 1, 0,
    0, 1, 1,
};

//Matrix 182
//1 6 7 8
float32_t matCase3by3_182[] = {
    1, 0, 0,
    0, 0, 1,
    1, 1, 0,
};

//Matrix 183
//1 6 7 9
float32_t matCase3by3_183[] = {
    1, 0, 0,
    0, 0, 1,
    1, 0, 1,
};

//Matrix 184
//1 6 8 9
float32_t matCase3by3_184[] = {
    1, 0, 0,
    0, 0, 1,
    0, 1, 1,
};

//Matrix 185
//1 7 8 9
float32_t matCase3by3_185[] = {
    1, 0, 0,
    0, 0, 0,
    1, 1, 1,
};

//Matrix 186
//2 3 4 5
float32_t matCase3by3_186[] = {
    0, 1, 1,
    1, 1, 0,
    0, 0, 0,
};

//Matrix 187
//2 3 4 6
float32_t matCase3by3_187[] = {
    0, 1, 1,
    1, 0, 1,
    0, 0, 0,
};

//Matrix 188
//2 3 4 7
float32_t matCase3by3_188[] = {
    0, 1, 1,
    1, 0, 0,
    1, 0, 0,
};

//Matrix 189
//2 3 4 8
float32_t matCase3by3_189[] = {
    0, 1, 1,
    1, 0, 0,
    0, 1, 0,
};

//Matrix 190
//2 3 4 9
float32_t matCase3by3_190[] = {
    0, 1, 1,
    1, 0, 0,
    0, 0, 1,
};

//Matrix 191
//2 3 5 6
float32_t matCase3by3_191[] = {
    0, 1, 1,
    0, 1, 1,
    0, 0, 0,
};

//Matrix 192
//2 3 5 7
float32_t matCase3by3_192[] = {
    0, 1, 1,
    0, 1, 0,
    1, 0, 0,
};

//Matrix 193
//2 3 5 8
float32_t matCase3by3_193[] = {
    0, 1, 1,
    0, 1, 0,
    0, 1, 0,
};

//Matrix 194
//2 3 5 9
float32_t matCase3by3_194[] = {
    0, 1, 1,
    0, 1, 0,
    0, 0, 1,
};

//Matrix 195
//2 3 6 7
float32_t matCase3by3_195[] = {
    0, 1, 1,
    0, 0, 1,
    1, 0, 0,
};

//Matrix 196
//2 3 6 8
float32_t matCase3by3_196[] = {
    0, 1, 1,
    0, 0, 1,
    0, 1, 0,
};

//Matrix 197
//2 3 6 9
float32_t matCase3by3_197[] = {
    0, 1, 1,
    0, 0, 1,
    0, 0, 1,
};

//Matrix 198
//2 3 7 8
float32_t matCase3by3_198[] = {
    0, 1, 1,
    0, 0, 0,
    1, 1, 0,
};

//Matrix 199
//2 3 7 9
float32_t matCase3by3_199[] = {
    0, 1, 1,
    0, 0, 0,
    1, 0, 1,
};

//Matrix 200
//2 3 8 9
float32_t matCase3by3_200[] = {
    0, 1, 1,
    0, 0, 0,
    0, 1, 1,
};

//Matrix 201
//2 4 5 6
float32_t matCase3by3_201[] = {
    0, 1, 0,
    1, 1, 1,
    0, 0, 0,
};

//Matrix 202
//2 4 5 7
float32_t matCase3by3_202[] = {
    0, 1, 0,
    1, 1, 0,
    1, 0, 0,
};

//Matrix 203
//2 4 5 8
float32_t matCase3by3_203[] = {
    0, 1, 0,
    1, 1, 0,
    0, 1, 0,
};

//Matrix 204
//2 4 5 9
float32_t matCase3by3_204[] = {
    0, 1, 0,
    1, 1, 0,
    0, 0, 1,
};

//Matrix 205
//2 4 6 7
float32_t matCase3by3_205[] = {
    0, 1, 0,
    1, 0, 1,
    1, 0, 0,
};

//Matrix 206
//2 4 6 8
float32_t matCase3by3_206[] = {
    0, 1, 0,
    1, 0, 1,
    0, 1, 0,
};

//Matrix 207
//2 4 6 9
float32_t matCase3by3_207[] = {
    0, 1, 0,
    1, 0, 1,
    0, 0, 1,
};

//Matrix 208
//2 4 7 8
float32_t matCase3by3_208[] = {
    0, 1, 0,
    1, 0, 0,
    1, 1, 0,
};

//Matrix 209
//2 4 7 9
float32_t matCase3by3_209[] = {
    0, 1, 0,
    1, 0, 0,
    1, 0, 1,
};

//Matrix 210
//2 4 8 9
float32_t matCase3by3_210[] = {
    0, 1, 0,
    1, 0, 0,
    0, 1, 1,
};

//Matrix 211
//2 5 6 7
float32_t matCase3by3_211[] = {
    0, 1, 0,
    0, 1, 1,
    1, 0, 0,
};

//Matrix 212
//2 5 6 8
float32_t matCase3by3_212[] = {
    0, 1, 0,
    0, 1, 1,
    0, 1, 0,
};

//Matrix 213
//2 5 6 9
float32_t matCase3by3_213[] = {
    0, 1, 0,
    0, 1, 1,
    0, 0, 1,
};

//Matrix 214
//2 5 7 8
float32_t matCase3by3_214[] = {
    0, 1, 0,
    0, 1, 0,
    1, 1, 0,
};

//Matrix 215
//2 5 7 9
float32_t matCase3by3_215[] = {
    0, 1, 0,
    0, 1, 0,
    1, 0, 1,
};

//Matrix 216
//2 5 8 9
float32_t matCase3by3_216[] = {
    0, 1, 0,
    0, 1, 0,
    0, 1, 1,
};

//Matrix 217
//2 6 7 8
float32_t matCase3by3_217[] = {
    0, 1, 0,
    0, 0, 1,
    1, 1, 0,
};

//Matrix 218
//2 6 7 9
float32_t matCase3by3_218[] = {
    0, 1, 0,
    0, 0, 1,
    1, 0, 1,
};

//Matrix 219
//2 6 8 9
float32_t matCase3by3_219[] = {
    0, 1, 0,
    0, 0, 1,
    0, 1, 1,
};

//Matrix 220
//2 7 8 9
float32_t matCase3by3_220[] = {
    0, 1, 0,
    0, 0, 0,
    1, 1, 1,
};

//Matrix 221
//3 4 5 6
float32_t matCase3by3_221[] = {
    0, 0, 1,
    1, 1, 1,
    0, 0, 0,
};

//Matrix 222
//3 4 5 7
float32_t matCase3by3_222[] = {
    0, 0, 1,
    1, 1, 0,
    1, 0, 0,
};

//Matrix 223
//3 4 5 8
float32_t matCase3by3_223[] = {
    0, 0, 1,
    1, 1, 0,
    0, 1, 0,
};

//Matrix 224
//3 4 5 9
float32_t matCase3by3_224[] = {
    0, 0, 1,
    1, 1, 0,
    0, 0, 1,
};

//Matrix 225
//3 4 6 7
float32_t matCase3by3_225[] = {
    0, 0, 1,
    1, 0, 1,
    1, 0, 0,
};

//Matrix 226
//3 4 6 8
float32_t matCase3by3_226[] = {
    0, 0, 1,
    1, 0, 1,
    0, 1, 0,
};

//Matrix 227
//3 4 6 9
float32_t matCase3by3_227[] = {
    0, 0, 1,
    1, 0, 1,
    0, 0, 1,
};

//Matrix 228
//3 4 7 8
float32_t matCase3by3_228[] = {
    0, 0, 1,
    1, 0, 0,
    1, 1, 0,
};

//Matrix 229
//3 4 7 9
float32_t matCase3by3_229[] = {
    0, 0, 1,
    1, 0, 0,
    1, 0, 1,
};

//Matrix 230
//3 4 8 9
float32_t matCase3by3_230[] = {
    0, 0, 1,
    1, 0, 0,
    0, 1, 1,
};

//Matrix 231
//3 5 6 7
float32_t matCase3by3_231[] = {
    0, 0, 1,
    0, 1, 1,
    1, 0, 0,
};

//Matrix 232
//3 5 6 8
float32_t matCase3by3_232[] = {
    0, 0, 1,
    0, 1, 1,
    0, 1, 0,
};

//Matrix 233
//3 5 6 9
float32_t matCase3by3_233[] = {
    0, 0, 1,
    0, 1, 1,
    0, 0, 1,
};

//Matrix 234
//3 5 7 8
float32_t matCase3by3_234[] = {
    0, 0, 1,
    0, 1, 0,
    1, 1, 0,
};

//Matrix 235
//3 5 7 9
float32_t matCase3by3_235[] = {
    0, 0, 1,
    0, 1, 0,
    1, 0, 1,
};

//Matrix 236
//3 5 8 9
float32_t matCase3by3_236[] = {
    0, 0, 1,
    0, 1, 0,
    0, 1, 1,
};

//Matrix 237
//3 6 7 8
float32_t matCase3by3_237[] = {
    0, 0, 1,
    0, 0, 1,
    1, 1, 0,
};

//Matrix 238
//3 6 7 9
float32_t matCase3by3_238[] = {
    0, 0, 1,
    0, 0, 1,
    1, 0, 1,
};

//Matrix 239
//3 6 8 9
float32_t matCase3by3_239[] = {
    0, 0, 1,
    0, 0, 1,
    0, 1, 1,
};

//Matrix 240
//3 7 8 9
float32_t matCase3by3_240[] = {
    0, 0, 1,
    0, 0, 0,
    1, 1, 1,
};

//Matrix 241
//4 5 6 7
float32_t matCase3by3_241[] = {
    0, 0, 0,
    1, 1, 1,
    1, 0, 0,
};

//Matrix 242
//4 5 6 8
float32_t matCase3by3_242[] = {
    0, 0, 0,
    1, 1, 1,
    0, 1, 0,
};

//Matrix 243
//4 5 6 9
float32_t matCase3by3_243[] = {
    0, 0, 0,
    1, 1, 1,
    0, 0, 1,
};

//Matrix 244
//4 5 7 8
float32_t matCase3by3_244[] = {
    0, 0, 0,
    1, 1, 0,
    1, 1, 0,
};

//Matrix 245
//4 5 7 9
float32_t matCase3by3_245[] = {
    0, 0, 0,
    1, 1, 0,
    1, 0, 1,
};

//Matrix 246
//4 5 8 9
float32_t matCase3by3_246[] = {
    0, 0, 0,
    1, 1, 0,
    0, 1, 1,
};

//Matrix 247
//4 6 7 8
float32_t matCase3by3_247[] = {
    0, 0, 0,
    1, 0, 1,
    1, 1, 0,
};

//Matrix 248
//4 6 7 9
float32_t matCase3by3_248[] = {
    0, 0, 0,
    1, 0, 1,
    1, 0, 1,
};

//Matrix 249
//4 6 8 9
float32_t matCase3by3_249[] = {
    0, 0, 0,
    1, 0, 1,
    0, 1, 1,
};

//Matrix 250
//4 7 8 9
float32_t matCase3by3_250[] = {
    0, 0, 0,
    1, 0, 0,
    1, 1, 1,
};

//Matrix 251
//5 6 7 8
float32_t matCase3by3_251[] = {
    0, 0, 0,
    0, 1, 1,
    1, 1, 0,
};

//Matrix 252
//5 6 7 9
float32_t matCase3by3_252[] = {
    0, 0, 0,
    0, 1, 1,
    1, 0, 1,
};

//Matrix 253
//5 6 8 9
float32_t matCase3by3_253[] = {
    0, 0, 0,
    0, 1, 1,
    0, 1, 1,
};

//Matrix 254
//5 7 8 9
float32_t matCase3by3_254[] = {
    0, 0, 0,
    0, 1, 0,
    1, 1, 1,
};

//Matrix 255
//6 7 8 9
float32_t matCase3by3_255[] = {
    0, 0, 0,
    0, 0, 1,
    1, 1, 1,
};

//Matrix 256
//1 2 3 4 5
float32_t matCase3by3_256[] = {
    1, 1, 1,
    1, 1, 0,
    0, 0, 0,
};

//Matrix 257
//1 2 3 4 6
float32_t matCase3by3_257[] = {
    1, 1, 1,
    1, 0, 1,
    0, 0, 0,
};

//Matrix 258
//1 2 3 4 7
float32_t matCase3by3_258[] = {
    1, 1, 1,
    1, 0, 0,
    1, 0, 0,
};

//Matrix 259
//1 2 3 4 8
float32_t matCase3by3_259[] = {
    1, 1, 1,
    1, 0, 0,
    0, 1, 0,
};

//Matrix 260
//1 2 3 4 9
float32_t matCase3by3_260[] = {
    1, 1, 1,
    1, 0, 0,
    0, 0, 1,
};

//Matrix 261
//1 2 3 5 6
float32_t matCase3by3_261[] = {
    1, 1, 1,
    0, 1, 1,
    0, 0, 0,
};

//Matrix 262
//1 2 3 5 7
float32_t matCase3by3_262[] = {
    1, 1, 1,
    0, 1, 0,
    1, 0, 0,
};

//Matrix 263
//1 2 3 5 8
float32_t matCase3by3_263[] = {
    1, 1, 1,
    0, 1, 0,
    0, 1, 0,
};

//Matrix 264
//1 2 3 5 9
float32_t matCase3by3_264[] = {
    1, 1, 1,
    0, 1, 0,
    0, 0, 1,
};

//Matrix 265
//1 2 3 6 7
float32_t matCase3by3_265[] = {
    1, 1, 1,
    0, 0, 1,
    1, 0, 0,
};

//Matrix 266
//1 2 3 6 8
float32_t matCase3by3_266[] = {
    1, 1, 1,
    0, 0, 1,
    0, 1, 0,
};

//Matrix 267
//1 2 3 6 9
float32_t matCase3by3_267[] = {
    1, 1, 1,
    0, 0, 1,
    0, 0, 1,
};

//Matrix 268
//1 2 3 7 8
float32_t matCase3by3_268[] = {
    1, 1, 1,
    0, 0, 0,
    1, 1, 0,
};

//Matrix 269
//1 2 3 7 9
float32_t matCase3by3_269[] = {
    1, 1, 1,
    0, 0, 0,
    1, 0, 1,
};

//Matrix 270
//1 2 3 8 9
float32_t matCase3by3_270[] = {
    1, 1, 1,
    0, 0, 0,
    0, 1, 1,
};

//Matrix 271
//1 2 4 5 6
float32_t matCase3by3_271[] = {
    1, 1, 0,
    1, 1, 1,
    0, 0, 0,
};

//Matrix 272
//1 2 4 5 7
float32_t matCase3by3_272[] = {
    1, 1, 0,
    1, 1, 0,
    1, 0, 0,
};

//Matrix 273
//1 2 4 5 8
float32_t matCase3by3_273[] = {
    1, 1, 0,
    1, 1, 0,
    0, 1, 0,
};

//Matrix 274
//1 2 4 5 9
float32_t matCase3by3_274[] = {
    1, 1, 0,
    1, 1, 0,
    0, 0, 1,
};

//Matrix 275
//1 2 4 6 7
float32_t matCase3by3_275[] = {
    1, 1, 0,
    1, 0, 1,
    1, 0, 0,
};

//Matrix 276
//1 2 4 6 8
float32_t matCase3by3_276[] = {
    1, 1, 0,
    1, 0, 1,
    0, 1, 0,
};

//Matrix 277
//1 2 4 6 9
float32_t matCase3by3_277[] = {
    1, 1, 0,
    1, 0, 1,
    0, 0, 1,
};

//Matrix 278
//1 2 4 7 8
float32_t matCase3by3_278[] = {
    1, 1, 0,
    1, 0, 0,
    1, 1, 0,
};

//Matrix 279
//1 2 4 7 9
float32_t matCase3by3_279[] = {
    1, 1, 0,
    1, 0, 0,
    1, 0, 1,
};

//Matrix 280
//1 2 4 8 9
float32_t matCase3by3_280[] = {
    1, 1, 0,
    1, 0, 0,
    0, 1, 1,
};

//Matrix 281
//1 2 5 6 7
float32_t matCase3by3_281[] = {
    1, 1, 0,
    0, 1, 1,
    1, 0, 0,
};

//Matrix 282
//1 2 5 6 8
float32_t matCase3by3_282[] = {
    1, 1, 0,
    0, 1, 1,
    0, 1, 0,
};

//Matrix 283
//1 2 5 6 9
float32_t matCase3by3_283[] = {
    1, 1, 0,
    0, 1, 1,
    0, 0, 1,
};

//Matrix 284
//1 2 5 7 8
float32_t matCase3by3_284[] = {
    1, 1, 0,
    0, 1, 0,
    1, 1, 0,
};

//Matrix 285
//1 2 5 7 9
float32_t matCase3by3_285[] = {
    1, 1, 0,
    0, 1, 0,
    1, 0, 1,
};

//Matrix 286
//1 2 5 8 9
float32_t matCase3by3_286[] = {
    1, 1, 0,
    0, 1, 0,
    0, 1, 1,
};

//Matrix 287
//1 2 6 7 8
float32_t matCase3by3_287[] = {
    1, 1, 0,
    0, 0, 1,
    1, 1, 0,
};

//Matrix 288
//1 2 6 7 9
float32_t matCase3by3_288[] = {
    1, 1, 0,
    0, 0, 1,
    1, 0, 1,
};

//Matrix 289
//1 2 6 8 9
float32_t matCase3by3_289[] = {
    1, 1, 0,
    0, 0, 1,
    0, 1, 1,
};

//Matrix 290
//1 2 7 8 9
float32_t matCase3by3_290[] = {
    1, 1, 0,
    0, 0, 0,
    1, 1, 1,
};

//Matrix 291
//1 3 4 5 6
float32_t matCase3by3_291[] = {
    1, 0, 1,
    1, 1, 1,
    0, 0, 0,
};

//Matrix 292
//1 3 4 5 7
float32_t matCase3by3_292[] = {
    1, 0, 1,
    1, 1, 0,
    1, 0, 0,
};

//Matrix 293
//1 3 4 5 8
float32_t matCase3by3_293[] = {
    1, 0, 1,
    1, 1, 0,
    0, 1, 0,
};

//Matrix 294
//1 3 4 5 9
float32_t matCase3by3_294[] = {
    1, 0, 1,
    1, 1, 0,
    0, 0, 1,
};

//Matrix 295
//1 3 4 6 7
float32_t matCase3by3_295[] = {
    1, 0, 1,
    1, 0, 1,
    1, 0, 0,
};

//Matrix 296
//1 3 4 6 8
float32_t matCase3by3_296[] = {
    1, 0, 1,
    1, 0, 1,
    0, 1, 0,
};

//Matrix 297
//1 3 4 6 9
float32_t matCase3by3_297[] = {
    1, 0, 1,
    1, 0, 1,
    0, 0, 1,
};

//Matrix 298
//1 3 4 7 8
float32_t matCase3by3_298[] = {
    1, 0, 1,
    1, 0, 0,
    1, 1, 0,
};

//Matrix 299
//1 3 4 7 9
float32_t matCase3by3_299[] = {
    1, 0, 1,
    1, 0, 0,
    1, 0, 1,
};

//Matrix 300
//1 3 4 8 9
float32_t matCase3by3_300[] = {
    1, 0, 1,
    1, 0, 0,
    0, 1, 1,
};

//Matrix 301
//1 3 5 6 7
float32_t matCase3by3_301[] = {
    1, 0, 1,
    0, 1, 1,
    1, 0, 0,
};

//Matrix 302
//1 3 5 6 8
float32_t matCase3by3_302[] = {
    1, 0, 1,
    0, 1, 1,
    0, 1, 0,
};

//Matrix 303
//1 3 5 6 9
float32_t matCase3by3_303[] = {
    1, 0, 1,
    0, 1, 1,
    0, 0, 1,
};

//Matrix 304
//1 3 5 7 8
float32_t matCase3by3_304[] = {
    1, 0, 1,
    0, 1, 0,
    1, 1, 0,
};

//Matrix 305
//1 3 5 7 9
float32_t matCase3by3_305[] = {
    1, 0, 1,
    0, 1, 0,
    1, 0, 1,
};

//Matrix 306
//1 3 5 8 9
float32_t matCase3by3_306[] = {
    1, 0, 1,
    0, 1, 0,
    0, 1, 1,
};

//Matrix 307
//1 3 6 7 8
float32_t matCase3by3_307[] = {
    1, 0, 1,
    0, 0, 1,
    1, 1, 0,
};

//Matrix 308
//1 3 6 7 9
float32_t matCase3by3_308[] = {
    1, 0, 1,
    0, 0, 1,
    1, 0, 1,
};

//Matrix 309
//1 3 6 8 9
float32_t matCase3by3_309[] = {
    1, 0, 1,
    0, 0, 1,
    0, 1, 1,
};

//Matrix 310
//1 3 7 8 9
float32_t matCase3by3_310[] = {
    1, 0, 1,
    0, 0, 0,
    1, 1, 1,
};

//Matrix 311
//1 4 5 6 7
float32_t matCase3by3_311[] = {
    1, 0, 0,
    1, 1, 1,
    1, 0, 0,
};

//Matrix 312
//1 4 5 6 8
float32_t matCase3by3_312[] = {
    1, 0, 0,
    1, 1, 1,
    0, 1, 0,
};

//Matrix 313
//1 4 5 6 9
float32_t matCase3by3_313[] = {
    1, 0, 0,
    1, 1, 1,
    0, 0, 1,
};

//Matrix 314
//1 4 5 7 8
float32_t matCase3by3_314[] = {
    1, 0, 0,
    1, 1, 0,
    1, 1, 0,
};

//Matrix 315
//1 4 5 7 9
float32_t matCase3by3_315[] = {
    1, 0, 0,
    1, 1, 0,
    1, 0, 1,
};

//Matrix 316
//1 4 5 8 9
float32_t matCase3by3_316[] = {
    1, 0, 0,
    1, 1, 0,
    0, 1, 1,
};

//Matrix 317
//1 4 6 7 8
float32_t matCase3by3_317[] = {
    1, 0, 0,
    1, 0, 1,
    1, 1, 0,
};

//Matrix 318
//1 4 6 7 9
float32_t matCase3by3_318[] = {
    1, 0, 0,
    1, 0, 1,
    1, 0, 1,
};

//Matrix 319
//1 4 6 8 9
float32_t matCase3by3_319[] = {
    1, 0, 0,
    1, 0, 1,
    0, 1, 1,
};

//Matrix 320
//1 4 7 8 9
float32_t matCase3by3_320[] = {
    1, 0, 0,
    1, 0, 0,
    1, 1, 1,
};

//Matrix 321
//1 5 6 7 8
float32_t matCase3by3_321[] = {
    1, 0, 0,
    0, 1, 1,
    1, 1, 0,
};

//Matrix 322
//1 5 6 7 9
float32_t matCase3by3_322[] = {
    1, 0, 0,
    0, 1, 1,
    1, 0, 1,
};

//Matrix 323
//1 5 6 8 9
float32_t matCase3by3_323[] = {
    1, 0, 0,
    0, 1, 1,
    0, 1, 1,
};

//Matrix 324
//1 5 7 8 9
float32_t matCase3by3_324[] = {
    1, 0, 0,
    0, 1, 0,
    1, 1, 1,
};

//Matrix 325
//1 6 7 8 9
float32_t matCase3by3_325[] = {
    1, 0, 0,
    0, 0, 1,
    1, 1, 1,
};

//Matrix 326
//2 3 4 5 6
float32_t matCase3by3_326[] = {
    0, 1, 1,
    1, 1, 1,
    0, 0, 0,
};

//Matrix 327
//2 3 4 5 7
float32_t matCase3by3_327[] = {
    0, 1, 1,
    1, 1, 0,
    1, 0, 0,
};

//Matrix 328
//2 3 4 5 8
float32_t matCase3by3_328[] = {
    0, 1, 1,
    1, 1, 0,
    0, 1, 0,
};

//Matrix 329
//2 3 4 5 9
float32_t matCase3by3_329[] = {
    0, 1, 1,
    1, 1, 0,
    0, 0, 1,
};

//Matrix 330
//2 3 4 6 7
float32_t matCase3by3_330[] = {
    0, 1, 1,
    1, 0, 1,
    1, 0, 0,
};

//Matrix 331
//2 3 4 6 8
float32_t matCase3by3_331[] = {
    0, 1, 1,
    1, 0, 1,
    0, 1, 0,
};

//Matrix 332
//2 3 4 6 9
float32_t matCase3by3_332[] = {
    0, 1, 1,
    1, 0, 1,
    0, 0, 1,
};

//Matrix 333
//2 3 4 7 8
float32_t matCase3by3_333[] = {
    0, 1, 1,
    1, 0, 0,
    1, 1, 0,
};

//Matrix 334
//2 3 4 7 9
float32_t matCase3by3_334[] = {
    0, 1, 1,
    1, 0, 0,
    1, 0, 1,
};

//Matrix 335
//2 3 4 8 9
float32_t matCase3by3_335[] = {
    0, 1, 1,
    1, 0, 0,
    0, 1, 1,
};

//Matrix 336
//2 3 5 6 7
float32_t matCase3by3_336[] = {
    0, 1, 1,
    0, 1, 1,
    1, 0, 0,
};

//Matrix 337
//2 3 5 6 8
float32_t matCase3by3_337[] = {
    0, 1, 1,
    0, 1, 1,
    0, 1, 0,
};

//Matrix 338
//2 3 5 6 9
float32_t matCase3by3_338[] = {
    0, 1, 1,
    0, 1, 1,
    0, 0, 1,
};

//Matrix 339
//2 3 5 7 8
float32_t matCase3by3_339[] = {
    0, 1, 1,
    0, 1, 0,
    1, 1, 0,
};

//Matrix 340
//2 3 5 7 9
float32_t matCase3by3_340[] = {
    0, 1, 1,
    0, 1, 0,
    1, 0, 1,
};

//Matrix 341
//2 3 5 8 9
float32_t matCase3by3_341[] = {
    0, 1, 1,
    0, 1, 0,
    0, 1, 1,
};

//Matrix 342
//2 3 6 7 8
float32_t matCase3by3_342[] = {
    0, 1, 1,
    0, 0, 1,
    1, 1, 0,
};

//Matrix 343
//2 3 6 7 9
float32_t matCase3by3_343[] = {
    0, 1, 1,
    0, 0, 1,
    1, 0, 1,
};

//Matrix 344
//2 3 6 8 9
float32_t matCase3by3_344[] = {
    0, 1, 1,
    0, 0, 1,
    0, 1, 1,
};

//Matrix 345
//2 3 7 8 9
float32_t matCase3by3_345[] = {
    0, 1, 1,
    0, 0, 0,
    1, 1, 1,
};

//Matrix 346
//2 4 5 6 7
float32_t matCase3by3_346[] = {
    0, 1, 0,
    1, 1, 1,
    1, 0, 0,
};

//Matrix 347
//2 4 5 6 8
float32_t matCase3by3_347[] = {
    0, 1, 0,
    1, 1, 1,
    0, 1, 0,
};

//Matrix 348
//2 4 5 6 9
float32_t matCase3by3_348[] = {
    0, 1, 0,
    1, 1, 1,
    0, 0, 1,
};

//Matrix 349
//2 4 5 7 8
float32_t matCase3by3_349[] = {
    0, 1, 0,
    1, 1, 0,
    1, 1, 0,
};

//Matrix 350
//2 4 5 7 9
float32_t matCase3by3_350[] = {
    0, 1, 0,
    1, 1, 0,
    1, 0, 1,
};

//Matrix 351
//2 4 5 8 9
float32_t matCase3by3_351[] = {
    0, 1, 0,
    1, 1, 0,
    0, 1, 1,
};

//Matrix 352
//2 4 6 7 8
float32_t matCase3by3_352[] = {
    0, 1, 0,
    1, 0, 1,
    1, 1, 0,
};

//Matrix 353
//2 4 6 7 9
float32_t matCase3by3_353[] = {
    0, 1, 0,
    1, 0, 1,
    1, 0, 1,
};

//Matrix 354
//2 4 6 8 9
float32_t matCase3by3_354[] = {
    0, 1, 0,
    1, 0, 1,
    0, 1, 1,
};

//Matrix 355
//2 4 7 8 9
float32_t matCase3by3_355[] = {
    0, 1, 0,
    1, 0, 0,
    1, 1, 1,
};

//Matrix 356
//2 5 6 7 8
float32_t matCase3by3_356[] = {
    0, 1, 0,
    0, 1, 1,
    1, 1, 0,
};

//Matrix 357
//2 5 6 7 9
float32_t matCase3by3_357[] = {
    0, 1, 0,
    0, 1, 1,
    1, 0, 1,
};

//Matrix 358
//2 5 6 8 9
float32_t matCase3by3_358[] = {
    0, 1, 0,
    0, 1, 1,
    0, 1, 1,
};

//Matrix 359
//2 5 7 8 9
float32_t matCase3by3_359[] = {
    0, 1, 0,
    0, 1, 0,
    1, 1, 1,
};

//Matrix 360
//2 6 7 8 9
float32_t matCase3by3_360[] = {
    0, 1, 0,
    0, 0, 1,
    1, 1, 1,
};

//Matrix 361
//3 4 5 6 7
float32_t matCase3by3_361[] = {
    0, 0, 1,
    1, 1, 1,
    1, 0, 0,
};

//Matrix 362
//3 4 5 6 8
float32_t matCase3by3_362[] = {
    0, 0, 1,
    1, 1, 1,
    0, 1, 0,
};

//Matrix 363
//3 4 5 6 9
float32_t matCase3by3_363[] = {
    0, 0, 1,
    1, 1, 1,
    0, 0, 1,
};

//Matrix 364
//3 4 5 7 8
float32_t matCase3by3_364[] = {
    0, 0, 1,
    1, 1, 0,
    1, 1, 0,
};

//Matrix 365
//3 4 5 7 9
float32_t matCase3by3_365[] = {
    0, 0, 1,
    1, 1, 0,
    1, 0, 1,
};

//Matrix 366
//3 4 5 8 9
float32_t matCase3by3_366[] = {
    0, 0, 1,
    1, 1, 0,
    0, 1, 1,
};

//Matrix 367
//3 4 6 7 8
float32_t matCase3by3_367[] = {
    0, 0, 1,
    1, 0, 1,
    1, 1, 0,
};

//Matrix 368
//3 4 6 7 9
float32_t matCase3by3_368[] = {
    0, 0, 1,
    1, 0, 1,
    1, 0, 1,
};

//Matrix 369
//3 4 6 8 9
float32_t matCase3by3_369[] = {
    0, 0, 1,
    1, 0, 1,
    0, 1, 1,
};

//Matrix 370
//3 4 7 8 9
float32_t matCase3by3_370[] = {
    0, 0, 1,
    1, 0, 0,
    1, 1, 1,
};

//Matrix 371
//3 5 6 7 8
float32_t matCase3by3_371[] = {
    0, 0, 1,
    0, 1, 1,
    1, 1, 0,
};

//Matrix 372
//3 5 6 7 9
float32_t matCase3by3_372[] = {
    0, 0, 1,
    0, 1, 1,
    1, 0, 1,
};

//Matrix 373
//3 5 6 8 9
float32_t matCase3by3_373[] = {
    0, 0, 1,
    0, 1, 1,
    0, 1, 1,
};

//Matrix 374
//3 5 7 8 9
float32_t matCase3by3_374[] = {
    0, 0, 1,
    0, 1, 0,
    1, 1, 1,
};

//Matrix 375
//3 6 7 8 9
float32_t matCase3by3_375[] = {
    0, 0, 1,
    0, 0, 1,
    1, 1, 1,
};

//Matrix 376
//4 5 6 7 8
float32_t matCase3by3_376[] = {
    0, 0, 0,
    1, 1, 1,
    1, 1, 0,
};

//Matrix 377
//4 5 6 7 9
float32_t matCase3by3_377[] = {
    0, 0, 0,
    1, 1, 1,
    1, 0, 1,
};

//Matrix 378
//4 5 6 8 9
float32_t matCase3by3_378[] = {
    0, 0, 0,
    1, 1, 1,
    0, 1, 1,
};

//Matrix 379
//4 5 7 8 9
float32_t matCase3by3_379[] = {
    0, 0, 0,
    1, 1, 0,
    1, 1, 1,
};

//Matrix 380
//4 6 7 8 9
float32_t matCase3by3_380[] = {
    0, 0, 0,
    1, 0, 1,
    1, 1, 1,
};

//Matrix 381
//5 6 7 8 9
float32_t matCase3by3_381[] = {
    0, 0, 0,
    0, 1, 1,
    1, 1, 1,
};

//Matrix 382
//1 2 3 4 5 6
float32_t matCase3by3_382[] = {
    1, 1, 1,
    1, 1, 1,
    0, 0, 0,
};

//Matrix 383
//1 2 3 4 5 7
float32_t matCase3by3_383[] = {
    1, 1, 1,
    1, 1, 0,
    1, 0, 0,
};

//Matrix 384
//1 2 3 4 5 8
float32_t matCase3by3_384[] = {
    1, 1, 1,
    1, 1, 0,
    0, 1, 0,
};

//Matrix 385
//1 2 3 4 5 9
float32_t matCase3by3_385[] = {
    1, 1, 1,
    1, 1, 0,
    0, 0, 1,
};

//Matrix 386
//1 2 3 4 6 7
float32_t matCase3by3_386[] = {
    1, 1, 1,
    1, 0, 1,
    1, 0, 0,
};

//Matrix 387
//1 2 3 4 6 8
float32_t matCase3by3_387[] = {
    1, 1, 1,
    1, 0, 1,
    0, 1, 0,
};

//Matrix 388
//1 2 3 4 6 9
float32_t matCase3by3_388[] = {
    1, 1, 1,
    1, 0, 1,
    0, 0, 1,
};

//Matrix 389
//1 2 3 4 7 8
float32_t matCase3by3_389[] = {
    1, 1, 1,
    1, 0, 0,
    1, 1, 0,
};

//Matrix 390
//1 2 3 4 7 9
float32_t matCase3by3_390[] = {
    1, 1, 1,
    1, 0, 0,
    1, 0, 1,
};

//Matrix 391
//1 2 3 4 8 9
float32_t matCase3by3_391[] = {
    1, 1, 1,
    1, 0, 0,
    0, 1, 1,
};

//Matrix 392
//1 2 3 5 6 7
float32_t matCase3by3_392[] = {
    1, 1, 1,
    0, 1, 1,
    1, 0, 0,
};

//Matrix 393
//1 2 3 5 6 8
float32_t matCase3by3_393[] = {
    1, 1, 1,
    0, 1, 1,
    0, 1, 0,
};

//Matrix 394
//1 2 3 5 6 9
float32_t matCase3by3_394[] = {
    1, 1, 1,
    0, 1, 1,
    0, 0, 1,
};

//Matrix 395
//1 2 3 5 7 8
float32_t matCase3by3_395[] = {
    1, 1, 1,
    0, 1, 0,
    1, 1, 0,
};

//Matrix 396
//1 2 3 5 7 9
float32_t matCase3by3_396[] = {
    1, 1, 1,
    0, 1, 0,
    1, 0, 1,
};

//Matrix 397
//1 2 3 5 8 9
float32_t matCase3by3_397[] = {
    1, 1, 1,
    0, 1, 0,
    0, 1, 1,
};

//Matrix 398
//1 2 3 6 7 8
float32_t matCase3by3_398[] = {
    1, 1, 1,
    0, 0, 1,
    1, 1, 0,
};

//Matrix 399
//1 2 3 6 7 9
float32_t matCase3by3_399[] = {
    1, 1, 1,
    0, 0, 1,
    1, 0, 1,
};

//Matrix 400
//1 2 3 6 8 9
float32_t matCase3by3_400[] = {
    1, 1, 1,
    0, 0, 1,
    0, 1, 1,
};

//Matrix 401
//1 2 3 7 8 9
float32_t matCase3by3_401[] = {
    1, 1, 1,
    0, 0, 0,
    1, 1, 1,
};

//Matrix 402
//1 2 4 5 6 7
float32_t matCase3by3_402[] = {
    1, 1, 0,
    1, 1, 1,
    1, 0, 0,
};

//Matrix 403
//1 2 4 5 6 8
float32_t matCase3by3_403[] = {
    1, 1, 0,
    1, 1, 1,
    0, 1, 0,
};

//Matrix 404
//1 2 4 5 6 9
float32_t matCase3by3_404[] = {
    1, 1, 0,
    1, 1, 1,
    0, 0, 1,
};

//Matrix 405
//1 2 4 5 7 8
float32_t matCase3by3_405[] = {
    1, 1, 0,
    1, 1, 0,
    1, 1, 0,
};

//Matrix 406
//1 2 4 5 7 9
float32_t matCase3by3_406[] = {
    1, 1, 0,
    1, 1, 0,
    1, 0, 1,
};

//Matrix 407
//1 2 4 5 8 9
float32_t matCase3by3_407[] = {
    1, 1, 0,
    1, 1, 0,
    0, 1, 1,
};

//Matrix 408
//1 2 4 6 7 8
float32_t matCase3by3_408[] = {
    1, 1, 0,
    1, 0, 1,
    1, 1, 0,
};

//Matrix 409
//1 2 4 6 7 9
float32_t matCase3by3_409[] = {
    1, 1, 0,
    1, 0, 1,
    1, 0, 1,
};

//Matrix 410
//1 2 4 6 8 9
float32_t matCase3by3_410[] = {
    1, 1, 0,
    1, 0, 1,
    0, 1, 1,
};

//Matrix 411
//1 2 4 7 8 9
float32_t matCase3by3_411[] = {
    1, 1, 0,
    1, 0, 0,
    1, 1, 1,
};

//Matrix 412
//1 2 5 6 7 8
float32_t matCase3by3_412[] = {
    1, 1, 0,
    0, 1, 1,
    1, 1, 0,
};

//Matrix 413
//1 2 5 6 7 9
float32_t matCase3by3_413[] = {
    1, 1, 0,
    0, 1, 1,
    1, 0, 1,
};

//Matrix 414
//1 2 5 6 8 9
float32_t matCase3by3_414[] = {
    1, 1, 0,
    0, 1, 1,
    0, 1, 1,
};

//Matrix 415
//1 2 5 7 8 9
float32_t matCase3by3_415[] = {
    1, 1, 0,
    0, 1, 0,
    1, 1, 1,
};

//Matrix 416
//1 2 6 7 8 9
float32_t matCase3by3_416[] = {
    1, 1, 0,
    0, 0, 1,
    1, 1, 1,
};

//Matrix 417
//1 3 4 5 6 7
float32_t matCase3by3_417[] = {
    1, 0, 1,
    1, 1, 1,
    1, 0, 0,
};

//Matrix 418
//1 3 4 5 6 8
float32_t matCase3by3_418[] = {
    1, 0, 1,
    1, 1, 1,
    0, 1, 0,
};

//Matrix 419
//1 3 4 5 6 9
float32_t matCase3by3_419[] = {
    1, 0, 1,
    1, 1, 1,
    0, 0, 1,
};

//Matrix 420
//1 3 4 5 7 8
float32_t matCase3by3_420[] = {
    1, 0, 1,
    1, 1, 0,
    1, 1, 0,
};

//Matrix 421
//1 3 4 5 7 9
float32_t matCase3by3_421[] = {
    1, 0, 1,
    1, 1, 0,
    1, 0, 1,
};

//Matrix 422
//1 3 4 5 8 9
float32_t matCase3by3_422[] = {
    1, 0, 1,
    1, 1, 0,
    0, 1, 1,
};

//Matrix 423
//1 3 4 6 7 8
float32_t matCase3by3_423[] = {
    1, 0, 1,
    1, 0, 1,
    1, 1, 0,
};

//Matrix 424
//1 3 4 6 7 9
float32_t matCase3by3_424[] = {
    1, 0, 1,
    1, 0, 1,
    1, 0, 1,
};

//Matrix 425
//1 3 4 6 8 9
float32_t matCase3by3_425[] = {
    1, 0, 1,
    1, 0, 1,
    0, 1, 1,
};

//Matrix 426
//1 3 4 7 8 9
float32_t matCase3by3_426[] = {
    1, 0, 1,
    1, 0, 0,
    1, 1, 1,
};

//Matrix 427
//1 3 5 6 7 8
float32_t matCase3by3_427[] = {
    1, 0, 1,
    0, 1, 1,
    1, 1, 0,
};

//Matrix 428
//1 3 5 6 7 9
float32_t matCase3by3_428[] = {
    1, 0, 1,
    0, 1, 1,
    1, 0, 1,
};

//Matrix 429
//1 3 5 6 8 9
float32_t matCase3by3_429[] = {
    1, 0, 1,
    0, 1, 1,
    0, 1, 1,
};

//Matrix 430
//1 3 5 7 8 9
float32_t matCase3by3_430[] = {
    1, 0, 1,
    0, 1, 0,
    1, 1, 1,
};

//Matrix 431
//1 3 6 7 8 9
float32_t matCase3by3_431[] = {
    1, 0, 1,
    0, 0, 1,
    1, 1, 1,
};

//Matrix 432
//1 4 5 6 7 8
float32_t matCase3by3_432[] = {
    1, 0, 0,
    1, 1, 1,
    1, 1, 0,
};

//Matrix 433
//1 4 5 6 7 9
float32_t matCase3by3_433[] = {
    1, 0, 0,
    1, 1, 1,
    1, 0, 1,
};

//Matrix 434
//1 4 5 6 8 9
float32_t matCase3by3_434[] = {
    1, 0, 0,
    1, 1, 1,
    0, 1, 1,
};

//Matrix 435
//1 4 5 7 8 9
float32_t matCase3by3_435[] = {
    1, 0, 0,
    1, 1, 0,
    1, 1, 1,
};

//Matrix 436
//1 4 6 7 8 9
float32_t matCase3by3_436[] = {
    1, 0, 0,
    1, 0, 1,
    1, 1, 1,
};

//Matrix 437
//1 5 6 7 8 9
float32_t matCase3by3_437[] = {
    1, 0, 0,
    0, 1, 1,
    1, 1, 1,
};

//Matrix 438
//2 3 4 5 6 7
float32_t matCase3by3_438[] = {
    0, 1, 1,
    1, 1, 1,
    1, 0, 0,
};

//Matrix 439
//2 3 4 5 6 8
float32_t matCase3by3_439[] = {
    0, 1, 1,
    1, 1, 1,
    0, 1, 0,
};

//Matrix 440
//2 3 4 5 6 9
float32_t matCase3by3_440[] = {
    0, 1, 1,
    1, 1, 1,
    0, 0, 1,
};

//Matrix 441
//2 3 4 5 7 8
float32_t matCase3by3_441[] = {
    0, 1, 1,
    1, 1, 0,
    1, 1, 0,
};

//Matrix 442
//2 3 4 5 7 9
float32_t matCase3by3_442[] = {
    0, 1, 1,
    1, 1, 0,
    1, 0, 1,
};

//Matrix 443
//2 3 4 5 8 9
float32_t matCase3by3_443[] = {
    0, 1, 1,
    1, 1, 0,
    0, 1, 1,
};

//Matrix 444
//2 3 4 6 7 8
float32_t matCase3by3_444[] = {
    0, 1, 1,
    1, 0, 1,
    1, 1, 0,
};

//Matrix 445
//2 3 4 6 7 9
float32_t matCase3by3_445[] = {
    0, 1, 1,
    1, 0, 1,
    1, 0, 1,
};

//Matrix 446
//2 3 4 6 8 9
float32_t matCase3by3_446[] = {
    0, 1, 1,
    1, 0, 1,
    0, 1, 1,
};

//Matrix 447
//2 3 4 7 8 9
float32_t matCase3by3_447[] = {
    0, 1, 1,
    1, 0, 0,
    1, 1, 1,
};

//Matrix 448
//2 3 5 6 7 8
float32_t matCase3by3_448[] = {
    0, 1, 1,
    0, 1, 1,
    1, 1, 0,
};

//Matrix 449
//2 3 5 6 7 9
float32_t matCase3by3_449[] = {
    0, 1, 1,
    0, 1, 1,
    1, 0, 1,
};

//Matrix 450
//2 3 5 6 8 9
float32_t matCase3by3_450[] = {
    0, 1, 1,
    0, 1, 1,
    0, 1, 1,
};

//Matrix 451
//2 3 5 7 8 9
float32_t matCase3by3_451[] = {
    0, 1, 1,
    0, 1, 0,
    1, 1, 1,
};

//Matrix 452
//2 3 6 7 8 9
float32_t matCase3by3_452[] = {
    0, 1, 1,
    0, 0, 1,
    1, 1, 1,
};

//Matrix 453
//2 4 5 6 7 8
float32_t matCase3by3_453[] = {
    0, 1, 0,
    1, 1, 1,
    1, 1, 0,
};

//Matrix 454
//2 4 5 6 7 9
float32_t matCase3by3_454[] = {
    0, 1, 0,
    1, 1, 1,
    1, 0, 1,
};

//Matrix 455
//2 4 5 6 8 9
float32_t matCase3by3_455[] = {
    0, 1, 0,
    1, 1, 1,
    0, 1, 1,
};

//Matrix 456
//2 4 5 7 8 9
float32_t matCase3by3_456[] = {
    0, 1, 0,
    1, 1, 0,
    1, 1, 1,
};

//Matrix 457
//2 4 6 7 8 9
float32_t matCase3by3_457[] = {
    0, 1, 0,
    1, 0, 1,
    1, 1, 1,
};

//Matrix 458
//2 5 6 7 8 9
float32_t matCase3by3_458[] = {
    0, 1, 0,
    0, 1, 1,
    1, 1, 1,
};

//Matrix 459
//3 4 5 6 7 8
float32_t matCase3by3_459[] = {
    0, 0, 1,
    1, 1, 1,
    1, 1, 0,
};

//Matrix 460
//3 4 5 6 7 9
float32_t matCase3by3_460[] = {
    0, 0, 1,
    1, 1, 1,
    1, 0, 1,
};

//Matrix 461
//3 4 5 6 8 9
float32_t matCase3by3_461[] = {
    0, 0, 1,
    1, 1, 1,
    0, 1, 1,
};

//Matrix 462
//3 4 5 7 8 9
float32_t matCase3by3_462[] = {
    0, 0, 1,
    1, 1, 0,
    1, 1, 1,
};

//Matrix 463
//3 4 6 7 8 9
float32_t matCase3by3_463[] = {
    0, 0, 1,
    1, 0, 1,
    1, 1, 1,
};

//Matrix 464
//3 5 6 7 8 9
float32_t matCase3by3_464[] = {
    0, 0, 1,
    0, 1, 1,
    1, 1, 1,
};

//Matrix 465
//4 5 6 7 8 9
float32_t matCase3by3_465[] = {
    0, 0, 0,
    1, 1, 1,
    1, 1, 1,
};

//Matrix 466
//1 2 3 4 5 6 7
float32_t matCase3by3_466[] = {
    1, 1, 1,
    1, 1, 1,
    1, 0, 0,
};

//Matrix 467
//1 2 3 4 5 6 8
float32_t matCase3by3_467[] = {
    1, 1, 1,
    1, 1, 1,
    0, 1, 0,
};

//Matrix 468
//1 2 3 4 5 6 9
float32_t matCase3by3_468[] = {
    1, 1, 1,
    1, 1, 1,
    0, 0, 1,
};

//Matrix 469
//1 2 3 4 5 7 8
float32_t matCase3by3_469[] = {
    1, 1, 1,
    1, 1, 0,
    1, 1, 0,
};

//Matrix 470
//1 2 3 4 5 7 9
float32_t matCase3by3_470[] = {
    1, 1, 1,
    1, 1, 0,
    1, 0, 1,
};

//Matrix 471
//1 2 3 4 5 8 9
float32_t matCase3by3_471[] = {
    1, 1, 1,
    1, 1, 0,
    0, 1, 1,
};

//Matrix 472
//1 2 3 4 6 7 8
float32_t matCase3by3_472[] = {
    1, 1, 1,
    1, 0, 1,
    1, 1, 0,
};

//Matrix 473
//1 2 3 4 6 7 9
float32_t matCase3by3_473[] = {
    1, 1, 1,
    1, 0, 1,
    1, 0, 1,
};

//Matrix 474
//1 2 3 4 6 8 9
float32_t matCase3by3_474[] = {
    1, 1, 1,
    1, 0, 1,
    0, 1, 1,
};

//Matrix 475
//1 2 3 4 7 8 9
float32_t matCase3by3_475[] = {
    1, 1, 1,
    1, 0, 0,
    1, 1, 1,
};

//Matrix 476
//1 2 3 5 6 7 8
float32_t matCase3by3_476[] = {
    1, 1, 1,
    0, 1, 1,
    1, 1, 0,
};

//Matrix 477
//1 2 3 5 6 7 9
float32_t matCase3by3_477[] = {
    1, 1, 1,
    0, 1, 1,
    1, 0, 1,
};

//Matrix 478
//1 2 3 5 6 8 9
float32_t matCase3by3_478[] = {
    1, 1, 1,
    0, 1, 1,
    0, 1, 1,
};

//Matrix 479
//1 2 3 5 7 8 9
float32_t matCase3by3_479[] = {
    1, 1, 1,
    0, 1, 0,
    1, 1, 1,
};

//Matrix 480
//1 2 3 6 7 8 9
float32_t matCase3by3_480[] = {
    1, 1, 1,
    0, 0, 1,
    1, 1, 1,
};

//Matrix 481
//1 2 4 5 6 7 8
float32_t matCase3by3_481[] = {
    1, 1, 0,
    1, 1, 1,
    1, 1, 0,
};

//Matrix 482
//1 2 4 5 6 7 9
float32_t matCase3by3_482[] = {
    1, 1, 0,
    1, 1, 1,
    1, 0, 1,
};

//Matrix 483
//1 2 4 5 6 8 9
float32_t matCase3by3_483[] = {
    1, 1, 0,
    1, 1, 1,
    0, 1, 1,
};

//Matrix 484
//1 2 4 5 7 8 9
float32_t matCase3by3_484[] = {
    1, 1, 0,
    1, 1, 0,
    1, 1, 1
};

//Matrix 485
//1 2 4 6 7 8 9
float32_t matCase3by3_485[] = {
    1, 1, 0,
    1, 0, 1,
    1, 1, 1,
};

//Matrix 486
//1 2 5 6 7 8 9
float32_t matCase3by3_486[] = {
    1, 1, 0,
    0, 1, 1,
    1, 1, 1,
};

//Matrix 487
//1 3 4 5 6 7 8
float32_t matCase3by3_487[] = {
    1, 0, 1,
    1, 1, 1,
    1, 1, 0,
};

//Matrix 488
//1 3 4 5 6 7 9
float32_t matCase3by3_488[] = {
    1, 0, 1,
    1, 1, 1,
    1, 0, 1,
};

//Matrix 489
//1 3 4 5 6 8 9
float32_t matCase3by3_489[] = {
    1, 0, 1,
    1, 1, 1,
    0, 1, 1,
};

//Matrix 490
//1 3 4 5 7 8 9
float32_t matCase3by3_490[] = {
    1, 0, 1,
    1, 1, 0,
    1, 1, 1,
};

//Matrix 491
//1 3 4 6 7 8 9
float32_t matCase3by3_491[] = {
    1, 0, 1,
    1, 0, 1,
    1, 1, 1,
};

//Matrix 492
//1 3 5 6 7 8 9
float32_t matCase3by3_492[] = {
    1, 0, 1,
    0, 1, 1,
    1, 1, 1,
};

//Matrix 493
//1 4 5 6 7 8 9
float32_t matCase3by3_493[] = {
    1, 0, 0,
    1, 1, 1,
    1, 1, 1,
};

//Matrix 494
//2 3 4 5 6 7 8
float32_t matCase3by3_494[] = {
    0, 1, 1,
    1, 1, 1,
    1, 1, 0,
};

//Matrix 495
//2 3 4 5 6 7 9
float32_t matCase3by3_495[] = {
    0, 1, 1,
    1, 1, 1,
    1, 0, 1,
};

//Matrix 496
//2 3 4 5 6 8 9
float32_t matCase3by3_496[] = {
    0, 1, 1,
    1, 1, 1,
    0, 1, 1,
};

//Matrix 497
//2 3 4 5 7 8 9
float32_t matCase3by3_497[] = {
    0, 1, 1,
    1, 1, 0,
    1, 1, 1
};

//Matrix 498
//2 3 4 6 7 8 9
float32_t matCase3by3_498[] = {
    0, 1, 1,
    1, 0, 1,
    1, 1, 1,
};

//Matrix 499
//2 3 5 6 7 8 9
float32_t matCase3by3_499[] = {
    0, 1, 1,
    0, 1, 1,
    1, 1, 1,
};

//Matrix 500
//2 4 5 6 7 8 9
float32_t matCase3by3_500[] = {
    0, 1, 0,
    1, 1, 1,
    1, 1, 1,
};

//Matrix 501
//3 4 5 6 7 8 9
float32_t matCase3by3_501[] = {
    0, 0, 1,
    1, 1, 1,
    1, 1, 1,
};

//Matrix 502
//1 2 3 4 5 6 7 8
float32_t matCase3by3_502[] = {
    1, 1, 1,
    1, 1, 1,
    1, 1, 0
};

//Matrix 503
//1 2 3 4 5 6 7 9
float32_t matCase3by3_503[] = {
    1, 1, 1,
    1, 1, 1,
    1, 0, 1,
};

//Matrix 504
//1 2 3 4 5 6 8 9
float32_t matCase3by3_504[] = {
    1, 1, 1,
    1, 1, 1,
    0, 1, 1,
};

//Matrix 505
//1 2 3 4 5 7 8 9
float32_t matCase3by3_505[] = {
    1, 1, 1,
    1, 1, 0,
    1, 1, 1,
};

//Matrix 506
//1 2 3 4 6 7 8 9
float32_t matCase3by3_506[] = {
    1, 1, 1,
    1, 0, 1,
    1, 1, 1,
};

//Matrix 507
//1 2 3 5 6 7 8 9
float32_t matCase3by3_507[] = {
    1, 1, 1,
    0, 1, 1,
    1, 1, 1
};

//Matrix 508
//1 2 4 5 6 7 8 9
float32_t matCase3by3_508[] = {
    1, 1, 0,
    1, 1, 1,
    1, 1, 1,
};

//Matrix 509
//1 3 4 5 6 7 8 9
float32_t matCase3by3_509[] = {
    1, 0, 1,
    1, 1, 1,
    1, 1, 1,
};

//Matrix 510
//2 3 4 5 6 7 8 9
float32_t matCase3by3_510[] = {
    0, 1, 1,
    1, 1, 1,
    1, 1, 1
};

//Matrix 511
//1 2 3 4 5 6 7 8 9
float32_t matCase3by3_511[] = {
    1, 1, 1,
    1, 1, 1,
    1, 1, 1
};

struMat matArray2[] = {
    {matCase3by3_0, 3,FAIL},
    {matCase3by3_1, 3,FAIL},
    {matCase3by3_2, 3,FAIL},
    {matCase3by3_3, 3,FAIL},
    {matCase3by3_4, 3,FAIL},
    {matCase3by3_5, 3,FAIL},
    {matCase3by3_6, 3,FAIL},
    {matCase3by3_7, 3,FAIL},
    {matCase3by3_8, 3,FAIL},
    {matCase3by3_9, 3,FAIL},
    {matCase3by3_10, 3,FAIL},
    {matCase3by3_11, 3,FAIL},
    {matCase3by3_12, 3,FAIL},
    {matCase3by3_13, 3,FAIL},
    {matCase3by3_14, 3,FAIL},
    {matCase3by3_15, 3,FAIL},
    {matCase3by3_16, 3,FAIL},
    {matCase3by3_17, 3,FAIL},
    {matCase3by3_18, 3,FAIL},
    {matCase3by3_19, 3,FAIL},
    {matCase3by3_20, 3,FAIL},
    {matCase3by3_21, 3,FAIL},
    {matCase3by3_22, 3,FAIL},
    {matCase3by3_23, 3,FAIL},
    {matCase3by3_24, 3,FAIL},
    {matCase3by3_25, 3,FAIL},
    {matCase3by3_26, 3,FAIL},
    {matCase3by3_27, 3,FAIL},
    {matCase3by3_28, 3,FAIL},
    {matCase3by3_29, 3,FAIL},
    {matCase3by3_30, 3,FAIL},
    {matCase3by3_31, 3,FAIL},
    {matCase3by3_32, 3,FAIL},
    {matCase3by3_33, 3,FAIL},
    {matCase3by3_34, 3,FAIL},
    {matCase3by3_35, 3,FAIL},
    {matCase3by3_36, 3,FAIL},
    {matCase3by3_37, 3,FAIL},
    {matCase3by3_38, 3,FAIL},
    {matCase3by3_39, 3,FAIL},
    {matCase3by3_40, 3,FAIL},
    {matCase3by3_41, 3,FAIL},
    {matCase3by3_42, 3,FAIL},
    {matCase3by3_43, 3,FAIL},
    {matCase3by3_44, 3,FAIL},
    {matCase3by3_45, 3,FAIL},
    {matCase3by3_46, 3,FAIL},
    {matCase3by3_47, 3,FAIL},
    {matCase3by3_48, 3,FAIL},
    {matCase3by3_49, 3,FAIL},
    {matCase3by3_50, 3,FAIL},
    {matCase3by3_51, 3,FAIL},
    {matCase3by3_52, 3,FAIL},
    {matCase3by3_53, 3,FAIL},
    {matCase3by3_54, 3,FAIL},
    {matCase3by3_55, 3,FAIL},
    {matCase3by3_56, 3,FAIL},
    {matCase3by3_57, 3,FAIL},
    {matCase3by3_58, 3,FAIL},
    {matCase3by3_59, 3,FAIL},
    {matCase3by3_60, 3,FAIL},
    {matCase3by3_61, 3,FAIL},
    {matCase3by3_62, 3,FAIL},
    {matCase3by3_63, 3,FAIL},
    {matCase3by3_64, 3,FAIL},
    {matCase3by3_65, 3,FAIL},
    {matCase3by3_66, 3,FAIL},
    {matCase3by3_67, 3,SUCCESS},
    {matCase3by3_68, 3,FAIL},
    {matCase3by3_69, 3,SUCCESS},
    {matCase3by3_70, 3,FAIL},
    {matCase3by3_71, 3,FAIL},
    {matCase3by3_72, 3,FAIL},
    {matCase3by3_73, 3,FAIL},
    {matCase3by3_74, 3,FAIL},
    {matCase3by3_75, 3,FAIL},
    {matCase3by3_76, 3,FAIL},
    {matCase3by3_77, 3,FAIL},
    {matCase3by3_78, 3,FAIL},
    {matCase3by3_79, 3,FAIL},
    {matCase3by3_80, 3,FAIL},
    {matCase3by3_81, 3,FAIL},
    {matCase3by3_82, 3,FAIL},
    {matCase3by3_83, 3,FAIL},
    {matCase3by3_84, 3,SUCCESS},
    {matCase3by3_85, 3,FAIL},
    {matCase3by3_86, 3,FAIL},
    {matCase3by3_87, 3,FAIL},
    {matCase3by3_88, 3,FAIL},
    {matCase3by3_89, 3,SUCCESS},
    {matCase3by3_90, 3,FAIL},
    {matCase3by3_91, 3,FAIL},
    {matCase3by3_92, 3,FAIL},
    {matCase3by3_93, 3,FAIL},
    {matCase3by3_94, 3,FAIL},
    {matCase3by3_95, 3,FAIL},
    {matCase3by3_96, 3,FAIL},
    {matCase3by3_97, 3,FAIL},
    {matCase3by3_98, 3,SUCCESS},
    {matCase3by3_99, 3,FAIL},
    {matCase3by3_100, 3,FAIL},
    {matCase3by3_101, 3,SUCCESS},
    {matCase3by3_102, 3,FAIL},
    {matCase3by3_103, 3,FAIL},
    {matCase3by3_104, 3,FAIL},
    {matCase3by3_105, 3,FAIL},
    {matCase3by3_106, 3,FAIL},
    {matCase3by3_107, 3,FAIL},
    {matCase3by3_108, 3,FAIL},
    {matCase3by3_109, 3,FAIL},
    {matCase3by3_110, 3,FAIL},
    {matCase3by3_111, 3,FAIL},
    {matCase3by3_112, 3,FAIL},
    {matCase3by3_113, 3,FAIL},
    {matCase3by3_114, 3,FAIL},
    {matCase3by3_115, 3,FAIL},
    {matCase3by3_116, 3,FAIL},
    {matCase3by3_117, 3,FAIL},
    {matCase3by3_118, 3,FAIL},
    {matCase3by3_119, 3,FAIL},
    {matCase3by3_120, 3,FAIL},
    {matCase3by3_121, 3,FAIL},
    {matCase3by3_122, 3,FAIL},
    {matCase3by3_123, 3,FAIL},
    {matCase3by3_124, 3,FAIL},
    {matCase3by3_125, 3,FAIL},
    {matCase3by3_126, 3,FAIL},
    {matCase3by3_127, 3,FAIL},
    {matCase3by3_128, 3,FAIL},
    {matCase3by3_129, 3,FAIL},
    {matCase3by3_130, 3,FAIL},
    {matCase3by3_131, 3,FAIL},
    {matCase3by3_132, 3,FAIL},
    {matCase3by3_133, 3,FAIL},
    {matCase3by3_134, 3,FAIL},
    {matCase3by3_135, 3,FAIL},
    {matCase3by3_136, 3,FAIL},
    {matCase3by3_137, 3,FAIL},
    {matCase3by3_138, 3,FAIL},
    {matCase3by3_139, 3,FAIL},
    {matCase3by3_140, 3,SUCCESS},
    {matCase3by3_141, 3,FAIL},
    {matCase3by3_142, 3,FAIL},
    {matCase3by3_143, 3,FAIL},
    {matCase3by3_144, 3,SUCCESS},
    {matCase3by3_145, 3,SUCCESS},
    {matCase3by3_146, 3,SUCCESS},
    {matCase3by3_147, 3,FAIL},
    {matCase3by3_148, 3,FAIL},
    {matCase3by3_149, 3,FAIL},
    {matCase3by3_150, 3,FAIL},
    {matCase3by3_151, 3,FAIL},
    {matCase3by3_152, 3,FAIL},
    {matCase3by3_153, 3,FAIL},
    {matCase3by3_154, 3,SUCCESS},
    {matCase3by3_155, 3,FAIL},
    {matCase3by3_156, 3,FAIL},
    {matCase3by3_157, 3,SUCCESS},
    {matCase3by3_158, 3,FAIL},
    {matCase3by3_159, 3,SUCCESS},
    {matCase3by3_160, 3,FAIL},
    {matCase3by3_161, 3,SUCCESS},
    {matCase3by3_162, 3,FAIL},
    {matCase3by3_163, 3,FAIL},
    {matCase3by3_164, 3,FAIL},
    {matCase3by3_165, 3,FAIL},
    {matCase3by3_166, 3,FAIL},
    {matCase3by3_167, 3,FAIL},
    {matCase3by3_168, 3,FAIL},
    {matCase3by3_169, 3,SUCCESS},
    {matCase3by3_170, 3,FAIL},
    {matCase3by3_171, 3,SUCCESS},
    {matCase3by3_172, 3,FAIL},
    {matCase3by3_173, 3,FAIL},
    {matCase3by3_174, 3,FAIL},
    {matCase3by3_175, 3,FAIL},
    {matCase3by3_176, 3,FAIL},
    {matCase3by3_177, 3,SUCCESS},
    {matCase3by3_178, 3,SUCCESS},
    {matCase3by3_179, 3,FAIL},
    {matCase3by3_180, 3,SUCCESS},
    {matCase3by3_181, 3,SUCCESS},
    {matCase3by3_182, 3,SUCCESS},
    {matCase3by3_183, 3,FAIL},
    {matCase3by3_184, 3,SUCCESS},
    {matCase3by3_185, 3,FAIL},
    {matCase3by3_186, 3,FAIL},
    {matCase3by3_187, 3,FAIL},
    {matCase3by3_188, 3,FAIL},
    {matCase3by3_189, 3,SUCCESS},
    {matCase3by3_190, 3,SUCCESS},
    {matCase3by3_191, 3,FAIL},
    {matCase3by3_192, 3,SUCCESS},
    {matCase3by3_193, 3,FAIL},
    {matCase3by3_194, 3,FAIL},
    {matCase3by3_195, 3,SUCCESS},
    {matCase3by3_196, 3,FAIL},
    {matCase3by3_197, 3,FAIL},
    {matCase3by3_198, 3,FAIL},
    {matCase3by3_199, 3,FAIL},
    {matCase3by3_200, 3,FAIL},
    {matCase3by3_201, 3,FAIL},
    {matCase3by3_202, 3,FAIL},
    {matCase3by3_203, 3,FAIL},
    {matCase3by3_204, 3,SUCCESS},
    {matCase3by3_205, 3,SUCCESS},
    {matCase3by3_206, 3,FAIL},
    {matCase3by3_207, 3,SUCCESS},
    {matCase3by3_208, 3,FAIL},
    {matCase3by3_209, 3,SUCCESS},
    {matCase3by3_210, 3,SUCCESS},
    {matCase3by3_211, 3,SUCCESS},
    {matCase3by3_212, 3,FAIL},
    {matCase3by3_213, 3,FAIL},
    {matCase3by3_214, 3,FAIL},
    {matCase3by3_215, 3,FAIL},
    {matCase3by3_216, 3,FAIL},
    {matCase3by3_217, 3,SUCCESS},
    {matCase3by3_218, 3,SUCCESS},
    {matCase3by3_219, 3,FAIL},
    {matCase3by3_220, 3,FAIL},
    {matCase3by3_221, 3,FAIL},
    {matCase3by3_222, 3,SUCCESS},
    {matCase3by3_223, 3,SUCCESS},
    {matCase3by3_224, 3,FAIL},
    {matCase3by3_225, 3,FAIL},
    {matCase3by3_226, 3,SUCCESS},
    {matCase3by3_227, 3,FAIL},
    {matCase3by3_228, 3,SUCCESS},
    {matCase3by3_229, 3,FAIL},
    {matCase3by3_230, 3,SUCCESS},
    {matCase3by3_231, 3,SUCCESS},
    {matCase3by3_232, 3,FAIL},
    {matCase3by3_233, 3,FAIL},
    {matCase3by3_234, 3,SUCCESS},
    {matCase3by3_235, 3,SUCCESS},
    {matCase3by3_236, 3,FAIL},
    {matCase3by3_237, 3,FAIL},
    {matCase3by3_238, 3,FAIL},
    {matCase3by3_239, 3,FAIL},
    {matCase3by3_240, 3,FAIL},
    {matCase3by3_241, 3,FAIL},
    {matCase3by3_242, 3,FAIL},
    {matCase3by3_243, 3,FAIL},
    {matCase3by3_244, 3,FAIL},
    {matCase3by3_245, 3,FAIL},
    {matCase3by3_246, 3,FAIL},
    {matCase3by3_247, 3,FAIL},
    {matCase3by3_248, 3,FAIL},
    {matCase3by3_249, 3,FAIL},
    {matCase3by3_250, 3,FAIL},
    {matCase3by3_251, 3,FAIL},
    {matCase3by3_252, 3,FAIL},
    {matCase3by3_253, 3,FAIL},
    {matCase3by3_254, 3,FAIL},
    {matCase3by3_255, 3,FAIL},
    {matCase3by3_256, 3,FAIL},
    {matCase3by3_257, 3,FAIL},
    {matCase3by3_258, 3,FAIL},
    {matCase3by3_259, 3,SUCCESS},
    {matCase3by3_260, 3,SUCCESS},
    {matCase3by3_261, 3,FAIL},
    {matCase3by3_262, 3,SUCCESS},
    {matCase3by3_263, 3,FAIL},
    {matCase3by3_264, 3,SUCCESS},
    {matCase3by3_265, 3,SUCCESS},
    {matCase3by3_266, 3,SUCCESS},
    {matCase3by3_267, 3,FAIL},
    {matCase3by3_268, 3,FAIL},
    {matCase3by3_269, 3,FAIL},
    {matCase3by3_270, 3,FAIL},
    {matCase3by3_271, 3,FAIL},
    {matCase3by3_272, 3,FAIL},
    {matCase3by3_273, 3,FAIL},
    {matCase3by3_274, 3,FAIL},
    {matCase3by3_275, 3,SUCCESS},
    {matCase3by3_276, 3,SUCCESS},
    {matCase3by3_277, 3,SUCCESS},
    {matCase3by3_278, 3,FAIL},
    {matCase3by3_279, 3,SUCCESS},
    {matCase3by3_280, 3,SUCCESS},
    {matCase3by3_281, 3,SUCCESS},
    {matCase3by3_282, 3,SUCCESS},
    {matCase3by3_283, 3,SUCCESS},
    {matCase3by3_284, 3,FAIL},
    {matCase3by3_285, 3,SUCCESS},
    {matCase3by3_286, 3,SUCCESS},
    {matCase3by3_287, 3,FAIL},
    {matCase3by3_288, 3,SUCCESS},
    {matCase3by3_289, 3,SUCCESS},
    {matCase3by3_290, 3,FAIL},
    {matCase3by3_291, 3,FAIL},
    {matCase3by3_292, 3,SUCCESS},
    {matCase3by3_293, 3,SUCCESS},
    {matCase3by3_294, 3,SUCCESS},
    {matCase3by3_295, 3,FAIL},
    {matCase3by3_296, 3,FAIL},
    {matCase3by3_297, 3,FAIL},
    {matCase3by3_298, 3,SUCCESS},
    {matCase3by3_299, 3,FAIL},
    {matCase3by3_300, 3,SUCCESS},
    {matCase3by3_301, 3,SUCCESS},
    {matCase3by3_302, 3,SUCCESS},
    {matCase3by3_303, 3,SUCCESS},
    {matCase3by3_304, 3,SUCCESS},
    {matCase3by3_305, 3,FAIL},
    {matCase3by3_306, 3,SUCCESS},
    {matCase3by3_307, 3,SUCCESS},
    {matCase3by3_308, 3,FAIL},
    {matCase3by3_309, 3,SUCCESS},
    {matCase3by3_310, 3,FAIL},
    {matCase3by3_311, 3,FAIL},
    {matCase3by3_312, 3,SUCCESS},
    {matCase3by3_313, 3,SUCCESS},
    {matCase3by3_314, 3,FAIL},
    {matCase3by3_315, 3,SUCCESS},
    {matCase3by3_316, 3,SUCCESS},
    {matCase3by3_317, 3,SUCCESS},
    {matCase3by3_318, 3,FAIL},
    {matCase3by3_319, 3,SUCCESS},
    {matCase3by3_320, 3,FAIL},
    {matCase3by3_321, 3,SUCCESS},
    {matCase3by3_322, 3,SUCCESS},
    {matCase3by3_323, 3,FAIL},
    {matCase3by3_324, 3,SUCCESS},
    {matCase3by3_325, 3,SUCCESS},
    {matCase3by3_326, 3,FAIL},
    {matCase3by3_327, 3,SUCCESS},
    {matCase3by3_328, 3,SUCCESS},
    {matCase3by3_329, 3,SUCCESS},
    {matCase3by3_330, 3,SUCCESS},
    {matCase3by3_331, 3,SUCCESS},
    {matCase3by3_332, 3,SUCCESS},
    {matCase3by3_333, 3,SUCCESS},
    {matCase3by3_334, 3,SUCCESS},
    {matCase3by3_335, 3,FAIL},
    {matCase3by3_336, 3,FAIL},
    {matCase3by3_337, 3,FAIL},
    {matCase3by3_338, 3,FAIL},
    {matCase3by3_339, 3,SUCCESS},
    {matCase3by3_340, 3,SUCCESS},
    {matCase3by3_341, 3,FAIL},
    {matCase3by3_342, 3,SUCCESS},
    {matCase3by3_343, 3,SUCCESS},
    {matCase3by3_344, 3,FAIL},
    {matCase3by3_345, 3,FAIL},
    {matCase3by3_346, 3,SUCCESS},
    {matCase3by3_347, 3,FAIL},
    {matCase3by3_348, 3,SUCCESS},
    {matCase3by3_349, 3,FAIL},
    {matCase3by3_350, 3,SUCCESS},
    {matCase3by3_351, 3,SUCCESS},
    {matCase3by3_352, 3,SUCCESS},
    {matCase3by3_353, 3,FAIL},
    {matCase3by3_354, 3,SUCCESS},
    {matCase3by3_355, 3,SUCCESS},
    {matCase3by3_356, 3,SUCCESS},
    {matCase3by3_357, 3,SUCCESS},
    {matCase3by3_358, 3,FAIL},
    {matCase3by3_359, 3,FAIL},
    {matCase3by3_360, 3,SUCCESS},
    {matCase3by3_361, 3,SUCCESS},
    {matCase3by3_362, 3,SUCCESS},
    {matCase3by3_363, 3,FAIL},
    {matCase3by3_364, 3,FAIL},
    {matCase3by3_365, 3,SUCCESS},
    {matCase3by3_366, 3,SUCCESS},
    {matCase3by3_367, 3,SUCCESS},
    {matCase3by3_368, 3,FAIL},
    {matCase3by3_369, 3,SUCCESS},
    {matCase3by3_370, 3,SUCCESS},
    {matCase3by3_371, 3,SUCCESS},
    {matCase3by3_372, 3,SUCCESS},
    {matCase3by3_373, 3,FAIL},
    {matCase3by3_374, 3,SUCCESS},
    {matCase3by3_375, 3,FAIL},
    {matCase3by3_376, 3,FAIL},
    {matCase3by3_377, 3,FAIL},
    {matCase3by3_378, 3,FAIL},
    {matCase3by3_379, 3,FAIL},
    {matCase3by3_380, 3,FAIL},
    {matCase3by3_381, 3,FAIL},
    {matCase3by3_382, 3,FAIL},
    {matCase3by3_383, 3,SUCCESS},
    {matCase3by3_384, 3,SUCCESS},
    {matCase3by3_385, 3,FAIL},
    {matCase3by3_386, 3,SUCCESS},
    {matCase3by3_387, 3,FAIL},
    {matCase3by3_388, 3,SUCCESS},
    {matCase3by3_389, 3,SUCCESS},
    {matCase3by3_390, 3,SUCCESS},
    {matCase3by3_391, 3,FAIL},
    {matCase3by3_392, 3,FAIL},
    {matCase3by3_393, 3,SUCCESS},
    {matCase3by3_394, 3,SUCCESS},
    {matCase3by3_395, 3,SUCCESS},
    {matCase3by3_396, 3,FAIL},
    {matCase3by3_397, 3,SUCCESS},
    {matCase3by3_398, 3,FAIL},
    {matCase3by3_399, 3,SUCCESS},
    {matCase3by3_400, 3,SUCCESS},
    {matCase3by3_401, 3,FAIL},
    {matCase3by3_402, 3,SUCCESS},
    {matCase3by3_403, 3,SUCCESS},
    {matCase3by3_404, 3,FAIL},
    {matCase3by3_405, 3,FAIL},
    {matCase3by3_406, 3,FAIL},
    {matCase3by3_407, 3,FAIL},
    {matCase3by3_408, 3,FAIL},
    {matCase3by3_409, 3,FAIL},
    {matCase3by3_410, 3,SUCCESS},
    {matCase3by3_411, 3,SUCCESS},
    {matCase3by3_412, 3,FAIL},
    {matCase3by3_413, 3,SUCCESS},
    {matCase3by3_414, 3,FAIL},
    {matCase3by3_415, 3,SUCCESS},
    {matCase3by3_416, 3,FAIL},
    {matCase3by3_417, 3,SUCCESS},
    {matCase3by3_418, 3,FAIL},
    {matCase3by3_419, 3,SUCCESS},
    {matCase3by3_420, 3,FAIL},
    {matCase3by3_421, 3,FAIL},
    {matCase3by3_422, 3,SUCCESS},
    {matCase3by3_423, 3,FAIL},
    {matCase3by3_424, 3,FAIL},
    {matCase3by3_425, 3,FAIL},
    {matCase3by3_426, 3,SUCCESS},
    {matCase3by3_427, 3,SUCCESS},
    {matCase3by3_428, 3,FAIL},
    {matCase3by3_429, 3,FAIL},
    {matCase3by3_430, 3,FAIL},
    {matCase3by3_431, 3,SUCCESS},
    {matCase3by3_432, 3,SUCCESS},
    {matCase3by3_433, 3,SUCCESS},
    {matCase3by3_434, 3,FAIL},
    {matCase3by3_435, 3,SUCCESS},
    {matCase3by3_436, 3,SUCCESS},
    {matCase3by3_437, 3,FAIL},
    {matCase3by3_438, 3,FAIL},
    {matCase3by3_439, 3,SUCCESS},
    {matCase3by3_440, 3,SUCCESS},
    {matCase3by3_441, 3,FAIL},
    {matCase3by3_442, 3,SUCCESS},
    {matCase3by3_443, 3,FAIL},
    {matCase3by3_444, 3,SUCCESS},
    {matCase3by3_445, 3,FAIL},
    {matCase3by3_446, 3,FAIL},
    {matCase3by3_447, 3,FAIL},
    {matCase3by3_448, 3,FAIL},
    {matCase3by3_449, 3,FAIL},
    {matCase3by3_450, 3,FAIL},
    {matCase3by3_451, 3,SUCCESS},
    {matCase3by3_452, 3,SUCCESS},
    {matCase3by3_453, 3,SUCCESS},
    {matCase3by3_454, 3,FAIL},
    {matCase3by3_455, 3,SUCCESS},
    {matCase3by3_456, 3,SUCCESS},
    {matCase3by3_457, 3,FAIL},
    {matCase3by3_458, 3,SUCCESS},
    {matCase3by3_459, 3,FAIL},
    {matCase3by3_460, 3,SUCCESS},
    {matCase3by3_461, 3,SUCCESS},
    {matCase3by3_462, 3,FAIL},
    {matCase3by3_463, 3,SUCCESS},
    {matCase3by3_464, 3,SUCCESS},
    {matCase3by3_465, 3,FAIL},
    {matCase3by3_466, 3,FAIL},
    {matCase3by3_467, 3,FAIL},
    {matCase3by3_468, 3,FAIL},
    {matCase3by3_469, 3,FAIL},
    {matCase3by3_470, 3,SUCCESS},
    {matCase3by3_471, 3,SUCCESS},
    {matCase3by3_472, 3,SUCCESS},
    {matCase3by3_473, 3,FAIL},
    {matCase3by3_474, 3,SUCCESS},
    {matCase3by3_475, 3,FAIL},
    {matCase3by3_476, 3,SUCCESS},
    {matCase3by3_477, 3,SUCCESS},
    {matCase3by3_478, 3,FAIL},
    {matCase3by3_479, 3,FAIL},
    {matCase3by3_480, 3,FAIL},
    {matCase3by3_481, 3,FAIL},
    {matCase3by3_482, 3,SUCCESS},
    {matCase3by3_483, 3,SUCCESS},
    {matCase3by3_484, 3,FAIL},
    {matCase3by3_485, 3,SUCCESS},
    {matCase3by3_486, 3,SUCCESS},
    {matCase3by3_487, 3,SUCCESS},
    {matCase3by3_488, 3,FAIL},
    {matCase3by3_489, 3,SUCCESS},
    {matCase3by3_490, 3,SUCCESS},
    {matCase3by3_491, 3,FAIL},
    {matCase3by3_492, 3,SUCCESS},
    {matCase3by3_493, 3,FAIL},
    {matCase3by3_494, 3,SUCCESS},
    {matCase3by3_495, 3,SUCCESS},
    {matCase3by3_496, 3,FAIL},
    {matCase3by3_497, 3,SUCCESS},
    {matCase3by3_498, 3,SUCCESS},
    {matCase3by3_499, 3,FAIL},
    {matCase3by3_500, 3,FAIL},
    {matCase3by3_501, 3,FAIL},
    {matCase3by3_502, 3,FAIL},
    {matCase3by3_503, 3,FAIL},
    {matCase3by3_504, 3,FAIL},
    {matCase3by3_505, 3,FAIL},
    {matCase3by3_506, 3,FAIL},
    {matCase3by3_507, 3,FAIL},
    {matCase3by3_508, 3,FAIL},
    {matCase3by3_509, 3,FAIL},
    {matCase3by3_510, 3,FAIL},
    {matCase3by3_511, 3,FAIL}
};
