CC = gcc
RM = rm
RUN_TEST_SCRIPT = ../../run_test.sh

# modify these variables
TEST_MACRO = cfft_radix4_f32.m  cfft_radix4_q15.m  cfft_radix4_q31.m      \
			 rfft_f32.m  rfft_q15.m  rfft_q31.m


# rules
golden:
	$(RUN_TEST_SCRIPT) $(TEST_MACRO)

clean_golden:          #Octave do not support xcorr func.
	$(RM) golden*
