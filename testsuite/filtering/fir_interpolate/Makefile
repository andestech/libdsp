CC = gcc
RM = rm
RUN_TEST_SCRIPT = ../../run_test.sh

# modify these variables
TEST_MACRO = fir_interpolate_f32.m fir_interpolate_q15.m  \
	fir_interpolate_q31.m



# rules
golden:
	$(RUN_TEST_SCRIPT) $(TEST_MACRO)

clean_golden:
	$(RM) golden*
