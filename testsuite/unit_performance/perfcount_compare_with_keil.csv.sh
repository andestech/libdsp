#!/bin/sh
#set -x
# Generate the nds performance compare with keil using by handrun.sh
#
# Input:
#  $1: v3m, v3m_n7, v3, v3_d10, v3_d13, v3j, v3j_d10, v3j_d13
#  $2: The path of gcc.log location.
#  $3: The option for enable unrolling.
#
# Example:
#  ./perfcount_compare_with_keil.csv.sh v3_d10  /local/sharon/daily_run_perf 0

export PATH=:$PATH
RLEN=10
tc=$1

cpu_type=`echo ${1} | cut -d "_" -f 2`
if [[ $cpu_type == "a25" ]]; then
    CC=riscv32-elf-gcc
    SZ=riscv32-elf-size
    get_size=get_size_v3
elif [[ $cpu_type == "ax25" ]]; then
    CC=riscv64-elf-gcc
    SZ=riscv64-elf-size
    get_size=get_size_v3
else
    CC=nds32le-elf-gcc
    SZ=nds32le-elf-size
    get_size=get_size_v3
fi
LOCAL=`pwd`

# Get each section size of binary. (for v3's size output format)
function get_size_v3 {
      ${SZ} ${OUTPUT_DIR}/$1_pf.adx  | tail -1 | sed  's/[\(\)\+]//g' | awk '{printf "%d,%d,%d,%d,%d,%d,0x%s\n",$1,$2,$3,$4,$5,$6,$7}' \
      >> COMPARE_Keil_CSV/nds_unit_performance_${LEVEL}.csv
      ${CC} -include ../include/nds32_empty_function.h $COMMON_CFLAGS -DPF_COUNTER t_pf_$1.c $2 -o ${OUTPUT_DIR}/$1_sz.adx LIBRARY/$3_empty.a -lm
      ${SZ} ${OUTPUT_DIR}/$1_sz.adx  | tail -1 | sed  's/[\(\)\+]//g' | awk '{printf "%d,%d,%d,%d,%d,%d,0x%s\n",$1,$2,$3,$4,$5,$6,$7}' \
      >> COMPARE_Keil_CSV/nds_unit_performance_${LEVEL}_empty.csv
}

# Get each section size of binary. (for v5's size output format)
function get_size_v5 {
      # revision
      tempStr=$(${SZ} ${OUTPUT_DIR}/$1_pf.adx  | tail -1 | sed  's/[\(\)\+]//g')
      outStr=$(echo $tempStr | awk '{printf "%d,CODE,RODATA,%d,%d,%d,0x%s\n",$1,$4,$5,$6,$7}')
      #get text section size
      text=$(echo $tempStr | awk '{printf "%d", $1}')
      #get rodata size from another output format
      tempStr=$(${SZ} -A ${OUTPUT_DIR}/$1_pf.adx | sed -n 4,5p)
      rodata=$(echo $tempStr | awk '{printf "%d",$2}')
      code=$(($text-$rodata))
      echo $outStr | sed "s/CODE/${code}/g" | sed "s/RODATA/${rodata}/g" >> COMPARE_Keil_CSV/nds_unit_performance_${LEVEL}.csv

      ${CC} -include ../include/nds32_empty_function.h $COMMON_CFLAGS -DPF_COUNTER t_pf_$1.c $2 -o ${OUTPUT_DIR}/$1_sz.adx LIBRARY/$3_empty.a -lm

      tempStr=$(${SZ} ${OUTPUT_DIR}/$1_sz.adx  | tail -1 | sed  's/[\(\)\+]//g')
      outStr=$(echo $tempStr | awk '{printf "%d,CODE,RODATA,%d,%d,%d,0x%s\n",$1,$4,$5,$6,$7}')
      text=$(echo $tempStr | awk '{printf "%d", $1}')
      tempStr=$(${SZ} -A ${OUTPUT_DIR}/$1_sz.adx | sed -n 4,5p)
      rodata=$(echo $tempStr | awk '{printf "%d",$2}')
      code=$(($text-$rodata))
      echo $outStr | sed "s/CODE/${code}/g" | sed "s/RODATA/${rodata}/g" >> COMPARE_Keil_CSV/nds_unit_performance_${LEVEL}_empty.csv
}

