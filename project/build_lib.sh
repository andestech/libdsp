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
PREFIX="${COMPILER_NAME%-*}"

EXTRA_FLAG="$2"
CUR_DIR="$(readlink -f `dirname ${0}`)"
cd $CUR_DIR
cd ../
SRC_DIR=`pwd`
cd $CUR_DIR

CC="$COMPILER_NAME"
AR="$PREFIX-ar"
RANLIB="$PREFIX-ranlib"

if [[ $EXTRA_FLAG == *"-mzfh"* ]]; then
    ENA_FP16=1
fi

DEFINE_RES_LOG=10

VERSION_C="${SRC_DIR}/nds_version.c"
if [ ! -f ${VERSION_C} ]; then
    cd ${SRC_DIR}
    ./nds_autogen_info.sh libdsp
    cd $CUR_DIR
fi

source $SRC_DIR/source_file_list

BUILD_DIR="build_dir"
C_FLAGS="-O3 -DRES_LOGN=$DEFINE_RES_LOG -frandom-seed=libdsp -ffunction-sections -fdata-sections -Wall -Wextra -Werror -Wno-narrowing -DPF_COUNTER -DHERMITE_INTERPOLATION"
BUILD_FLAGS="$C_FLAGS $EXTRA_FLAG"
INCLUDE="-I$CUR_DIR -I$SRC_DIR/include -I$SRC_DIR/internal"

function Build_OBJ () {
    SRC_FNAME=${1}
    C_NAME="`echo ${SRC_FNAME} | rev | cut -d '/' -f1 | rev | cut -d '.' -f1`"
    echo "${CC} ${INCLUDE} ${BUILD_FLAGS} -c -o ${C_NAME}.o ${SRC_DIR}/${SRC_FNAME}" | tee -a build.log
    ${CC} ${INCLUDE} ${BUILD_FLAGS} -c -o ${C_NAME}.o ${SRC_DIR}/${SRC_FNAME}
    if [ "$?" != "0" ]; then
        echo "${C_NAME}.o build fail"
        exit
    fi
}

#### start build library
mkdir -p $BUILD_DIR
cd $BUILD_DIR
rm -rf build.log *.o libdsp.a

## Check bash verion to decide parallel building
REQ_BASH_MAJOR=5
BASH_MAJOR=`bash --version | head -n1 | cut -d' ' -f4 | cut -d'.' -f1`
PARALLEL_BUILD="false"
if { [ $BASH_MAJOR -gt $REQ_BASH_MAJOR ] || [ $BASH_MAJOR -eq $REQ_BASH_MAJOR ]; } then
    PARALLEL_BUILD="true"
fi
echo "[INFO] Parallel building': $PARALLEL_BUILD" >> build.log

if [ $PARALLEL_BUILD == "true" ]; then
    N=16
    for FUNC_NAME in $FUNCTION_LIST;do
        {
            # .. do your stuff here
            Build_OBJ "${FUNC_NAME}"
        } &

        # allow to execute up to $N jobs in parallel
        if [[ $(jobs -r -p | wc -l) -ge $N ]]; then
            # now there are $N jobs already running, so wait here for any job
            # to be finished so there is a place to start next one.
            wait -n
        fi
        done
else
    for FUNC_NAME in $FUNCTION_LIST;do
        Build_OBJ "${FUNC_NAME}"

        if [[ $? -ne 0 ]]; then
            ret = $?
            printf "!!!\n[ERROR] build error! building is terminated.\n!!!\n"
            exit $((ret))
        fi
    done
fi

# no more jobs to be started but wait for pending jobs
# (all need to be finished)
wait

echo "" >> build.log
ALL_OBJ_FILES=`ls *.o`
echo "${AR} crD libdsp.a ${ALL_OBJ_FILES}" >> build.log
${AR} crD libdsp.a ${ALL_OBJ_FILES}
${RANLIB} -D libdsp.a
echo "==================================================" >> build.log
echo "build libdsp.a sucess" >> build.log
echo "==================================================" >> build.log
