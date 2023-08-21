#!/bin/sh
# Daily run for nds performance compare with keil
#
# Input:
#  $1: Option for testing GW_BOARD, D10_BOARD, SIMULATOR
#  $2: The path of gcc.log and output csv location.
#
# Example:
#  ./handrun.sh GW_BOARD /local/sharon/daily_run_perf

TARGET=$1

if [ "$TARGET" == "GW_BOARD" ];then
    BOARDS_TEST=1
    GRAYWOLF_BOARDS=1
    RV_BOARDS=0
elif [ "$TARGET" == "D10_BOARD" ];then
    BOARDS_TEST=1
    GRAYWOLF_BOARDS=0
    RV_BOARDS=0
#elif [ "$TARGET" == "N25_BOARD" ];then
elif [ "$TARGET" == "RV_BOARD" ];then
    BOARDS_TEST=1
    GRAYWOLF_BOARDS=0
    RV_BOARDS=1
elif [ "$TARGET" == "SIMULATOR" ];then
    BOARDS_TEST=0
else
    echo "Please input the target(GW_BOARD, D10_BOARD, RV_BOARD or SIMULATOR)"
    exit
fi

DSP_ROOT=`pwd`
SQA_PATH="/home/users3/sqa/BSP-latest/nds32le-elf-mculib"
RV32_PATH="/home/users3/sqa/BSP-latest/nds32le-elf-mculib"
RV64_PATH="/home/users3/sqa/BSP-latest/nds64le-elf-mculib"
#SQA_PATH="/home/users3/sqa/SW_Release/AndeSight_Linux/AndeSight-v3_2_0/toolchains-bin/nds32le-elf-mculib"
#RV32_PATH="/home/users3/sqa/SW_Release/AndeSight_Linux/AndeSight-v3_2_0/toolchains-bin/nds32le-elf-mculib"
#RV64_PATH="/home/users3/sqa/SW_Release/AndeSight_Linux/AndeSight-v3_2_0/toolchains-bin/nds64le-elf-mculib"

LIB_TAG="build_lib_fail"
rm -rf nds32.adx
mkdir -p nds32.adx
ONE_TEST=1

################################# main ######################################
if [ $BOARDS_TEST -eq 1 ];then
    if [ $RV_BOARDS -eq 1 ]; then
        LIB_TARGET="v5m_n25_board"
#        LIB_TARGET="v5m_nx25_board"
    elif [ $GRAYWOLF_BOARDS -eq 0 ];then
        LIB_TARGET="v3s_n10_dsp v3f_n10_dsp v3_n10_dsp"
    else
        LIB_TARGET="v3s_gw_dsp v3f_gw_dsp v3_gw_dsp"
    fi
elif [ $ONE_TEST -eq 1 ];then
    LIB_TARGET="v5f_a25 v3s_d10 v5f_ax25"
else
    LIB_TARGET="v3_d10 v5_n25_sid v5_nx25_sid"
fi

for LIB in $LIB_TARGET
    do
        toolchain=`echo ${LIB} | cut -d "_" -f 1`
        cpu_type=`echo ${LIB} | cut -d "_" -f 2`
        if [[ $cpu_type == "a25" ]];then
            TOOL_PREFIX="riscv32-elf"
            export PATH="${RV32_PATH}-${toolchain}/bin:$PATH"
        elif [[ $cpu_type == "ax25" ]];then
            TOOL_PREFIX="riscv64-elf"
            export PATH="${RV64_PATH}-${toolchain}/bin:$PATH"
        else
            TOOL_PREFIX="nds32le-elf"
            export PATH="${SQA_PATH}-${toolchain}/bin:$PATH"
        fi
        ${TOOL_PREFIX}-gcc -v 2>&1 | tee COMPARE_Keil_CSV/gcc-$toolchain.log

        # Generate the library for compare with Keil
        cd ../../../build_dir/
        ./../libdsp/testsuite/unit_performance/gen_lib_for_compare_keil.sh ./../libdsp ${TOOL_PREFIX} $LIB

        # Generate the data for nds32_libdsp
        if [ ! -e "$LIB_TAG" ]; then
            cd $DSP_ROOT
            mkdir -p nds32.adx/$LIB
            ./perfcount_compare_with_keil.csv.sh $LIB $2
            if [ "$2" != "" ];then
                cp COMPARE_Keil_CSV/gcc-$toolchain.log $2
            fi
        else
            echo "  library build error.."
        fi
    done
cd $DSP_ROOT
