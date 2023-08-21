#set -x
#clear

#Usage:
# ./t_pf.sh add_q15 v3 O3
# $1 : max_q31 or max_q15 or max_q7
# $2 : v3 or v3m
# $3 : O0 ~ O3, Os
# $4 : cflags

lib_file="libdsp.10_${2,,}_${3,,}.a"

tcase="${1}"
objdump_file="objdump_${tcase}_${2,,}_${3,,}.log"
src_file="t_pf_${tcase}.c"
adx_folder="nds32.adx"
adx_file="${tcase}_${2,,}_${3,,}.adx"
map_file="link_${tcase}_${2,,}_${3,,}.map"

link_flags="-Wl,-Map=${map_file},--cref"

#-mext-dsp
all_flags="-${3} ${4} -DPF_COUNTER -DRES_LOGN=10 -DDSP_TEST -I./../../include  -I./../include -include t_pf_perf.h $src_file -o $adx_folder/$adx_file ./LIBRARY/$lib_file -lm ${link_flags}"
#test_size=1024
#test_log_size=10

if [ "$1" == "biquad_df1_q15" ];then
all_flags="-DNDS_BQ_DF1_Q15_USE_SIMD ${all_flags} "
fi


echo "****************************"
echo   $lib_file $2 $3 $4
echo   SOURCE FILE  $src_file
echo   ADX FILE     $adx_folder/$adx_file
echo   OBJDUMP FILE $objdump_file
echo   MAP FILE     $map_file
echo "****************************"
nds32le-elf-gcc --version|grep 'gcc'
nds32le-elf-gcc -print-libgcc-file-name
echo
echo gcc flags: $all_flags
echo
echo "****************************"

ls --full-time ./LIBRARY/$lib_file
mkdir -p $adx_folder
rm -f $objdump_file
rm -f $adx_folder/$adx_file

nds32le-elf-gcc $all_flags

if [ "$2" == "v3m" ];then
	echo call v3m sim-wrapper
	nds32-sim-wrapper-n801-v3m $adx_folder/$adx_file
else
	echo call v3 sim-wrapper
	#nds32-sim-wrapper-n968-v3 $adx_folder/$adx_file
	#nds32-sim-wrapper-n1068-v3 $adx_folder/$adx_file
	nds32-sim-wrapper-d1088-v3 $adx_folder/$adx_file
fi

echo The code size of $adx_folder/$adx_file:
nds32le-elf-size $adx_folder/$adx_file

nds32le-elf-objdump -S $adx_folder/$adx_file >>$objdump_file

ls --full-time $adx_folder/$adx_file
ls --full-time $objdump_file
ls --full-time $map_file

#exit 

#echo 
#echo gprof ...
#sid++ --profile=gprof $adx_folder/$adx_file
#mv $adx_file.perflog $adx_file.gproflog
#cat $adx_file.gproflog
##grep "nds32_${1}" $adx_file.gproflog

#echo 
#echo perf-detail ...
#perf-detail $adx_folder/$adx_file

echo [t_pf.sh] done.
echo 