function run {
  ${CC} $COMMON_CFLAGS -DPF_COUNTER t_pf_$1.c $2 -o ${OUTPUT_DIR}/$1_pf.adx LIBRARY/$3.a  -lm
  echo -n nds32_$1"," >> COMPARE_Keil_CSV/nds_unit_performance_${LEVEL}.csv
  echo -n nds32_$1"," >> COMPARE_Keil_CSV/nds_unit_performance_${LEVEL}_empty.csv
  if [ ! -e "${OUTPUT_DIR}/$1_pf.adx" ]; then
      echo "0,0,0,0,0,0,0,0,0x0" >> COMPARE_Keil_CSV/nds_unit_performance_${LEVEL}.csv
      echo "0,0,0,0,0,0,0,0,0x0" >> COMPARE_Keil_CSV/nds_unit_performance_${LEVEL}_empty.csv
  else
      ${sidtool} ${OUTPUT_DIR}/$1_pf.adx > adx.log
      cat adx.log | sed  "s/\r//" | grep "The inst count" | awk '{printf "%u,", $5}' >> COMPARE_Keil_CSV/nds_unit_performance_${LEVEL}.csv
      cat adx.log | sed  "s/\r//" | grep "The cycle count" | awk '{printf "%u,", $5}' >> COMPARE_Keil_CSV/nds_unit_performance_${LEVEL}.csv
      cat adx.log | sed  "s/\r//" | grep "average cycle count" | awk '{printf "%.2f,", $6}' >> COMPARE_Keil_CSV/nds_unit_performance_${LEVEL}.csv
      cat adx.log | sed  "s/\r//" | grep "function unit size" | awk '{printf "%d,", $6}' >> COMPARE_Keil_CSV/nds_unit_performance_${LEVEL}.csv
      ${get_size} $1 $2 $3
  fi
}

