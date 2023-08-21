CC = gcc
RM = rm
RUN_TEST_SCRIPT = ../../run_test.sh

# modify these variables
TEST_MACRO = biquad_cascade_df1_f32.m   biquad_cascade_df2T_f32.m \
biquad_cascade_df1_q15.m  biquad_cascade_df1_q31.m




# rules
golden:
	$(RUN_TEST_SCRIPT) $(TEST_MACRO)

clean_golden:          #Octave do not support xcorr func.
	$(RM) golden*
