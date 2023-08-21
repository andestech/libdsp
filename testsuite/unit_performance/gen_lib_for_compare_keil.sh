#!/bin/sh
#set -x
# Generate the library for nds performance compare with keil using by handrun.sh
#
# Input:
#  $1: The related path of sourced configure.
#  $2: nds32le-elf or nds32be-elf
#  $3: v3m, v3m_n7, v3, v3_d10, v3_d13, v3j, v3j_d10, v3j_d13
#  $4: The option for enable unrolling.
#
# Example:
#  ./gen_lib_for_compare_keil.sh ./../nds32_libdsp/libdsp/ nds32le-elf v3_d10 0

RES="10"

BASIC_FLAGS="-ffunction-sections -fdata-sections -O3"
EMPTY_FLAGS="-include $1/testsuite/include/nds32_empty_function.h -Wno-unused-variable -Wno-unused-but-set-variable -Wno-unused-function"

################################# main ######################################
if [ "$3" == "v3_gw" ]; then
    CFLAGS="-mcpu=graywolf -funroll-loops --param max-unroll-times=4 -fsched-pressure --param sched-pressure-algorithm=2 -fno-auto-inc-dec"
    CP_MAKEFILE=0
elif [ "$3" == "v3_gwd" ] || [ "$3" == "v3s_gw_dsp" ] || [ "$3" == "v3f_gw_dsp" ] || [ "$3" == "v3_gw_dsp" ]; then
    CFLAGS="-mcpu=graywolf -mext-dsp -mext-zol -funroll-loops --param max-unroll-times=4 -fsched-pressure --param sched-pressure-algorithm=2 -fno-auto-inc-dec"
    CP_MAKEFILE=0
elif [ "$3" == "v3s_d10" ] || [ "$3" == "v3s_n10_dsp" ] || [ "$3" == "v3f_n10_dsp" ] || [ "$3" == "v3_n10_dsp" ]; then
    CFLAGS="-mcpu=d10 -mext-dsp -mext-zol"
    LIB_FOLDER="mext-dsp"
    CP_MAKEFILE=0
elif [[ $3 == "v5f_n25"* ]] || [[ $3 == "v5f_nx25"* ]]; then
    CFLAGS=""
    CP_MAKEFILE=0
elif [[ $3 == "v5_a25_nonds" ]] || [[ $3 == "v5f_a25_nonds" ]]; then
    CFLAGS="-mcpu=a25 -mext-dsp -mno-nds"
    CP_MAKEFILE=0
elif [[ $3 == "v5_ax25_nonds" ]] || [[ $3 == "v5f_ax25_nonds" ]]; then
    CFLAGS="-mcpu=ax25 -mext-dsp -mno-nds"
    CP_MAKEFILE=0
elif [[ $3 == "v5f_a25"* ]] || [[ $3 == "v5_a25"* ]]; then
    CFLAGS="-mcpu=a25 -mext-dsp"
    LIB_FOLDER="mext-dsp"
    CP_MAKEFILE=0
elif [[ $3 == "v5f_ax25"* ]] || [[ $3 == "v5_ax25"* ]]; then
    CFLAGS="-mcpu=ax25 -mext-dsp"
    LIB_FOLDER="mext-dsp"
    CP_MAKEFILE=0
else
    LIB_FOLDER=""
    CFLAGS="-mcpu=n10"
    CP_MAKEFILE=0
fi

FILE="libdsp.a"
function check_lib_exist {
    if [ "$1" != "0" ];then
        cd ${LIB_FOLDER}
    fi
    if [ ! -e "$FILE" ]; then
        echo "    $FILE not found!"
        if [ "$1" != "0" ];then
            cd ../
        fi
        return 1
    else
        if [ "$1" != "0" ];then
            cd ../
        fi
        return 0
    fi
}

if [ "$CP_MAKEFILE" == "1" ]; then
    cp ../libdsp/opt_makefile/Makefile_gw.in ../libdsp/Makefile.in
fi

LEVEL="o3"

$1/configure --host=$2 > /dev/null
rm -f *.a
make clean > /dev/null

        BUILD_FLAFS="$BASIC_FLAGS $CFLAGS"
        echo
        echo $BUILD_FLAFS

        echo "Build the libdsp.${RES}_$3_${LEVEL}.a ...."
        echo "CPPFLAGS=-DRES_LOGN=${RES} $BUILD_FLAFS"
        #make -j16 CPPFLAGS="-DRES_LOGN=${RES} $BUILD_FLAFS" CFLAGS="" > /dev/null
        cd ../libdsp/
        ./build_default_library.sh ../build_dir $2 ../build_dir
        cd ../build_dir/
        echo `pwd`
        check_lib_exist 1
        ret=$?
        if [ "$ret" == "1" ];then
            echo "Error:"
            echo "!!!Build libdsp.${RES}_$3_${LEVEL}.a ERROR! please check the log files."
            touch build_lib_fail
            exit 0
        else
            rm -f build_lib_fail
            echo "Build libdsp.${RES}_$3_${LEVEL}.a sucessfully."
        echo `pwd`
            mv ./${LIB_FOLDER}/libdsp.a libdsp.${RES}_$3_${LEVEL}.a
        fi

        echo "Build the libdsp.${RES}_$3_${LEVEL}_empty.a ...."
        rm -f empty.log
        make -j16 CPPFLAGS="-DRES_LOGN=${RES} $BUILD_FLAFS" CFLAGS="$EMPTY_FLAGS" > empty.log
        check_lib_exist 0
        ret=$?
        if [ "$ret" == "1" ];then
            echo "Error:"
            echo "!!!Build libdsp.${RES}_$3_${LEVEL}_empty.a ERROR! please check the log files."
            touch build_lib_fail
            exit 0
        else
            rm -f build_lib_fail
            echo "Build libdsp.${RES}_$3_${LEVEL}_empty.a sucessfully."
            mv libdsp.a libdsp.${RES}_$3_${LEVEL}_empty.a
            make clean > /dev/null
        fi

echo
echo "Copy the library to /testsuite/unit_performance ..."
cp libdsp.*_$3_*.a $1/testsuite/unit_performance/LIBRARY/