function paste_keil {
    cp COMPARE_Keil_CSV/keil_$1_$2.csv COMPARE_Keil_CSV/temp1.csv
    sed '1 ittt' -i COMPARE_Keil_CSV/temp1.csv
    echo ",inst count,cycle count,text,data,bss,dec,hex" > COMPARE_Keil_CSV/temp.csv
    cat COMPARE_Keil_CSV/temp1.csv | awk '{FS=","} NR>=2{printf "%s,,%d,%d,%d,%d,%d,%d,%s\n", $1, $4,$5,$6,$7,$8,$9,$10}' >> COMPARE_Keil_CSV/temp.csv
    dos2unix COMPARE_Keil_CSV/temp.csv

    ##generate the nds32 size - empty_size
    echo ",inst count,cycle count,avg count,unit size,text,code,rodata,data,bss,dec,hex" > COMPARE_Keil_CSV/temp_nds_size.csv
    paste -d "," COMPARE_Keil_CSV/nds_unit_performance_$3.csv COMPARE_Keil_CSV/nds_unit_performance_$3_empty.csv | \
    awk -F, '\
    NR>=2{printf "%s,%d,%d,%.2f,%d,%d,%d,%d,%d,%d,%d,0x%x\n", \
    $1,$2,$3,$4,$5,($6-$14),($7-$15),($8-$16),($9-$17),($10-$18),($11-$19),($11-$19)} \
    NF<10 {printf "%s,%d,%d,%.2f,%d,%d,%d,%d,%d,\n", \
    $1,$2,$3,$4,$5,$6,$7,$8,$9,$10,$11}' | \
    sed 's/,0,0,0,0,0,0,0,0,0,0,/,,,,,,,,,,,/g' | \
    sed 's/,0,0,0,0,0,0,0,0,/,,,,,,,,,/g' | \
    sed 's/,0,0,0,0,0,0,/,,,,,,,/g' | \
    #sed 's/,0,/,,/g' | \
    sed 's/inf,/,/g' | \
    sed 's/-nan,/,/g' | \
    sed 's/,hex,/,,/g' \
    >> COMPARE_Keil_CSV/temp_nds_size.csv
    dos2unix COMPARE_Keil_CSV/temp_nds_size.csv

    compare_file="Andes_${tc}_compare_with_keil_$1_$3.csv"

    echo NDS Function,Inst. count,Cycle Count,Avg Count,Unit Size,Text,Code,RO Data,Data,BSS,Total size\(Byte\),Total size\(hex\),Performance\(\%\),Code size\(all\)\(\%\)Code size\(code\)\(\%\),Code size\(rodata\)\(\%\),ARM Function,Inst. count,Cycle count,Avg Count,Code,ROdata,RWdata,ZIdata,Total size\(Byte\),Groups > COMPARE_Keil_CSV/$compare_file
    echo ,,,,,,,,,,G \+ H \+ I \+ J ,,\(C / R \) \* 100\%,\(K / X \) \* 100\%,,,,,,,,,,T \+ U \+ V \+ W \- 1024, >> COMPARE_Keil_CSV/$compare_file

    paste -d "," COMPARE_Keil_CSV/temp_nds_size.csv COMPARE_Keil_CSV/temp.csv | \
    awk -F, '\
    NF>=15 {A1=0; A2=0; A4=0; A3=0;\
    if ($15+0>0) A1=100*$3/$15;\
    if ($20+0>0) A2=100*$11/$20; \
    if ($17+0>0) A4=100*$8/$17; \
    if ($5+0>0) A3=$15/$5; \
    printf "%s,%d,%d,%.2f,%d,%d,%d,%d,%d,%d,%d,0x%x,\
    %.2f,%.2f,%.2f\
    ,%s,%d,%d,%.2f,%d,%d,%d,%d,%d,%s,\n", \
    $1,$2,$3,$4,$5,$6,$7,$8,$9,$10,$11,$12\
    ,A1,A2,A4,\
    $13,$14,$15,A3,$16,$17,$18,$19,$20,$21} \
    NF<14 {printf "%s,%d,%d,%.2f,%d,%d,%d,%d,%d,%d,%d,%s,,,,,,,,,,,,,,\n", \
    $1,$2,$3,$4,$5,$6,$7,$8,$9,$10,$11,$12}' | \
    sed 's/,0,0,0,0,0,0,0,0,0,0,/,,,,,,,,,,,/g' | \
    sed 's/,0,0,0,0,0,0,0,0,/,,,,,,,,,/g' | \
    sed 's/,0,0,0,0,0,0,/,,,,,,,/g' | \
    #sed 's/,0,/,,/g' | \
    sed 's/inf,/,/g' | \
    sed 's/-nan,/,/g' | \
    sed 's/,hex,/,,/g' \
    >> COMPARE_Keil_CSV/$compare_file
    dos2unix COMPARE_Keil_CSV/$compare_file


    if [ "$4" != "" ];then
        echo "Copy the CSV output to ${dst}/COMPARE_Keil_CSV"
        mkdir -p $4
        mkdir -p $4/Debug
        cp COMPARE_Keil_CSV/$compare_file ${dst}/
        cp COMPARE_Keil_CSV/nds_unit_performance_$3.csv COMPARE_Keil_CSV/nds_unit_performance_$3_empty.csv ${dst}/Debug/
        cp COMPARE_Keil_CSV/temp.csv COMPARE_Keil_CSV/temp_nds_size.csv COMPARE_Keil_CSV/temp1.csv ${dst}/Debug/
    else
        mkdir -p COMPARE_Keil_CSV/Debug
        cp COMPARE_Keil_CSV/nds_unit_performance_$3.csv COMPARE_Keil_CSV/nds_unit_performance_$3_empty.csv COMPARE_Keil_CSV/Debug/
        cp COMPARE_Keil_CSV/temp.csv COMPARE_Keil_CSV/temp_nds_size.csv COMPARE_Keil_CSV/temp1.csv COMPARE_Keil_CSV/Debug/
    fi
}


################################# main ######################################

echo
echo "collect for ${tc} ..."

ADX_DIR="$tc"
if [ "$tc" == "v3m" ];then
    sidtool=nds32-sim-wrapper-n801-v3m
    CORTEX="cm0"
    ADX_DIR="v3m"
elif [ "$tc" == "v3m_n7" ];then
    sidtool=nds32-sim-wrapper-n705-v3m
    CORTEX="cm0p"
    ADX_DIR="v3m_n7"
elif [ "$tc" == "v3" ];then
    sidtool=nds32-sim-wrapper-d1088-v3
    CORTEX="cm3"
    SIMULATOR="1"
elif [ "$tc" == "v3_d10" ];then
    sidtool=nds32-sim-wrapper-d1088-v3
    CORTEX="cm4"
    SIMULATOR="1"
elif [ "$tc" == "v3s_d10" ];then
    sidtool=nds32-sim-wrapper-d1088-v3
    CORTEX="cm4_sp_cmsis180"
    SIMULATOR="1"
