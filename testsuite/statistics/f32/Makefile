CC = gcc
RM = rm
RUN_TEST_SCRIPT = ../../run_test.sh

# modify these variables
TEST_MACRO = max_f32.m min_f32.m rms_f32.m var_f32.m mean_f32.m \
	power_f32.m std_f32.m

# rules
golden:
	$(RUN_TEST_SCRIPT) $(TEST_MACRO)

clean_golden:
	$(RM) golden*
