#! /usr/bin/env bash
# Unit testing for cycle and size
#
# Input:
#  $1: xxx testcase name for t_pf_xxx.c
#  $2: target test. ex: d15, d10, lm
#
# Example:
#  ./run_perf.sh add_f32 lm
#  ./run_perf.sh dot_prod_q15 d10
#  ./run_perf.sh abs_q31 d15

###########################################################################
mkdir -p ISA_ADX
LIBRARY_PATH="LIBRARY/libdsp.a"
EMPTY_LIBRARY_PATH="LIBRARY/libdsp_empty.a"
USE_LDSP=0
TOOLCHAIN_PATH="/home/users3/si/BSP-latest"
###########################################################################
CC=nds32le-elf-gcc
SZ=nds32le-elf-size
OBJ=nds32le-elf-objdump
DSP_FLAGS="-mext-dsp -mext-zol"

tc=$2
if [ "$tc" == "d15" ];then
sidtool=nds32-sim-wrapper-d15
#SID_FLAG="-mvh -T nds32.ld"
TOOLCHAIN="nds32le-elf-mculib-v3"
elif [ "$tc" == "lm" ];then
sidtool=nds32-sim-wrapper-on-board
SID_FLAG="-mvh -T nds32.ld"
elif [ "$tc" == "lm2" ];then
sidtool=nds32-sim-wrapper-on-board
SID_FLAG="-mvh -T nds32_ro_ilm.ld"
elif [ "$tc" == "cache" ];then
sidtool=nds32-sim-wrapper-on-board-cache
SID_FLAG="-mvh"
elif [ "$tc" == "d10" ];then
sidtool=nds32-sim-wrapper-d1088-v3
TOOLCHAIN="nds32le-elf-mculib-v3"
#LIBRARY_PATH="-ldsp -mext-dsp"
elif [ "$tc" == "a25" ];then
sidtool=riscv32-sim-wrapper-d25-v5
CC=riscv32-elf-gcc
SZ=riscv32-elf-size
OBJ=riscv32-elf-objdump
DSP_FLAGS="-mext-dsp -mcpu=a25"
#SID_FLAG="-T ae350-xip.ld"
TOOLCHAIN="nds32le-elf-mculib-v5f"
elif [ "$tc" == "d45" ];then
sidtool=riscv32-sim-wrapper-d45-v5
CC=riscv32-elf-gcc
SZ=riscv32-elf-size
OBJ=riscv32-elf-objdump
DSP_FLAGS="-mext-dsp -mcpu=d45"
#SID_FLAG="-T ae350-xip.ld"
TOOLCHAIN="nds32le-elf-mculib-v5f"
elif [ "$tc" == "a25board" ];then
#sidtool=riscv32-sim-wrapper-on-board
sidtool=riscv32-sim-wrapper-boardfarm
CC=riscv32-elf-gcc
SZ=riscv32-elf-size
OBJ=riscv32-elf-objdump
DSP_FLAGS="-mext-dsp -mcpu=a25"
SID_FLAG="-mvh -T ae350-xip.ld"
TOOLCHAIN="nds32le-elf-mculib-v5f"
elif [ "$tc" == "d45board" ];then
sidtool=riscv32-sim-wrapper-on-board
#sidtool=riscv32-sim-wrapper-boardfarm
CC=riscv32-elf-gcc
SZ=riscv32-elf-size
OBJ=riscv32-elf-objdump
DSP_FLAGS="-mext-dsp -mcpu=d45"
SID_FLAG="-mvh -T ae350-xip.ld"
TOOLCHAIN="nds32le-elf-mculib-v5f"
elif [ "$tc" == "n25board" ];then
sidtool=riscv32-sim-wrapper-on-board
CC=riscv32-elf-gcc
SZ=riscv32-elf-size
OBJ=riscv32-elf-objdump
DSP_FLAGS=""
SID_FLAG="-mvh -T ae350-xip.ld"
TOOLCHAIN="nds32le-elf-mculib-v5f"
elif [ "$tc" == "ax25" ];then
sidtool=riscv64-sim-wrapper-dx25-v5
CC=riscv64-elf-gcc
SZ=riscv64-elf-size
OBJ=riscv64-elf-objdump
DSP_FLAGS="-mext-dsp"
TOOLCHAIN="nds64le-elf-mculib-v5f"
#SID_FLAG=" -T ae350-xip.ld"
elif [ "$tc" == "ax45" ];then
sidtool=riscv64-sim-wrapper-ax45-v5
CC=riscv64-elf-gcc
SZ=riscv64-elf-size
OBJ=riscv64-elf-objdump
DSP_FLAGS="-mext-dsp"
TOOLCHAIN="nds64le-elf-mculib-v5f"
#SID_FLAG=" -T ae350-xip.ld"
elif [ "$tc" == "nx25" ];then
sidtool=riscv64-sim-wrapper-nx25-v5
CC=riscv64-elf-gcc
SZ=riscv64-elf-size
OBJ=riscv64-elf-objdump
DSP_FLAGS=""
SID_FLAG=" -T ae350-xip.ld"
TOOLCHAIN="nds64le-elf-mculib-v5f"
elif [ "$tc" == "ax25board" ];then
sidtool=riscv64-sim-wrapper-on-board
#sidtool=riscv32-sim-wrapper-boardfarm
CC=riscv64-elf-gcc
SZ=riscv64-elf-size
OBJ=riscv64-elf-objdump
SID_FLAG="-mvh -T ae350-xip.ld"
DSP_FLAGS="-mext-dsp"
TOOLCHAIN="nds64le-elf-mculib-v5f"
elif [ "$tc" == "ax45board" ];then
sidtool=riscv64-sim-wrapper-on-board
#sidtool=riscv32-sim-wrapper-boardfarm
CC=riscv64-elf-gcc
SZ=riscv64-elf-size
OBJ=riscv64-elf-objdump
SID_FLAG="-mvh -T ae350-xip.ld"
DSP_FLAGS="-mext-dsp"
TOOLCHAIN="nds64le-elf-mculib-v5f"
elif [ "$tc" == "nx25board" ];then
sidtool=riscv64-sim-wrapper-on-board
sidtool=riscv32-sim-wrapper-boardfarm
CC=riscv64-elf-gcc
SZ=riscv64-elf-size
OBJ=riscv64-elf-objdump
SID_FLAG="-mvh -T ae350-xip.ld"
DSP_FLAGS=""
TOOLCHAIN="nds64le-elf-mculib-v5f"
elif [ "$tc" == "n25" ];then
sidtool=riscv32-sim-wrapper-n25-v5
CC=riscv32-elf-gcc
SZ=riscv32-elf-size
OBJ=riscv32-elf-objdump
DSP_FLAGS="-mcpu=n25"
SID_FLAG=" -T ae350-xip.ld"
TOOLCHAIN="nds32le-elf-mculib-v5f"
elif [ "$tc" == "n13" ];then
sidtool=nds32-sim-wrapper-d1337-v3
else
echo "Please input SID option in param2 (d15 lm cache d10)"
exit
fi

