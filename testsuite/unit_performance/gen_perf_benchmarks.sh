## ./find_cflags.sh $1
# $1: cpu type
# $2: sim wrapper
###################################################
COMPILER_NAME="$1"
SIM_WRAPPER="$2"
TARGET="$3"
EXTRA_CFLAGS="$4"
####################################################
BASIC=1
COMPLEX=1
CONTROLLER=1
FILTERING=1
MATRIX=1
STATISTICS=1
TRANSFORM=1
UTILS=1
SVM=1
DISTANCE=1
SORT=1
NDS_ONLY=1
####################################################
CURRENT_PATH=`pwd`
SCRIPT_PATH=$(readlink -f `dirname ${0}`)

TOOLCHAIN_PREFIX=`echo ${COMPILER_NAME} | rev | cut -d "-" -f1 --complement | rev`
echo $TOOLCHAIN_PREFIX
OBJ="${TOOLCHAIN_PREFIX}-objdump"
SZ="${TOOLCHAIN_PREFIX}-size"

cd ${SCRIPT_PATH}
cd ../../
LIBROOT_PATH=`pwd`
cd ${CURRENT_PATH}
cd ../
WRAPPER_PATH=`pwd`
cd ${CURRENT_PATH}

export PATH=${WRAPPER_PATH}:${CURRENT_PATH}:$PATH

TIME_STR=`date +%Y%m%d%H%M%S`
TEST_LOG="test_sh_${TIME_STR}.log"
NEW_LOG_DIR="test_sh_${TIME_STR}"
mkdir -p $NEW_LOG_DIR
mkdir -p $NEW_LOG_DIR/adx
cd $NEW_LOG_DIR/adx
ADX_PATH="${NEW_LOG_DIR}/adx"
cd $CURRENT_PATH

LD_FLAG="-T ${SCRIPT_PATH}/ae350-xip.ld"
if [ "$TARGET" == "BOARD" ];then
    LD_FLAG="-mvh $LD_FLAG"
fi

CPU_FLAGS="$EXTRA_CFLAGS"
if [[ $EXTRA_CFLAGS == *"-DENA_BUILD_LIB"* ]]; then
    LIBRARY_PATH="${LIBROOT_PATH}/project/build_dir/libdsp.a"
else
    LIBRARY_PATH="-ldsp $CPU_FLAGS"
fi

BASIC_FLAGS="-ffunction-sections -fdata-sections -O3 -DRES_LOGN=10"
DEBUG_FLAG="-DRES_LOGN=10 -I${LIBROOT_PATH}/include -include ${LIBROOT_PATH}/testsuite/include/t_pf_perf.h  -Wl,--gc-sections -O3"

if [[ $EXTRA_CFLAGS == *"-DENA_SIZE"* ]]; then
    ENA_SIZE=1
else
    ENA_SIZE=0
fi

ADX_COUNT=0
F16_ADX_COUNT=0

EMPTY_LIBRARY_PATH="${LIBROOT_PATH}/project/libdsp_empty.a"

function gen_size_after_astv520 {
    FUNC=$1
    ${SZ} -A ${ADX_PATH}/${FUNC}_pf.adx > size.log
    ${COMPILER_NAME} -DPF_COUNTER ${LFLAGS} -o ${ADX_PATH}/$1_empty.adx ${SCRIPT_PATH}/t_pf_$1.c ${EMPTY_LIBRARY_PATH} -lm
    ${SZ} -A ${ADX_PATH}/$1_empty.adx > empty.log
    paste size.log empty.log > tt
    code_val=`cat tt | grep "text" | awk '{printf "%u\n", $2-$5}'`
    rodata_val=`cat tt | grep "rodata" | awk '{printf "%u\n", $2-$5}'`
    eh_frame_val=`cat tt | grep "eh_frame" | awk '{printf "%u\n", $2-$5}'`
    rodata_val=$((rodata_val+eh_frame_val))
    data_val=`cat tt | grep "\.data" | awk '{printf "%u\n", $2-$5}'`
    sdata_val=`cat tt | grep "\.sdata" | awk '{printf "%u\n", $2-$5}'`
    data_val=`echo $data_val | awk '{printf "%u\n", $1+$2}'`
    data_val=$((data_val+sdata_val))
    bss_val=`cat tt | grep "bss"  | awk '{printf "%u\n", $2-$5}'`
    bss_val=`echo $bss_val | awk '{printf "%u\n", $1+$2+$3+$4+$5}'`
    text_val=$((code_val+2+rodata_val))
    echo "$text_val, $((code_val+2)), $rodata_val, $data_val, $bss_val, " >> $output_log
    #rm -rf tt size.log empty.log
}