elif [ "$tc" == "v3_n9" ];then
    sidtool=nds32-sim-wrapper-n968-v3
    CORTEX="cm3"
    SIMULATOR="1"
elif [ "$tc" == "v3_gw" ];then
    sidtool=nds32-sim-wrapper-graywolf-4r3w-32k
    CORTEX="cm7"
    SIMULATOR="1"
elif [ "$tc" == "v3_gwd" ];then
    sidtool=nds32-sim-wrapper-graywolf-4r3w-32k
    CORTEX="cm7"
    SIMULATOR="1"
elif [ "$tc" == "v3s_n10_dsp" ];then
    sidtool=nds32-sim-wrapper-on-board
    CORTEX="cm4_sp"
    LM="1"
elif [ "$tc" == "v3f_n10_dsp" ];then
    sidtool=nds32-sim-wrapper-on-board
    CORTEX="cm4_sp"
    LM="1"
elif [ "$tc" == "v3_n10_dsp" ];then
    sidtool=nds32-sim-wrapper-on-board
    CORTEX="cm4_norm"
    LM="1"
elif [ "$tc" == "v3s_gw_dsp" ];then
    sidtool=nds32-sim-wrapper-on-board
    CORTEX="cm7_sp"
    LM="1"
elif [ "$tc" == "v3f_gw_dsp" ];then
    sidtool=nds32-sim-wrapper-on-board
    CORTEX="cm7_dp"
    LM="1"
elif [ "$tc" == "v3_gw_dsp" ];then
    sidtool=nds32-sim-wrapper-on-board
    CORTEX="cm7_norm"
    LM="1"
elif [ "$tc" == "v5_n25_sid" ];then
    sidtool=riscv32-sim-wrapper-n25-v5
    CORTEX="cm3"
    SIMULATOR="1"
elif [[ "$tc" == "v5_a25"* ]];then
    sidtool=riscv32-sim-wrapper-d25-v5
    CORTEX="cm4"
    SIMULATOR="1"
elif [[ "$tc" == "v5f_a25"* ]];then
    sidtool=riscv32-sim-wrapper-d25-v5
    CORTEX="cm4_sp_cmsis180"
    SIMULATOR="1"
elif [ "$tc" == "v5_n25_board" ];then
    sidtool=riscv32-sim-wrapper-on-board
    CORTEX="cm3"
    LM="1"
elif [[ "$tc" == "v5_ax25"* ]];then
    sidtool=riscv64-sim-wrapper-dx25-v5
    CORTEX="cm7"
    SIMULATOR="1"
elif [[ "$tc" == "v5f_ax25"* ]];then
    sidtool=riscv64-sim-wrapper-dx25-v5
    CORTEX="cm4_sp_cmsis180"
    SIMULATOR="1"
elif [ "$tc" == "v5_nx25_board" ];then
    sidtool=riscv64-sim-wrapper-on-board
    CORTEX="cm3"
    LM="1"
else
    sidtool=nds32-sim-wrapper-d1088-v3
    CORTEX="cm4"
    SIMULATOR="1"
fi

## set the destination of gcc.log location
if [ "$2" != "" ];then
    dst=$2
else
    dst=${LOCAL}
fi

if [ "$tc" == "v3_d10" ] || [ "$tc" == "v3_gwd" ] || [ "$tc" == "*_dsp" ]; then
    EXTDSP_FLAGS="-mext-dsp -mext-zol"
    echo "now run $tc"
else
    EXTDSP_FLAGS="-mext-dsp"
    echo "now run $tc"
fi

echo "sidtool=$sidtool"


DEBUG_FLAGS="-DDSP_TEST -I./../../include -I./../include -include t_pf_perf.h -malways-align $EXTDSP_FLAGS"
# use the mvh flag only when run program on board
if [[ $sidtool == *"on-board" ]]; then
    DEBUG_FLAGS="$DEBUG_FLAGS -mvh"

    if [[ $tc == "v5_n25"* ]]; then
        DEBUG_FLAGS="${DEBUG_FLAGS} -T riscv32.ld"
    elif [[ $tc == "v5_nx25"* ]]; then
        DEBUG_FLAGS="${DEBUG_FLAGS} -T riscv64.ld"
    else
        DEBUG_FLAGS="${DEBUG_FLAGS} -T nds32.ld"
    fi
fi

