TOOLCHAIN_PATH="/home/users3/si/BSP-latest/nds32le-elf-mculib-v5f/bin"

LOCAL=`pwd`
cd ../
BUILD_PATH=`pwd`
cd ../
LIB_ROOT=`pwd`
cd $LOCAL

export PATH=${LOCAL}:${TOOLCHAIN_PATH}:${PATH}
export SID="/home/users3/zero/myWork/daily_run_perf/nds32_libdsp/libdsp/testsuite/unit_performance/sid"

ENA_BUILD_LIB=1
ENA_ACCURACY=1
ENA_DEBUG=0
## you can check item in ../test_list, for accuracy test
UNIT_TEST_FUNC="utils/convert/t_convert_f32_q31.c "

EXTRA_FLAGS="$1"
COMPILER_NAME="gcc"
SIM_WRAPPER=""
CPU_FLAGS=""
CFLAGS="${CPU_FLAGS} $EXTRA_FLAGS"
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
    elif [ ${ENA_PERF} -eq 1 ];then
        export PERF_TEST_LIST="$UNIT_PERF_FUNC"
    fi
fi

LIB_PATH="${BUILD_PATH}/build_dir/libdsp.a"
PERF_LFLAGS="-DENA_BUILD_LIB"


####### main
## Build libdsp
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

if [ ${ENA_ACCURACY} -eq 1 ];then
    echo "Start check accuracy for ${LIB_PATH} ..."
    rm -f accuracy.log
    ../run_accuracy.sh "${COMPILER_NAME}" "${CFLAGS} ${ACCURACY_LFLAGS} ${EXTRA_FLAGS}" "${LIB_PATH}" ${SIM_WRAPPER} | tee -a accuracy.log
fi
