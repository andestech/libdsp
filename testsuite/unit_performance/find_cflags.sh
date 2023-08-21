## ./find_cflags.sh $1
# $1: cpu type(d10 or graywolf)
####################################################
BASIC=0
COMPLEX=1
CONTROLLER=0
FILTERING=0
MATRIX=0
STATISTICS=0
TRANSFORM=0
UTILS=0
SVM=0
DISTANCE=0
COMPARE_KEIL=0
USING_TOOLCHAIN_LDSP=0
USING_BUILD_DEFAULT_LIB_SCRIPT=0
####################################################
export PATH=:$PATH
LOCAL=`pwd`
CPU_TYPE=$1
mkdir -p ISA_ADX
CC=nds32le-elf-gcc

if [ "$1" == "d15" ];then
    CPU_TYPE_FLAGS="-mcpu=d15 -DCPU_TYPE_GRAYWOLF"
    SIDTOOL="nds32-sim-wrapper-on-board"
elif [ "$1" == "n13" ];then
   CPU_TYPE_FLAGS="-mcpu=n13"
   SIDTOOL="nds32-sim-wrapper-on-board"
elif [ "$1" == "a25" ];then
   CPU_TYPE_FLAGS="-mcpu=a25 -mext-dsp"
   SIDTOOL="riscv32-sim-wrapper-d25-v5"
   CC="riscv32-elf-gcc"
   LD_FLAG="-T ae350-xip.ld"
elif [ "$1" == "d45" ];then
   CPU_TYPE_FLAGS="-mcpu=d45 -mext-dsp"
   SIDTOOL="riscv32-sim-wrapper-d45-v5"
   CC="riscv32-elf-gcc"
   LD_FLAG="-T ae350-xip.ld"
elif [ "$1" == "n25" ];then
   CPU_TYPE_FLAGS="-mcpu=n25"
   SIDTOOL="riscv32-sim-wrapper-n25-v5"
   CC="riscv32-elf-gcc"
   LD_FLAG="-T ae350-xip.ld"
elif [ "$1" == "n25board" ];then
   CPU_TYPE_FLAGS="-mcpu=n25"
   SIDTOOL="riscv32-sim-wrapper-on-board"
   CC="riscv32-elf-gcc"
   LD_FLAG="-mvh -T ae350-xip.ld"
elif [ "$1" == "a25board" ];then
   CPU_TYPE_FLAGS="-mcpu=a25 -mext-dsp"
   #SIDTOOL="riscv32-sim-wrapper-on-board"
   SIDTOOL="riscv32-sim-wrapper-boardfarm"
   CC="riscv32-elf-gcc"
   PREFIX="riscv32-elf"
   LD_FLAG="-mvh -T ae350-xip.ld"
elif [ "$1" == "d45board" ];then
   CPU_TYPE_FLAGS="-mcpu=d45 -mext-dsp"
   #SIDTOOL="riscv32-sim-wrapper-on-board"
   SIDTOOL="riscv32-sim-wrapper-boardfarm"
   CC="riscv32-elf-gcc"
   PREFIX="riscv32-elf"
   LD_FLAG="-mvh -T ae350-xip.ld"
elif [ "$1" == "ax25" ];then
   CPU_TYPE_FLAGS="-mcpu=ax25 -mext-dsp"
   SIDTOOL="riscv64-sim-wrapper-dx25-v5"
   CC="riscv64-elf-gcc"
   LD_FLAG=" -T ae350-xip.ld"
elif [ "$1" == "ax45" ];then
   CPU_TYPE_FLAGS="-mcpu=ax45 -mext-dsp"
   SIDTOOL="riscv64-sim-wrapper-ax45-v5"
   CC="riscv64-elf-gcc"
   LD_FLAG=" -T ae350-xip.ld"
elif [ "$1" == "ax25board" ];then
   CPU_TYPE_FLAGS="-mcpu=ax25 -mext-dsp"
   #SIDTOOL="riscv64-sim-wrapper-on-board"
   SIDTOOL="riscv64-sim-wrapper-boardfarm"
   CC="riscv64-elf-gcc"
   LD_FLAG="-mvh -T ae350-xip.ld"
elif [ "$1" == "ax45board" ];then
   CPU_TYPE_FLAGS="-mcpu=ax45 -mext-dsp"
   #SIDTOOL="riscv64-sim-wrapper-on-board"
   SIDTOOL="riscv64-sim-wrapper-boardfarm"
   CC="riscv64-elf-gcc"
   LD_FLAG="-mvh -T ae350-xip.ld"