LEVEL="o3"

    OUTPUT_DIR="nds32.adx/${ADX_DIR}/${LEVEL}"
    mkdir -p ${OUTPUT_DIR}

    echo "    for $tc vs $CORTEX ..."

    export COMMON_CFLAGS="-O3 -Wl,--gc-sections $DEBUG_FLAGS"

    echo ",inst count,cycle count,avg count,unit size,text,code,RO data,data,bss,dec,hex" > COMPARE_Keil_CSV/nds_unit_performance_${LEVEL}.csv
    echo ",text,code,RO data,data,bss,dec,hex" > COMPARE_Keil_CSV/nds_unit_performance_${LEVEL}_empty.csv

    ## Basic ##
    echo "collect the basic performance"
    for SRC in \
        abs_f32 abs_q15 abs_q31 abs_q7 add_f32 add_q15 add_q31 add_q7 dot_prod_f32 dot_prod_q15 dot_prod_q31 dot_prod_q7 \
        mul_f32 mul_q15 mul_q31 mul_q7 neg_f32 neg_q15 neg_q31 neg_q7 offset_f32 offset_q15 offset_q31 offset_q7 \
        scale_f32 scale_q15 scale_q31 scale_q7 shift_q15 shift_q31 shift_q7 sub_f32 sub_q15 sub_q31 sub_q7
    do
        run ${SRC} -DRES_LOGN=${RLEN} libdsp.10_${tc}_${LEVEL}
        echo -n "."
    done

    ## Complex ##
    echo ""
    echo "collect the complex performance"
    for SRC in \
        clx_conj_f32 clx_conj_q15 clx_conj_q31 clx_mag_f32 clx_mag_q15 clx_mag_q31 clx_mag_sqr_f32 clx_mag_sqr_q15 clx_mag_sqr_q31 \
        clx_mul_f32 clx_mul_q15 clx_mul_q31 clx_mul_real_f32 clx_mul_real_q15 clx_mul_real_q31 \
        clx_dot_prod_typ2_f32 clx_dot_prod_typ2_q15 clx_dot_prod_typ2_q31

    do
        run ${SRC} -DRES_LOGN=${RLEN} libdsp.10_${tc}_${LEVEL}
        echo -n "."
    done

    ## Controller ##
    echo ""
    echo "collect the controller performance"
    for SRC in \
        clarke_f32 clarke_q31 inv_clarke_f32 inv_clarke_q31 park_f32 park_q31 inv_park_f32 inv_park_q31 \
        pid_f32 pid_q15 pid_q31
    do
        run ${SRC} -DRES_LOGN=${RLEN} libdsp.10_${tc}_${LEVEL}
        echo -n "."
    done


    ## Filtering ##
    echo ""
    echo "collect the filtering performance"
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
        run ${SRC} -DRES_LOGN=${RLEN} libdsp.10_${tc}_${LEVEL}
        echo -n "."
    done

    ## Matrix ##
    echo ""
    echo "collect the matrix performance"
    for SRC in \
        mat_add_f32 mat_add_q15 mat_add_q31 mat_inv_f32 mat_inv_f64 mat_mul_f32 mat_mul_q15 mat_mul_q31 \
        mat_scale_f32 mat_scale_q15 mat_scale_q31 mat_sub_f32 mat_sub_q15 mat_sub_q31 \
        mat_trans_f32 mat_trans_q15 mat_trans_q31 \
        mat_mul_fast_q15 mat_mul_fast_q31 cmat_mul_f32 cmat_mul_q31 cmat_mul_q15
    do
        run ${SRC} -DRES_LOGN=${RLEN} libdsp.10_${tc}_${LEVEL}
        echo -n "."
    done

    ## Statistics ##
    echo ""
    echo "collect the statistics performance"
    for SRC in \
        max_f32 max_q15 max_q31 max_q7 mean_f32 mean_q15 mean_q31 mean_q7 min_f32 min_q15 min_q31 min_q7 \
        rms_f32 rms_q15 rms_q31 sos_f32 sos_q15 sos_q31 sos_q7 std_f32 std_q15 std_q31 var_f32 var_q15 var_q31 \
        max_val_f32 entropy_f32 relative_entropy_f32 lse_dprod_f32 lse_f32 gaussian_naive_bayes_est_f32
    do
        run ${SRC} -DRES_LOGN=${RLEN} libdsp.10_${tc}_${LEVEL}
        echo -n "."
    done

    ## Transform ##
    echo ""
    echo "collect the transform performance"
    for SRC in \
        cfft_radix2_f32 cfft_radix2_q15 cfft_radix2_q31 cfft_radix4_f32 cfft_radix4_q15 cfft_radix4_q31 \
        cfft_f32 cfft_q15 cfft_q31 cifft_f32 cifft_q15 cifft_q31 dct4_f32 dct4_q15 dct4_q31 \
        idct4_f32 idct4_q15 idct4_q31 rfft_f32 rfft_q15 rfft_q31 \
        rifft_f32 rifft_q15 rifft_q31
    do
        run ${SRC} -DRES_LOGN=${RLEN} libdsp.10_${tc}_${LEVEL}
        echo -n "."
    done

    ## Utils ##
    echo ""
    echo "collect the utils performance"
    for SRC in \
        cos_f32 cos_q15 cos_q31 sin_f32 sin_q15 sin_q31 convert_f32_q15 convert_f32_q31 convert_f32_q7 \
        convert_q15_f32 convert_q15_q31 convert_q15_q7 convert_q31_f32 convert_q31_q15 convert_q31_q7 \
        convert_q7_f32 convert_q7_q15 convert_q7_q31 dup_f32 dup_q15 dup_q31 dup_q7 set_f32 set_q15 set_q31 set_q7 \
        sqrt_f32 sqrt_q15 sqrt_q31 \
        barycenter_f32 weighted_sum_f32
    do
        run ${SRC} -DRES_LOGN=${RLEN} libdsp.10_${tc}_${LEVEL}
        echo -n "."
    done

    ## SVM ##
    echo ""
    echo "collect the SVM performance"
    for SRC in \
        svm_linear_est_f32 svm_poly_est_f32 svm_rbf_est_f32 svm_sigmoid_est_f32
    do
        run ${SRC} -DRES_LOGN=${RLEN} libdsp.10_${tc}_${LEVEL}
        echo -n "."
    done

    ## Distance ##
    echo ""
    echo "collect the distance performance"
    for SRC in \
        dist_bray_curtis_f32 dist_canberra_f32 dist_chebyshev_f32 dist_city_block_f32 dist_corr_f32 dist_cos_f32 \
        dist_euclidean_f32 dist_jensen_shannon_f32 dist_minkowski_f32 \
        bdist_dice_u32_f32 bdist_hamming_u32_f32 bdist_jaccard_u32_f32 bdist_kulsinski_u32_f32 bdist_rogers_tanimoto_u32_f32 \
        bdist_russell_rao_u32_f32 bdist_sokal_michener_u32_f32 bdist_sokal_sneath_u32_f32 bdist_yule_u32_f32
    do
        run ${SRC} -DRES_LOGN=${RLEN} libdsp.10_${tc}_${LEVEL}
        echo -n "."
    done

