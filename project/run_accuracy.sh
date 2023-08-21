##########################
# Used for project to check libdsp.a accuracy
# Input:
#  $1 : compile name, (riscv32-elf-gcc, riscv32-elf-clang, riscv64-elf-gcc, riscv64-elf-clang)
#  $2 : Extra compile flags, default only for -O3 and some libdsp default defined
#  $3 : path for library, "/build_dir/libdsp.a" or "-ldsp"
#  $4 : simulation wrapper
#
# Ex:
#    ./build_lib.sh "riscv32-elf-gcc" "-O3 -mext-dsp" "build_dir/libdsp.a" "riscv-sim-wrapper"
###########################

COMPILER_NAME="$1"
EXTRA_FLAGS="$2"
LIB_PATH="$3"
SIM_WRAPPER="$4"

CUR_DIR=`pwd`
TESTSUITE_PATH=$(readlink -f `dirname ${0}`)
cd ${TESTSUITE_PATH}
cd ../
SRC_DIR=`pwd`
cd $CUR_DIR

if [[ $EXTRA_FLAGS == *"-mzfh"* ]]; then
    ENA_FP16=1
fi

if [[ $EXTRA_FLAGS == *"-DENA_DEBUG"* ]]; then
    TESTSUITE_LIST="$UNIT_TEST_LIST"
else
    source ${TESTSUITE_PATH}/test_list
fi


CC="$COMPILER_NAME"
TOOLCHAIN_PREFIX=`echo ${COMPILER_NAME} | rev | cut -d "-" -f1 --complement | rev`

C_FLAGS="-O3 -DRES_LOGN=10 -Wall -Werror"
INCLUDE="-I$CUR_DIR -I$SRC_DIR/include -I$SRC_DIR/testsuite/include -include$SRC_DIR/testsuite/include/nds32_pfcounter.h"
BUILD_FLAGS="${C_FLAGS} ${EXTRA_FLAGS}"

#### start check library
mkdir -p test_dir
cd test_dir
rm -f accuracy_fail_list

TEST_FAIL=0

function run_and_check {
        CFLAGS="$BUILD_FLAGS $TRANS_FLAG"
        if [ "$1" != "" ]; then
            BIN_FILE="${1}"
        else
            BIN_FILE="${BIN}"
        fi
        echo "============== test $BIN_FILE =============="
        echo "${CC} ${INCLUDE} ${CFLAGS} -DGETENV_SRCDIR=\"${SRC_DIR}/testsuite/$SRC_FOLDER\" -o ${BIN_FILE} ${SRC_DIR}/testsuite/${SRC_FOLDER}/${BIN_FILE}.c ${LIB_PATH} -lm"
        ${CC} ${INCLUDE} ${CFLAGS} -DGETENV_SRCDIR=\"${SRC_DIR}/testsuite/$SRC_FOLDER\" -o ${BIN_FILE} ${SRC_DIR}/testsuite/${SRC_FOLDER}/${BIN_FILE}.c ${LIB_PATH} -lm
        if [[ $EXTRA_FLAGS == *"-DENA_DUMP_ACCUARCY_TO_CSV"* ]]; then
            CASE_N="`echo ${BIN_FILE} | cut -d '_' -f1 --complement`"
            ${SIM_WRAPPER} ${BIN_FILE}  | tee temp_${CASE_N}.log
            echo -n riscv_dsp_${CASE_N}"," >> log_accuracy.csv
            cat temp_${CASE_N}.log |  grep -m 1 "MAE is" | awk '{printf "%10f, %10f, %10f, %10f, %10f\n", $3, $6, $9, $12, $15}' >> log_accuracy.csv
        else
            ${SIM_WRAPPER} ${BIN_FILE}
            sim_ret="$?"
        fi
        if [ "$sim_ret" != "0" ]; then
            echo "${BIN_FILE} ${LOGN} fail" | tee -a accuracy_fail_list
            TEST_FAIL=1
        else
            echo "${BIN_FILE} ${LOGN} pass"
        fi
        echo ""
}

for UNIT in $TESTSUITE_LIST
do
    SRC_FOLDER="`echo $UNIT | rev | cut -d '/' -f1 --complement | rev`"
    BIN="`echo $UNIT | rev | cut -d '/' -f1 | rev | cut -d '.' -f1`"
    TRANS_FLAG=""
    if [[ "$SRC_FOLDER" = *"cfft" ]]; then
        for LOGN in 3 4 5 6 7 8 9 10
        do
            echo "======= test LOGN = $LOGN  ========="
            TRANS_FLAG="-DTRANSFORM_SAMPLES_LOG=$LOGN"
            run_and_check "t_cfft_f64"
            run_and_check "t_cfft_f32"
            run_and_check "t_cfft_q15"
            run_and_check "t_cfft_q31"
        done
    elif [[ "$SRC_FOLDER" = *"rfft"* ]]; then
        for LOGN in 4 5 6 7 8 9 10
        do
            echo "======= test LOGN = $LOGN  ========="
            TRANS_FLAG="-DTRANSFORM_SAMPLES_LOG=$LOGN"
            run_and_check "t_rfft_f32"
            run_and_check "t_rfft_q15"
            run_and_check "t_rfft_q31"
            run_and_check "t_rfft_f64"
        done
    elif [[ "$SRC_FOLDER" = *"dct4"* ]]; then
        for LOGN in 3 4 5 6 7
        do
            echo "======= test LOGN = $LOGN  ========="
            TRANS_FLAG="-DTRANSFORM_SAMPLES_LOG=$LOGN"
            run_and_check "t_dct4_f32"
            run_and_check "t_dct4_q15"
            run_and_check "t_dct4_q31"
        done
    elif [[ "$SRC_FOLDER" = *"/dct" ]]; then
        for LOGN in 3 4 5 6 7 8
        do
            echo "======= test LOGN = $LOGN  ========="
            TRANS_FLAG="-DTRANSFORM_SAMPLES_LOG=$LOGN"
            run_and_check "t_dct_f32"
            run_and_check "t_dct_q15"
            run_and_check "t_dct_q31"
        done
    else
        LOGN=""
        run_and_check
    fi
done

echo "============================"
if [ "$TEST_FAIL" == "0" ]; then
    echo "All test pass"
else
    echo "test fail list:"
    echo "`cat accuracy_fail_list`"
    cp accuracy_fail_list $CUR_DIR/
fi
echo "============================"
