##########################
# Used for project to build libdsp.a
# Input:
#  $1 : compile name, (riscv32-elf-gcc, riscv32-elf-clang, riscv64-elf-gcc, riscv64-elf-clang)
#  $2 : Extra compile flags, default only for -O3 and some libdsp default defined
#
# Ex:
#    ./build_lib.sh "riscv32-elf-gcc" "-O3 -mext-dsp"
###########################

COMPILER_NAME="$1"
COMPILER_TYPE="`echo ${COMPILER_NAME} | cut -d '-' -f3`"
PREFIX=`echo ${COMPILER_NAME} | rev | cut -d "-" -f1 --complement | rev`

EXTRA_FLAG="$2"
CUR_DIR="$(readlink -f `dirname ${0}`)"
cd $CUR_DIR
cd ../
SRC_DIR=`pwd`
cd $CUR_DIR

CC="$COMPILER_NAME"
AR="$PREFIX-ar"

if [[ $EXTRA_FLAG == *"-mzfh"* ]]; then
    ENA_FP16=1
fi

DEFINE_RES_LOG=10
source $SRC_DIR/source_file_list

BUILD_DIR="build_dir"
C_FLAGS="-O3 -DRES_LOGN=$DEFINE_RES_LOG -frandom-seed=libdsp -ffunction-sections -fdata-sections -Wall -Wextra -Werror -DPF_COUNTER -DHERMITE_INTERPOLATION"
BUILD_FLAGS="$C_FLAGS $EXTRA_FLAG"
INCLUDE="-I$CUR_DIR -I$SRC_DIR/include -I$SRC_DIR/internal"

#### start build library
mkdir -p $BUILD_DIR
cd $BUILD_DIR
rm -rf build.log *.o libdsp.a
for FUNC_NAME in $FUNCTION_LIST
    do
        C_NAME="`echo $FUNC_NAME | rev | cut -d '/' -f1 | rev | cut -d '.' -f1`"
        echo "${CC} ${INCLUDE} ${BUILD_FLAGS} -c -o ${C_NAME}.o ${SRC_DIR}/${FUNC_NAME}" | tee -a build.log
        ${CC} ${INCLUDE} ${BUILD_FLAGS} -c -o ${C_NAME}.o ${SRC_DIR}/${FUNC_NAME}
        if [ "$?" != "0" ]; then
            echo "${C_NAME}.o build fail"
            exit
        fi
    done

echo "" >> build.log
ALL_OBJ_FILES=`ls *.o`
echo "${AR} crD libdsp.a ${ALL_OBJ_FILES}" >> build.log
${AR} crD libdsp.a ${ALL_OBJ_FILES}
echo "==================================================" >> build.log
echo "build libdsp.a sucess" >> build.log
echo "==================================================" >> build.log