elif [ "$1" == "nx25board" ];then
   CPU_TYPE_FLAGS="-mcpu=nx25"
elif [ "$1" == "nx25board" ];then
   CPU_TYPE_FLAGS="-mcpu=nx25"
   #SIDTOOL="riscv64-sim-wrapper-on-board"
   SIDTOOL="riscv32-sim-wrapper-boardfarm"
   CC="riscv64-elf-gcc"
   LD_FLAG="-mvh -T ae350-xip.ld"
else
    CPU_TYPE_FLAGS="-mcpu=d10"
    SIDTOOL="nds32-sim-wrapper-on-board"
fi

which ${CC}

#BASIC_FLAGS="-ffunction-sections -fdata-sections -O3 -mext-dsp -mext-zol -fsched-pressure --param sched-pressure-algorithm=2 -DPERF_TEST -fsection-anchors -fno-merge-constants -fsingle-precision-constant -mext-fpu-fma"
BASIC_FLAGS="-ffunction-sections -fdata-sections -O3"
#BASIC_FLAGS="-ffunction-sections -fdata-sections -O3 -mext-dsp -mext-zol -fsched-pressure --param sched-pressure-algorithm=2 -DPERF_TEST -fsection-anchors -fno-merge-constants -fsingle-precision-constant"

function build_lib {
    if [ "$1" == "" ];then
        COMPILE_FLAGS=""
    elif [ "$1" == "g1" ];then
        COMPILE_FLAGS="-funroll-loops --param max-unroll-times=2"
    elif [ "$1" == "g2" ];then
        COMPILE_FLAGS="-funroll-loops --param max-unroll-times=2 -fno-auto-inc-dec"
    elif [ "$1" == "g3" ];then
        COMPILE_FLAGS="-funroll-loops --param max-unroll-times=4"
    elif [ "$1" == "g4" ];then
        COMPILE_FLAGS="-funroll-loops --param max-unroll-times=4 -fno-auto-inc-dec"
    elif [ "$1" == "g5" ];then
        COMPILE_FLAGS="-funroll-loops --param max-unroll-times=8 -fno-auto-inc-dec"
    elif [ "$1" == "g6" ];then
        COMPILE_FLAGS="-funroll-loops --param max-unroll-times=8"
    fi
    BUILD_FLAFS="$BASIC_FLAGS $COMPILE_FLAGS $CPU_TYPE_FLAGS"
    make clean > /dev/null
    make -j16 CPPFLAGS="${BUILD_FLAFS} -DRES_LOGN=10" CFLAGS="" > /dev/null
    cp libdsp.a $LOCAL/LIBRARY/libdsp.a
    echo "build $1 libdsp.a ...."
    echo "CFLAGS= ${BUILD_FLAFS}"
}
if [ "$USING_TOOLCHAIN_LDSP" == "1" ];then
    LIBRARY_PATH="-ldsp $CPU_TYPE_FLAGS"
else
    LIBRARY_PATH="LIBRARY/libdsp.a"
fi

function run {
    echo "$1 ...."
    DEBUG_FLAG="-DDSP_TEST -DRES_LOGN=10 -I./../../include -include ../include/t_pf_perf.h  -Wl,--gc-sections -O3 ${CPU_TYPE_FLAGS} ${LD_FLAG}"
    LFLAGS="$DEBUG_FLAG"
    ${CC} -DPF_COUNTER ${LFLAGS} -o ISA_ADX/$1_pf.adx t_pf_$1.c ${LIBRARY_PATH} -lm
    ${SIDTOOL} ISA_ADX/$1_pf.adx > log
    echo -n nds32_$1"," >> $output_log
    cat log | sed  "s/\r//" | grep "The cycle count" | awk '{print $5}' >> $output_log
}

