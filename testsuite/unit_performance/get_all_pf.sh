#usage:
# get_all_pf.sh <Compiler_options>
#
#example
# ./get_all_pf.sh -mreduced-regs

#set -x
CUR_DIR=`pwd`
BUILD_DIR="./../../../../build_dir/"
COMMON_C_FLAG="-mcpu=n10 -DRES_LOGN=10 -O3 $*"

#$1: function name
#$2: output file
#$3: group name
function exec_adx {
	NOW_C_FILE="t_pf_${1}.c"
	if [ -f ${NOW_C_FILE} ]
	then
		./t_pf.sh ${1} v3 o3 | grep "SOURCE FILE\|inst count\|cycle count\|ERROR" | tr "\n" "," | \
        	sed "s/SOURCE FILE //g" | \
			sed "s/The inst count is //g" | \
			sed "s/The cycle count is //g" | \
			sed "s/.*/\0${3},\n/g" | tee -a ${2}
	else
		echo Error: Can not find file ${NOW_C_FILE}
	fi

		#sed "s/<--ERROR:.*$/0,0,/g" | tee -a ${2}
	#echo | tee -a ${2}

}

#$1: output file
function collect_perf {
	NOW_OUT_FILE="$1"
	rm -f ${NOW_OUT_FILE}

	echo
	echo =================================================================
	echo Writing ${NOW_OUT_FILE} ...
	echo

	#execute adx oder by group
	NOW_GROUP="basic"
	for SRC in \
		abs_f32 abs_q15 abs_q31 abs_q7 \
		add_f32 add_q15 add_q31 add_q7 \
		div_f32 div_q31 \
		dot_prod_f32 dot_prod_q15 dot_prod_q31 dot_prod_q7 \
		mul_f32 mul_q15 mul_q31 mul_q7 \
		neg_f32 neg_q15 neg_q31 neg_q7 \
		offset_f32 offset_q15 offset_q31 offset_q7 \
		scale_f32 scale_q15 scale_q31 scale_q7 \
		shift_q15 shift_q31 shift_q7 \
		sub_f32 sub_q15 sub_q31 sub_q7
	do
		exec_adx ${SRC} ${NOW_OUT_FILE} ${NOW_GROUP}
	done

	NOW_GROUP="complex"
	for SRC in \
		clx_conj_f32 clx_conj_q15 clx_conj_q31 \
		clx_dot_prod_f32 clx_dot_prod_q15 clx_dot_prod_q31 \
		clx_mag_f32 clx_mag_q15 clx_mag_q31 \
		clx_mag_sqr_f32 clx_mag_sqr_q15 clx_mag_sqr_q31 \
		clx_mul_f32 clx_mul_q15 clx_mul_q31 \
		clx_mul_real_f32 clx_mul_real_q15 clx_mul_real_q31
	do
		exec_adx ${SRC} ${NOW_OUT_FILE} ${NOW_GROUP}
	done

	NOW_GROUP="controller"
	for SRC in \
		clarke_f32 clarke_q31 inv_clarke_f32 inv_clarke_q31 \
		park_f32 park_q31 inv_park_f32 inv_park_q31 \
		pid_f32 pid_q15 pid_q31
	do
		exec_adx ${SRC} ${NOW_OUT_FILE} ${NOW_GROUP}
	done

	NOW_GROUP="filtering"
	for SRC in \
		biquad_df1_f32 biquad_df1_q15 biquad_df1_q31 \
		conv_f32 conv_q15 conv_q31 conv_q7 \
		corr_f32 corr_q15 corr_q31 corr_q7 \
		dcmfir_f32 dcmfir_q15 dcmfir_q31 \
		fir_f32 fir_q15 fir_q31 fir_q7 \
		lfir_f32 lfir_q15 lfir_q31 \
		liir_f32 liir_q15 liir_q31 \
		lms_f32 lms_q15 lms_q31 \
		nlms_f32 \
		spafir_f32 spafir_q15 spafir_q31 spafir_q7 \
		upsplfir_f32 upsplfir_q15 upsplfir_q31
	do
		exec_adx ${SRC} ${NOW_OUT_FILE} ${NOW_GROUP}
	done

	NOW_GROUP="matrix"
	for SRC in \
		mat_add_f32 mat_add_q15 mat_add_q31 \
		mat_inv_f32 \
		mat_mul_f32 mat_mul_q15 mat_mul_q31 \
		mat_scale_f32 mat_scale_q15 mat_scale_q31 \
		mat_sub_f32 mat_sub_q15 mat_sub_q31 \
		mat_trans_f32 mat_trans_q15 mat_trans_q31
	do
		exec_adx ${SRC} ${NOW_OUT_FILE} ${NOW_GROUP}
	done


	NOW_GROUP="statistics"
	for SRC in \
		max_f32 max_q15 max_q31 max_q7 \
		mean_f32 mean_q15 mean_q31 mean_q7 \
		min_f32 min_q15 min_q31 min_q7 \
		rms_f32 rms_q15 rms_q31 \
		sos_f32 sos_q15 sos_q31 sos_q7 \
		std_f32 std_q15 std_q31 \
		var_f32 var_q15 var_q31
	do
		exec_adx ${SRC} ${NOW_OUT_FILE} ${NOW_GROUP}
	done

	NOW_GROUP="transform"
	for SRC in \
		cfft_radix2_f32 cfft_radix2_q15 cfft_radix2_q31 \
		cfft_radix4_f32 cfft_radix4_q15 cfft_radix4_q31 \
		dct4_f32 dct4_q15 dct4_q31 \
		dct_f32 dct_q15 dct_q31 \
		rfft_f32 rfft_q15 rfft_q31
	do
		exec_adx ${SRC} ${NOW_OUT_FILE} ${NOW_GROUP}
	done

	NOW_GROUP="utils"
	for SRC in \
		arctan_f32 arctan_q15 arctan_q31 \
		atan2_q15 atan2_q31 \
		convert_f32_q15 convert_f32_q31 convert_f32_q7 \
		convert_q15_f32 convert_q31_f32 convert_q7_f32 \
		convert_q15_q31 convert_q15_q7 \
		convert_q31_q15 convert_q31_q7 \
		convert_q7_q15 convert_q7_q31 \
		cos_f32 cos_q15 cos_q31 \
		dup_f32 dup_q15 dup_q31 dup_q7 \
		set_f32 set_q15 set_q31 set_q7 \
		sin_f32 sin_q15 sin_q31 \
		sqrt_f32 sqrt_q15 sqrt_q31
	do
		exec_adx ${SRC} ${NOW_OUT_FILE} ${NOW_GROUP}
	done


}