if [ "$SIMULATOR" == "1" ];then
    ## NOT COMPARE
    echo ""
    echo "collect the not compare performance"
    echo "-----------------notcompare performance---------------" >> COMPARE_Keil_CSV/nds_unit_performance_${LEVEL}.csv
    echo "-----------------notcompare performance---------------" >> COMPARE_Keil_CSV/nds_unit_performance_${LEVEL}_empty.csv

    for SRC in \
        dct_f32 dct_q15 dct_q31 arctan_f32 arctan_q15 arctan_q31 atan2_f32 atan2_q15 atan2_q31 clx_dot_prod_f32 \
        clx_dot_prod_q15 clx_dot_prod_q31 div_f32 div_q31 \
        add_u8_u16 sub_u8_q7 dot_prod_u8 dot_prod_u8xq15 dprod_q7xq15 mul_u8_u16 offset_u8 scale_u8 shift_u8  \
        mat_trans_u8 max_u8 mean_u8 min_u8 std_u8 div_u64_u32 div_s64_u32 mat_mul_q7 mat_mul_vxm_q7
    do
        run ${SRC} -DRES_LOGN=10 libdsp.10_${tc}_${LEVEL}
        echo -n "."
    done
fi

    unset COMMON_CFLAGS

    ## Generate the compare data with Keil
    echo "Generate the compare data with Keil..."

    KEIL_LEVEL="pf"

    ## convert keil level
    paste_keil ${CORTEX} ${KEIL_LEVEL} ${LEVEL} $2

    if [ "${CORTEX2}" == "cm7" ];then
        echo "    for $LEVEL vs $CORTEX2 ..."
        paste_keil ${CORTEX2} ${KEIL_LEVEL} ${LEVEL} $2
    fi
