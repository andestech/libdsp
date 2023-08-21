CC = gcc
RM = rm
RUN_TEST_SCRIPT = ../../run_test.sh

# modify these variables
TEST_MACRO = fir_f32.m fir_q15.m fir_q31.m fir_decimate_f32.m \
			 fir_decimate_q15.m fir_decimate_q31.m fir_q7.m



# rules
golden:
	$(RUN_TEST_SCRIPT) $(TEST_MACRO)

clean_golden:
	$(RM) golden*
