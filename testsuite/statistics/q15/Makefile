CC = gcc
RM = rm
RUN_TEST_SCRIPT = ../../run_test.sh

# modify these variables
TEST_MACRO = max_q15.m min_q15.m mean_q15.m rms_q15.m rms_sqrt_q15.m \
	var_q15.m std_q15.m std_sqrt_q15.m power_q15.m

# rules
golden:
	$(RUN_TEST_SCRIPT) $(TEST_MACRO)

clean_golden:
	$(RM) golden*
