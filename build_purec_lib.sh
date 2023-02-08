##########################
# Used for project to build pure c libdsp.a
# Input:
#  $1 : compile name, (riscv32-elf-gcc, riscv32-elf-clang, riscv64-elf-gcc,
#                       riscv64-elf-clang, gcc, arm-none-eabi-gcc)
#  $2 : Extra compile flags, default for "-O3" and some libdsp default defined
#
# Ex:
#    ./build_purec_lib.sh "riscv32-elf-gcc" "-O0"
####################################

COMPILER_NAME="$1"
EXTRA_FLAGS="$2"
LIB_ROOT=`pwd`
BUILD_LOG="$LIB_ROOT/build_lib.log"
rm -rf ${BUILD_LOG}

function log () {
    echo "$*" | tee -a ${BUILD_LOG}
}

log "=================================================="
which $COMPILER_NAME > /dev/null
if [ "$?" != 0 ];then
    log "Input 1: \"$COMPILER_NAME\" not found !!"
    log "please check your PATH !!"
    exit
else
    log "Input 1 (COMPILER_NAME) = $COMPILER_NAME"
    if [ "$EXTRA_FLAGS" != "" ];then
        log "Input 2 (EXTRA_FLAGS) = $EXTRA_FLAGS"
    fi
fi
log "=================================================="

PREFIX=`echo ${COMPILER_NAME} | rev | cut -d "-" -f1 --complement | rev`

BUILD_DIR="build_dir"
C_FLAGS="-O3 -DRES_LOGN=10 -ffunction-sections -fdata-sections -Wall -Wextra -Werror -DHERMITE_INTERPOLATION"
BUILD_FLAGS="$C_FLAGS $EXTRA_FLAGS"
INCLUDE="-I${LIB_ROOT}/include -I${LIB_ROOT}/internal"
CC="$COMPILER_NAME"
AR="$PREFIX-ar"

#### start build library
source ${LIB_ROOT}/source_file_list

mkdir -p $BUILD_DIR
cd $BUILD_DIR
rm  -rf *.o libdsp.a
for FUNC_NAME in $FUNCTION_LIST
    do
        C_NAME="`echo $FUNC_NAME | rev | cut -d '/' -f1 | rev | cut -d '.' -f1`"
        echo "${CC} ${INCLUDE} ${BUILD_FLAGS} -c -o ${C_NAME}.o ${LIB_ROOT}/${FUNC_NAME}" >> ${BUILD_LOG}
        ${CC} ${INCLUDE} ${BUILD_FLAGS} -c -o ${C_NAME}.o ${LIB_ROOT}/${FUNC_NAME}
        if [ "$?" != "0" ]; then
            log "${C_NAME}.o build fail"
            exit
        fi
    done

log "" >> ${BUILD_LOG}
ALL_OBJ_FILES=`ls *.o`
${AR} crD libdsp.a ${ALL_OBJ_FILES}
log "=================================================="
log "build libdsp.a sucess"
log "    lib: ${BUILD_DIR}/libdsp.a"
log "    build log: build_lib.log"
log "=================================================="
cd ${LIB_ROOT}
log "Demo: riscv_dsp_add_f32"
log "    ${CC} ${BUILD_FLAG} -I./include/ -o demo.adx demo.c ${BUILD_DIR}/libdsp.a"
${CC} ${BUILD_FLAG} -I./include/ -o demo.adx demo.c ${BUILD_DIR}/libdsp.a
log "=================================================="