#$*: gcc flags
function build_lib {
	echo
	echo =================================================================
	echo Building libdsp.a with flags: $*
	echo

	cd ${BUILD_DIR}
	make clean > nul
	make CFLAGS="$*" > nul
	ls -l libdsp.a
	cp -f libdsp.a ../nds32_libdsp/libdsp/testsuite/unit_performance/LIBRARY/libdsp.10_v3_o3.a
	cd ${CUR_DIR}

	echo Done!!!
	echo

}

#=========== main ==============
echo "Begin collect perf data ..."
export PATH=/NOBACKUP/atcsqa02/monk/build-system-3/toolchain/nds32le-elf-mculib-v3-dsp/bin:$PATH
nds32le-elf-gcc -v

#Build lib with DSP + aligned + hwloop(zero overhead loop)
DSP_HWL_FILE="perf_per_function.log.dsp_hwloop"
build_lib ${COMMON_C_FLAG} -mext-dsp -mext-zol -DENA_DSP_ISA
collect_perf ${DSP_HWL_FILE}

#Build lib with DSP + aligned
DSP_AL_FILE="perf_per_function.log.dsp"
build_lib ${COMMON_C_FLAG} -mext-dsp -DENA_DSP_ISA
collect_perf ${DSP_AL_FILE}

#Build lib with EXT_SAT
EXT_SAT_FILE="perf_per_function.log.ext_sat"
build_lib ${COMMON_C_FLAG} -DENA_EXT_SAT_INST
collect_perf ${EXT_SAT_FILE}

#Build lib
ORIG_FILE="perf_per_function.log.orig"
build_lib ${COMMON_C_FLAG}
collect_perf ${ORIG_FILE}

rm -f perf_per_function.csv
rm -f perf_per_function.paste
paste ${ORIG_FILE} ${EXT_SAT_FILE} ${DSP_AL_FILE} ${DSP_HWL_FILE} > perf_per_function.paste

cat perf_per_function.paste | \
       awk 'BEGIN{FS=",";\
       print("function,orig cycles,sat cycles,sat cycle/orig %,dsp cycles,dsp cycle/orig %,hwl cycles,hwl cycle/orig %,orig inst,sat inst,sat inst/orig%,dsp inst,dsp inst/orig%,hwl inst,hwl inst/orig%,group,")}\
       {printf("%s,%d,%d,%3.2f,%d,%3.2f,%d,%3.2f,%d,%d,%3.2f,%d,%3.2f,%d,%3.2f,%s\n",
       	       $1, $3, $7, $3?($7/$3)*100:0, $11, $3?($11/$3)*100:0, $15, $3?($15/$3)*100:0, $2, $6, $2?($6/$2)*100:0, $10, $2?($10/$2)*100:0, $14, $2?($14/$2)*100:0, $4)}' | \
       sed "s/^t_pf_//g" | sed "s/.c,/,/g" \
       > perf_per_function.csv

echo [get_all_pf.sh] done.
echo