function find_cflags {
    output_log=out${1}_$CPU_TYPE.csv
    rm -f $output_log
    touch $output_log

if [ "$BASIC" == "1" ];then
    ## Basic ##
    for SRC in \
        abs_f32 abs_q15 abs_q31 abs_q7 add_f32 add_q15 add_q31 add_q7 dot_prod_f32 dot_prod_q15 dot_prod_q31 dot_prod_q7 \
        mul_f32 mul_q15 mul_q31 mul_q7 neg_f32 neg_q15 neg_q31 neg_q7 offset_f32 offset_q15 offset_q31 offset_q7 \
        scale_f32 scale_q15 scale_q31 scale_q7 shift_q15 shift_q31 shift_q7 sub_f32 sub_q15 sub_q31 sub_q7
    do
        run ${SRC}
    done
    #echo "" >> $output_log
fi

if [ "$COMPARE_KEIL" == "0" ];then
    for SRC in \
        div_f32 div_q31 add_u8_u16 sub_u8_q7 dot_prod_u8 dot_prod_u8xq15 mul_u8_u16 offset_u8 scale_u8 shift_u8 div_u64_u32 div_s64_u32
    do
        run ${SRC}
    done
fi

if [ "$COMPLEX" == "1" ];then
    ## Complex ##
    for SRC in \
        clx_conj_f32 clx_conj_q15 clx_conj_q31 clx_mag_f32 clx_mag_q15 clx_mag_q31 clx_mag_sqr_f32 clx_mag_sqr_q15 clx_mag_sqr_q31 \
        clx_mul_f32 clx_mul_q15 clx_mul_q31 clx_mul_real_f32 clx_mul_real_q15 clx_mul_real_q31 \
        clx_dot_prod_typ2_f32 clx_dot_prod_typ2_q15 clx_dot_prod_typ2_q31
    do
        run ${SRC}
    done
    #echo "" >> $output_log
fi

if [ "$COMPARE_KEIL" == "0" ];then
    for SRC in \
        clx_dot_prod_f32 clx_dot_prod_q15 clx_dot_prod_q31
    do
        run ${SRC}
    done
fi

if [ "$CONTROLLER" == "1" ];then
    ## Controller ##
    for SRC in \
        clarke_f32 clarke_q31 inv_clarke_f32 inv_clarke_q31 park_f32 park_q31 inv_park_f32 inv_park_q31 \
        pid_f32 pid_q15 pid_q31
    do
        run ${SRC}
    done
    #echo "" >> $output_log
fi

if [ "$FILTERING" == "1" ];then
    ## Filtering ##
    for SRC in \
        biquad_df1_f32 biquad_df1_q15 biquad_df1_q31 conv_f32 conv_q15 conv_q31 conv_q7 corr_f32 corr_q15 corr_q31 corr_q7 \
        dcmfir_f32 dcmfir_q15 dcmfir_q31 fir_f32 fir_q15 fir_fast_q15 fir_q31 fir_fast_q31 fir_q7 lfir_f32 lfir_q15 lfir_q31 \
        liir_f32 liir_q15 liir_fast_q15 liir_q31 liir_fast_q31 lms_f32 lms_q15 lms_q31 nlms_f32 nlms_q15 nlms_q31 \
        spafir_f32 spafir_q15 spafir_q31 spafir_q7 \
        upsplfir_f32 upsplfir_q15 upsplfir_q31 \
        conv_partial_f32 conv_partial_q15 conv_partial_q15 conv_partial_q15 conv_partial_q15 \
        conv_partial_q31 conv_partial_q31 conv_partial_q7 conv_partial_q7 \
        biquad_df2T_f32 biquad_df2T_f64 biquad_stereo_df2T_f32 biquad_df1_32x64_q31 \
        bq_df1_fast_q15 bq_df1_fast_q31 dcmfir_fast_q15 dcmfir_fast_q31
    do
        run ${SRC}
    done
    #echo "" >> $output_log
fi

if [ "$MATRIX" == "1" ];then
    ## Matrix ##
    for SRC in \
        mat_add_f32 mat_add_q15 mat_add_q31 mat_inv_f32 mat_inv_f64 mat_mul_f32 mat_mul_q15 mat_mul_q31 \
        mat_scale_f32 mat_scale_q15 mat_scale_q31 mat_sub_f32 mat_sub_q15 mat_sub_q31 \
        mat_trans_f32 mat_trans_q15 mat_trans_q31 \
        mat_mul_fast_q15 mat_mul_fast_q31 cmat_mul_f32 cmat_mul_q31 cmat_mul_q15
    do
        run ${SRC}
    done
   #echo "" >> $output_log
fi

if [ "$COMPARE_KEIL" == "0" ];then
    for SRC in \
        mat_trans_u8
    do
        run ${SRC}
    done
fi


if [ "$STATISTICS" == "1" ];then
    ## Statistics ##
    for SRC in \
        max_f32 max_q15 max_q31 max_q7 mean_f32 mean_q15 mean_q31 mean_q7 min_f32 min_q15 min_q31 min_q7 \
        rms_f32 rms_q15 rms_q31 sos_f32 sos_q15 sos_q31 sos_q7 std_f32 std_q15 std_q31 var_f32 var_q15 var_q31 \
        max_val_f32 entropy_f32 relative_entropy_f32 lse_dprod_f32 lse_f32 gaussian_naive_bayes_est_f32
    do
        run ${SRC}
    done
   #echo "" >> $output_log
fi
if [ "$COMPARE_KEIL" == "0" ];then
    for SRC in \
        max_u8 mean_u8 min_u8 std_u8
    do
        run ${SRC}
    done
fi


if [ "$TRANSFORM" == "1" ];then
    ## Transform ##
    for SRC in \
        cfft_radix2_f32 cfft_radix2_q15 cfft_radix2_q31 cfft_radix4_f32 cfft_radix4_q15 cfft_radix4_q31 \
        cfft_f32 cfft_q15 cfft_q31 cifft_f32 cifft_q15 cifft_q31 dct4_f32 dct4_q15 dct4_q31 \
        idct4_f32 idct4_q15 idct4_q31 rfft_f32 rfft_q15 rfft_q31 \
        rifft_f32 rifft_q15 rifft_q31
    do
        run ${SRC}
    done
   #echo "" >> $output_log
fi

if [ "$COMPARE_KEIL" == "0" ];then
    for SRC in \
        dct_f32 dct_q15 dct_q31
    do
        run ${SRC}
    done
fi

if [ "$UTILS" == "1" ];then
    ## Utils ##
    for SRC in \
        cos_f32 cos_q15 cos_q31 sin_f32 sin_q15 sin_q31 convert_f32_q15 convert_f32_q31 convert_f32_q7 \
        convert_q15_f32 convert_q15_q31 convert_q15_q7 convert_q31_f32 convert_q31_q15 convert_q31_q7 \
        convert_q7_f32 convert_q7_q15 convert_q7_q31 dup_f32 dup_q15 dup_q31 dup_q7 set_f32 set_q15 set_q31 set_q7 \
        sqrt_f32 sqrt_q15 sqrt_q31 \
        barycenter_f32 weighted_sum_f32
    do
        run ${SRC}
    done
   #echo "" >> $output_log
fi

if [ "$COMPARE_KEIL" == "0" ];then
    for SRC in \
        arctan_f32 arctan_q15 arctan_q31 atan2_f32 atan2_q15 atan2_q31
    do
        run ${SRC}
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

if [ "$COMPARE_KEIL" == "1" ];then
    ## NOT COMPARE
    for SRC in \
        dct_f32 dct_q15 dct_q31 arctan_f32 arctan_q15 arctan_q31 atan2_f32 atan2_q15 atan2_q31 clx_dot_prod_f32 \
        clx_dot_prod_q15 clx_dot_prod_q31 div_f32 div_q31 \
        add_u8_u16 sub_u8_q7 dot_prod_u8 dot_prod_u8xq15 dprod_q7xq15 mul_u8_u16 offset_u8 scale_u8 shift_u8  \
        mat_trans_u8 mat_mul_f64 mat_mul_q7 mat_mul_vxm_q7 mat_oprod_q31 mat_pow2_cache_f64 \
        max_u8 mean_u8 min_u8 std_u8 div_u64_u32 div_s64_u32
    do
        run ${SRC}
    done
    #echo "" >> $output_log
fi
    echo "LFLAGS= $LFLAGS"
}

########################################################
#main
rm -f find_cflags.csv
for GROUP in \
    ""
    #"" g1 g2 g3 g4 g5 g6
do
if [ "$USING_TOOLCHAIN_LDSP" == "0" ];then
    if [ "$USING_BUILD_DEFAULT_LIB_SCRIPT" == "1" ];then
        cd ../../
        ./build_default_library.sh ../build_dir ${PREFIX} ../build_dir
        cp ../build_dir/mext-dsp/libdsp.a $LOCAL/LIBRARY/libdsp.a
    else
        cd ../../../build_dir/
        build_lib $GROUP
    fi
fi
    cd $LOCAL
    find_cflags $GROUP
    echo "outut CSV: out${GROUP}.csv"
done
#paste -d "," out.csv outg1.csv outg2.csv outg3.csv outg4.csv  > find_cflags.csv
#paste -d "," out.csv outg1.csv outg2.csv outg3.csv outg4.csv outg5.csv outg6.csv > find_cflags.csv