## The function only support in oldest toolchain
function gen_size {
    FUNC=$1
    ${SZ} ${ADX_PATH}/${FUNC}_pf.adx > size.log
    ${COMPILER_NAME} -DPF_COUNTER ${LFLAGS} -o ${ADX_PATH}/$1_empty.adx ${SCRIPT_PATH}/t_pf_$1.c ${EMPTY_LIBRARY_PATH} -lm
    ${SZ} ${ADX_PATH}/$1_empty.adx > empty.log
    paste size.log empty.log > tt
    #cat tt | grep "adx" | awk '{printf "%u, %u, %u, %u\n", $1-$7+8, $2-$8+8, $3-$9, $4-$10}'  >> $output_log
    cat tt | grep "adx" | awk '{printf "%u, %u, %u, %u\n", $1-$7+2, $2-$8, $3-$9, $4-$10+2}'  >> $output_log
    #rm -rf tt size.log empty.log
}

function run {
    ADX_COUNT=$((ADX_COUNT+1))
    if [ "$2" == "" ];then
        echo "$1 ...."
    else
        echo "$1($2) ...."
    fi
    LFLAGS="$DEBUG_FLAG ${CPU_FLAGS} ${LD_FLAG}"
    echo "${COMPILER_NAME} -DPF_COUNTER ${LFLAGS} -o ${ADX_PATH}/$1_pf.adx ${SCRIPT_PATH}/t_pf_$1.c ${LIBRARY_PATH} -lm" >> ${TEST_LOG}
    #${COMPILER_NAME} -DPF_COUNTER ${LFLAGS} -o ${ADX_PATH}/$1_pf.adx ${SCRIPT_PATH}/t_pf_$1.c ${LIBRARY_PATH} -lm -Wl,-Map=$1.map
    ${COMPILER_NAME} -DPF_COUNTER ${LFLAGS} -o ${ADX_PATH}/$1_pf.adx ${SCRIPT_PATH}/t_pf_$1.c ${LIBRARY_PATH} -lm
    ${SIM_WRAPPER} ${ADX_PATH}/$1_pf.adx > log
    ${OBJ} -S ${ADX_PATH}/$1_pf.adx > ${ADX_PATH}/$1_pf.objdump

    cat log >> ${TEST_LOG}
    if [ "$2" == "" ];then
        echo -n riscv_dsp_$1"," >> $output_log
    else
        echo -n riscv_dsp_$1\($2\)"," >> $output_log
    fi
    if [ "$ENA_SIZE" == "1" ]; then
        cat log | sed  "s/\r//" | grep "The cycle count" | awk '{printf "%u,", $5}' >> $output_log
        gen_size_after_astv520 "$1"
    else
        cat log | sed  "s/\r//" | grep "The inst count" | awk '{printf "%d, ", $5}' >> $output_log
        cat log | sed  "s/\r//" | grep "The cycle count" | awk '{print $5}' >> $output_log
    fi
}

function run_f16 {
    ADX_COUNT=$((ADX_COUNT+1))
    F16_ADX_COUNT=$((F16_ADX_COUNT+1))
    echo "$1 ...."
    if [[ $EXTRA_CFLAGS == *"-mtune=andes-45-series"* ]]; then
        LFLAGS="$DEBUG_FLAG ${CPU_FLAGS} ${LD_FLAG}"
        echo "${COMPILER_NAME} -DPF_COUNTER ${LFLAGS} -o ${ADX_PATH}/$1_pf.adx ${SCRIPT_PATH}/t_pf_$1.c ${LIBRARY_PATH} -lm" >> ${TEST_LOG}
        ${COMPILER_NAME} -DPF_COUNTER ${LFLAGS} -o ${ADX_PATH}/$1_pf.adx ${SCRIPT_PATH}/t_pf_$1.c ${LIBRARY_PATH} -lm
        ${SIM_WRAPPER} ${ADX_PATH}/$1_pf.adx > log
        ${OBJ} -S ${ADX_PATH}/$1_pf.adx > ${ADX_PATH}/$1_pf.objdump

        cat log >> ${TEST_LOG}
        echo -n riscv_dsp_$1"," >> $output_log
        cat log | sed  "s/\r//" | grep "The cycle count" | awk '{print $5}' >> $output_log
    else
        echo "riscv_dsp_$1, x," >> $output_log

    fi
}

