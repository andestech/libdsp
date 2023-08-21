

for LOGN in 3 4 5 6 7 8 9 10 11 12 13
do
    riscv64-elf-gcc -DTRANSFORM_SAMPLES_LOG=${LOGN} -DGETENV_LOG=${LOGN} -I../../include/ -o data_generator_f64.adx data_generator_f64.c -lm
    riscv64-sim-wrapper-dx25-v5 data_generator_f64.adx
    cp input_f64_${LOGN}.bin tmp_input.bin
    cmd=s/TEMP_LOGN/${LOGN}/g
    sed ''"$cmd"'' cfft_radix2_f64_golden.m > cfft_radix2_f64_golden_temp.m
    octave cfft_radix2_f64_golden_temp.m
    cp tmp_output.bin golden_cfft_f64_${LOGN}.bin
    rm cfft_radix2_f64_golden_temp.m tmp_input.bin tmp_output.bin
done