USED_TOOLCHAIN=$TOOLCHAIN_PATH/$TOOLCHAIN/bin:"$PATH"
export PATH=:$USED_TOOLCHAIN

if [ "$USE_LDSP" == "1" ];then
    LIBRARY_PATH="-ldsp ${DSP_FLAGS}"
fi


#DEBUG_FLAG="-I./../../include -include ../include/t_pf_perf.h -Wno-unused-but-set-variable -Wno-unused-variable"
DEBUG_FLAG="-I./../../include -include ../include/t_pf_perf.h"
BASIC_FLAG="-DRES_LOGN=10 -Wl,--gc-sections -O3 ${DSP_FLAGS}"
CFLAGS="$BASIC_FLAG $SID_FLAG"
###########################################################################
which ${CC}
echo "----- function -----"
echo "${CC} -DPF_COUNTER ${CFLAGS} ${DEBUG_FLAG} -o ISA_ADX/$1_$2_pf.adx t_pf_$1.c ${LIBRARY_PATH} -lm"
#${CC} -DPF_COUNTER ${CFLAGS} ${DEBUG_FLAG} -o ISA_ADX/$1_$2_pf.adx t_pf_$1.c ${LIBRARY_PATH} -lm -Wl,-Map=map,--cref
${CC} -DPF_COUNTER ${CFLAGS} ${DEBUG_FLAG} -o ISA_ADX/$1_$2_pf.adx t_pf_$1.c ${LIBRARY_PATH} -lm
${OBJ} -S ISA_ADX/$1_$2_pf.adx > ISA_ADX/$1_$2.log
${sidtool} ISA_ADX/$1_$2_pf.adx
${SZ} ISA_ADX/$1_$2_pf.adx
echo "----- ISA_ADX/$1_$2.log -------------"

if [ -f "${EMPTY_LIBRARY_PATH}" ]; then
echo "----- Empty function size ----------- ISA_ADX/$1_$2.log -------------"
${CC} -DPF_COUNTER -include ./../../testsuite/include/nds32_empty_function.h ${CFLAGS} ${DEBUG_FLAG} -o ISA_ADX/$1_$2_sz.adx t_pf_$1.c ${EMPTY_LIBRARY_PATH} -lm
${SZ} ISA_ADX/$1_$2_sz.adx
fi

###########################################################################