function gen_perf {
    if [ "$BASIC" == "1" ];then
        ## Basic ##
        for SRC in \
            abs_f32 abs_q15 abs_q31 abs_q7 \
            add_f32 add_q15 add_q31 add_q7 add_u8_u16 \
            dprod_f32 dprod_q15 dprod_q31 dprod_q7 dprod_u8 dprod_u8xq15 dprod_q7xq15 \
            mul_f32 mul_q15 mul_q31 mul_q7 mul_u8_u16 \
            neg_f32 neg_q15 neg_q31 neg_q7 \
            offset_f32 offset_q15 offset_q31 offset_q7 offset_u8 \
            scale_f32 scale_q15 scale_q31 scale_q7 scale_u8 \
            shift_q15 shift_q31 shift_q7 shift_u8 \
            sub_f32 sub_q15 sub_q31 sub_q7 sub_u8_q7 \
            and_u16 and_u32 and_u8 \
            not_u16 not_u32 not_u8 \
            or_u16 or_u32 or_u8 \
            xor_u16 xor_u32 xor_u8 \
            clip_f32 clip_q15 clip_q31 clip_q7 \
            divide_q15 divide_q31 div_f32 div_q31 div_u64_u32 div_s64_u32
        do
            run ${SRC}
        done
        #echo "" >> $output_log
    fi

    if [ "$COMPLEX" == "1" ];then
        ## Complex ##
        for SRC in \
            cconj_f32 cconj_q15 cconj_q31 \
            cmag_f32 cmag_q15 cmag_hp_q15 cmag_q31 \
            cmag_sqr_f32 cmag_sqr_q15 cmag_sqr_q31 \
            cmul_f32 cmul_q15 cmul_q31 \
            cmul_real_f32 cmul_real_q15 cmul_real_q31 \
            cdprod_typ2_f32 cdprod_typ2_q15 cdprod_typ2_q31 \
            cdprod_f32 cdprod_q15 cdprod_q31
        do
            run ${SRC}
        done
        #echo "" >> $output_log
    fi

    if [ "$CONTROLLER" == "1" ];then
        ## Controller ##
        for SRC in \
            clarke_f32 clarke_q31 inv_clarke_f32 inv_clarke_q31 \
            park_f32 park_q31 inv_park_f32 inv_park_q31 \
            pid_f32 pid_q15 pid_q31
        do
            run ${SRC}
        done
        #echo "" >> $output_log
    fi

    if [ "$FILTERING" == "1" ];then
        # Filtering ##
        for SRC in \
            bq_df1_f32 bq_df1_q15 bq_df1_q31 \
            conv_f32 conv_q15 conv_q31 conv_q7 \
            corr_f32 corr_q15 corr_q31 corr_q7 \
            dcmfir_f32 dcmfir_q15 dcmfir_q31 \
            fir_f32 fir_q15 fir_fast_q15 fir_q31 fir_fast_q31 fir_q7 \
            lfir_f32 lfir_q15 lfir_q31 \
            liir_f32 liir_q15 liir_fast_q15 liir_q31 liir_fast_q31 \
            lms_f32 lms_q15 lms_q31 \
            nlms_f32 nlms_q15 nlms_q31 \
            spafir_f32 spafir_q15 spafir_q31 spafir_q7 \
            upsplfir_f32 upsplfir_q15 upsplfir_q31 \
            conv_partial_f32
        do
            run ${SRC}
        done

        ## Corresponding to different functions in arm
        run "conv_partial_q15" ""
        run "conv_partial_q15" "opt"
        run "conv_partial_q15" "fast"
        run "conv_partial_q15" "fast_opt"
        run "conv_partial_q31" ""
        run "conv_partial_q31" "fast"
        run "conv_partial_q7" ""
        run "conv_partial_q7" "opt"

        for SRC in \
            bq_df2T_f32 bq_df2T_f64 bq_stereo_df2T_f32 bq_df1_32x64_q31 \
            bq_df1_fast_q15 bq_df1_fast_q31 dcmfir_fast_q15 dcmfir_fast_q31
        do
            run ${SRC}
        done
        #echo "" >> $output_log
    fi

    if [ "$MATRIX" == "1" ];then
        ## Matrix ##
        for SRC in \
            mat_add_f32 mat_add_f64 mat_add_q15 mat_add_q31 \
            mat_inv_f32 mat_inv_f64 \
            mat_mul_f32 mat_mul_f64 mat_mul_q15 mat_mul_fast_q15 mat_mul_q31 mat_mul_fast_q31 mat_mul_q7 \
            mat_scale_f32 mat_scale_q15 mat_scale_q31 \
            mat_sub_f32 mat_sub_f64 mat_sub_q15 mat_sub_q31 \
            mat_trans_f32 mat_trans_f64 mat_trans_q15 mat_trans_q31 mat_trans_q7 mat_trans_u8 \
            mat_mul_mxv_f32 mat_mul_mxv_q15 mat_mul_mxv_q31 mat_mul_mxv_q7 \
            cmat_mul_f32 cmat_mul_q31 cmat_mul_q15 \
            cmat_trans_f32 cmat_trans_q15 cmat_trans_q31 \
            mat_mul_vxm_q7 mat_oprod_q31 mat_pwr2_cache_f64
        do
            run ${SRC}
        done
       #echo "" >> $output_log
    fi

    if [ "$STATISTICS" == "1" ];then
        ## Statistics ##
        for SRC in \
            max_f32 max_f64 max_u64 max_q15 max_u16 max_q31 max_u32 max_q7 max_u8 \
            min_f32 min_f64 min_u64 min_q15 min_u16 min_q31 min_u32 min_q7 min_u8 \
            max_val_f32 max_val_f64 max_val_u64 max_val_q15 max_val_u16 max_val_q31 max_val_u32 max_val_q7 max_val_u8 \
            min_val_f32 min_val_f64 min_val_u64 min_val_q15 min_val_u16 min_val_q31 min_val_u32 min_val_q7 min_val_u8 \
            mean_f32 mean_q15 mean_q31 mean_q7 mean_u8 \
            rms_f32 rms_q15 rms_q31 \
            pwr_f32 pwr_q15 pwr_q31 pwr_q7 \
            std_f32 std_q15 std_q31 std_u8 \
            var_f32 var_q15 var_q31 \
            entropy_f32 relative_entropy_f32 \
            lse_dprod_f32 lse_f32 gaussian_naive_bayes_est_f32 \
            absmax_f32 absmax_f64 absmax_q15 absmax_q31 absmax_q7 \
            absmin_f32 absmin_f64 absmin_q15 absmin_q31 absmin_q7 \
            absmax_val_f32 absmax_val_f64 absmax_val_q15 absmax_val_q31 absmax_val_q7 \
            absmin_val_f32 absmin_val_f64 absmin_val_q15 absmin_val_q31 absmin_val_q7 \
            mse_f32 mse_f64 mse_q15 mse_q31 mse_q7


        do
            run ${SRC}
        done
       #echo "" >> $output_log
    fi

    if [ "$TRANSFORM" == "1" ];then
        ## Transform ##
        for SRC in \
            dct_f32 dct_q15 dct_q31 \
            dct4_f32 dct4_q15 dct4_q31 idct4_f32 idct4_q15 idct4_q31
        do
            run ${SRC}
        done
        for SRC in \
            cfft_f32 cfft_f64 cfft_q15 cfft_q31 cifft_f32 cifft_f64 cifft_q15 cifft_q31
        do
            for LOGM in 3 4 5 6 7 8 9 10
            do
                run "${SRC}_${LOGM}"
            done
        done
        for SRC in \
            rfft_f32 rfft_f64 rfft_q15 rfft_q31 rifft_f32 rifft_f64 rifft_q15 rifft_q31
        do
            for LOGM in 4 5 6 7 8 9 10
            do
                run "${SRC}_${LOGM}"
            done
        done
       #echo "" >> $output_log
    fi

    if [ "$UTILS" == "1" ];then
        ## Utils ##
        for SRC in \
            cos_f32 cos_q15 cos_q31 \
            sin_f32 sin_q15 sin_q31 \
            convert_f32_q15 convert_f32_q31 convert_f32_q7 \
            convert_q15_f32 convert_q15_q31 convert_q15_q7 \
            convert_q31_f32 convert_q31_q15 convert_q31_q7 \
            convert_q7_f32 convert_q7_q15 convert_q7_q31 \
            dup_f32 dup_q15 dup_q31 dup_q7 \
            set_f32 set_q15 set_q31 set_q7 \
            sqrt_f32 sqrt_q15 sqrt_q31 \
            barycenter_f32 weighted_sum_f32 \
            exp_f32 log_f32 log_f64 log_q15 log_q31 \
            sigmoid_f32 \
            atan_f32 atan_q15 atan_q31 \
            atan2_f32 atan2_q15 atan2_q31
        do
            run ${SRC}
        done

        for SRC in \
            cos_f16 sin_f16 exp_f16 log_f16 sigmoid_f16
        do
            run_f16 ${SRC}
        done
    fi

    if [ "$SVM" == "1" ];then
        ## SVM ##
        for SRC in \
            svm_linear_est_f32 svm_poly_est_f32 svm_rbf_est_f32 svm_sigmoid_est_f32
        do
            run ${SRC}
        done
    fi

    if [ "$DISTANCE" == "1" ];then
        ## SVM ##
        for SRC in \
            dist_bray_curtis_f32 dist_canberra_f32 dist_chebyshev_f32 dist_city_block_f32 dist_corr_f32 dist_cos_f32 \
            dist_euclidean_f32 dist_jensen_shannon_f32 dist_minkowski_f32 \
            bdist_dice_u32_f32 bdist_hamming_u32_f32 bdist_jaccard_u32_f32 bdist_kulsinski_u32_f32 bdist_rogers_tanimoto_u32_f32 \
            bdist_russell_rao_u32_f32 bdist_sokal_michener_u32_f32 bdist_sokal_sneath_u32_f32 bdist_yule_u32_f32
        do
            run ${SRC}
        done
    fi

    if [ "$SORT" == "1" ];then
        ## SVM ##
        for SRC in \
            sort_bitonic_f32 sort_bubble_f32 sort_heap_f32 sort_insertion_f32 \
            sort_merge_f32 sort_quick_f32 sort_selection_f32
        do
            run ${SRC}
        done
    fi

}

