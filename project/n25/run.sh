########  User Change  #########
TOOLCHAIN_PATH="/home/users3/si/BSP-latest/nds32le-elf-mculib-v5f/bin"

ENA_BUILD_LIB=1
ENA_ACCURACY=1
ENA_PERF=0
ENA_LDSP=0
ENA_DEBUG=0
ENA_FPGA=0
## you can check item in ../test_list, for accuracy test
UNIT_TEST_FUNC="basic/twoin_f32/t_add_f32.c"
## you can check item in gen_perf_benchmarks.sh, for perf test
UNIT_PERF_FUNC=""
###############################
LOCAL=`pwd`
cd ../
BUILD_PATH=`pwd`
cd ../
LIB_ROOT=`pwd`
cd $LOCAL
export PATH=${LOCAL}:${TOOLCHAIN_PATH}:${PATH}

EXTRA_FLAGS="$1"
COMPILER_NAME="riscv32-elf-gcc"
SIM_WRAPPER="riscv-sim-wrapper"
CPU_FLAGS="-mcpu=n25"
CFLAGS="${CPU_FLAGS} -fno-math-errno $EXTRA_FLAGS"
ACCURACY_LFLAGS=""
LIB_PATH="${BUILD_PATH}/build_dir/libdsp.a"

if [ ${ENA_DEBUG} -eq 1 ];then
    EXTRA_FLAGS="$EXTRA_FLAGS -DENA_DEBUG"
    if [ ${ENA_ACCURACY} -eq 1 ];then
        if [ "$UNIT_TEST_FUNC" == "" ]; then
            echo "Error: UNIT_TEST_FUNC is empty !!"
            exit
        fi
        export UNIT_TEST_LIST="$UNIT_TEST_FUNC"
    fi
    if [ ${ENA_PERF} -eq 1 ];then
        if [ "$UNIT_PERF_FUNC" == "" ]; then
            echo "Error: UNIT_PERF_FUNC is empty !!"
            exit
        fi
        export PERF_TEST_LIST="$UNIT_PERF_FUNC"
    fi
fi

if [ ${ENA_FPGA} -eq 1 ];then
    EXTRA_FLAGS="$EXTRA_FLAGS -mvh"
    SIM_WRAPPER="riscv-sim-wrapper-on-board"
    TARGET="BOARD"
else
    EXTRA_FLAGS="$EXTRA_FLAGS"
    SIM_WRAPPER="riscv-sim-wrapper"
    TARGET="SID"
fi

if [ ${ENA_LDSP} -eq 1 ];then
    ENA_BUILD_LIB=0
    LIB_PATH="-ldsp $CPU_FLAGS"
    PERF_LFLAGS=""
else
    LIB_PATH="${BUILD_PATH}/build_dir/libdsp.a"
    PERF_LFLAGS="-DENA_BUILD_LIB"

fi


####### Build lib
if [ ${ENA_BUILD_LIB} -eq 1 ];then
    cd ../
    echo "Start build libdsp.a ${CFLAGS} ${EXTRA_FLAGS} ..."
    ./build_lib.sh "${COMPILER_NAME}" "${CFLAGS} ${EXTRA_FLAGS}"
    if [ ! -e "${LIB_PATH}" ]; then
        echo "Build libdsp.a Fail: not found !!"
        exit
    else
        echo "Build libdsp.a Ok !!"
        echo "    lib: ${LIB_PATH}"
    fi
    echo "    log: ${BUILD_PATH}/build_dir/build.log"
    cd $LOCAL
fi

##### Check accuracy
if [ ${ENA_ACCURACY} -eq 1 ];then
    echo "Start check accuracy for ${LIB_PATH} ..."
    rm -f accuracy.log
    pwd
    ../run_accuracy.sh "${COMPILER_NAME}" "${CFLAGS} ${ACCURACY_LFLAGS} ${EXTRA_FLAGS}" "${LIB_PATH}" ${SIM_WRAPPER} | tee -a accuracy.log
fi

##### Evaluate performance
if [ ${ENA_PERF} -eq 1 ];then
    echo "Start evaluate performane for ${LIB_PATH} ..."
    rm -rf perf.log perf_adx
    mkdir -p perf_adx
    ../../testsuite/unit_performance/gen_perf_benchmarks.sh "${COMPILER_NAME}" "${SIM_WRAPPER}" "${TARGET}" "${CPU_FLAGS} ${PERF_LFLAGS} ${EXTRA_FLAGS}"
    mv test_sh*.log perf.log
    cp -rf test_sh*/* perf_adx
    rm -rf test_sh*
fi
