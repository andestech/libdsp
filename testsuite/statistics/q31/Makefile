CC = gcc
RM = rm
RUN_TEST_SCRIPT = ../../run_test.sh

# modify these variables
TEST_MACRO = max_q31.m min_q31.m mean_q31.m rms_q31.m rms_sqrt_q31.m \
	var_q31.m std_q31.m std_sqrt_q31.m power_q31.m

# rules
golden:
	$(RUN_TEST_SCRIPT) $(TEST_MACRO)

clean_golden:
	$(RM) golden*