################## main ######################################
cd $CURRENT_PATH
output_log=libdsp_perf.csv
rm -f $output_log
touch $output_log
if [[ $EXTRA_CFLAGS == *"-DENA_SIZE"* ]]; then
    echo "function, cycle,text,code,rodata,data,bss" >> $output_log
    ENA_SIZE=1
else
    echo "function, inst, cycle" >> $output_log
    ENA_SIZE=0
fi
if [[ $EXTRA_CFLAGS == *"-DENA_DEBUG"* ]]; then
    for SRC in $PERF_TEST_LIST
    do
        run ${SRC}
    done
else
    gen_perf $GROUP
fi
rm -f log
mv $output_log ${NEW_LOG_DIR}/performance.csv
cp $TEST_LOG ${NEW_LOG_DIR}/


## Check the "num of testcase = num of cycle results"
CHECK_COUNT=$((ADX_COUNT-F16_ADX_COUNT))
echo "================================="
echo "Start check performance results:"
if [[ $EXTRA_CFLAGS != *"-mtune=andes-45-series"* ]]; then
    echo "num of total case : $ADX_COUNT($CHECK_COUNT) (include f16 case: $F16_ADX_COUNT)"
else
    echo "num of total case : $ADX_COUNT"
fi
CYCLE_RESULTS="`grep -c "The cycle count is" $TEST_LOG`"
echo "num of cycle results : $CYCLE_RESULTS"
echo "================================="

if [[ $EXTRA_CFLAGS != *"-mtune=andes-45-series"* ]]; then
    if [ "$CHECK_COUNT" != "$CYCLE_RESULTS" ]; then
        touch diff_log
        touch ../diff_log
        echo "num of total case != num of cycle results"
        echo "performance testing FAIL"
    else
        echo "num of total case = num of cycle results"
        echo "performance testing PASS"
    fi
else
    if [ "$ADX_COUNT" != "$CYCLE_RESULTS" ]; then
        touch diff_log
        touch ../diff_log
        echo "num of total case != num of cycle results"
        echo "performance testing FAIL"
    else
        echo "num of total case = num of cycle results"
        echo "performance testing PASS"
    fi
fi
