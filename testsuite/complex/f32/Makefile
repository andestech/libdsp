CC = gcc
RM = rm
RUN_TEST_SCRIPT = ../../run_test.sh

# modify these variables
TEST_MACRO = conj_f32.m mag_f32.m mag_squared_f32.m mult_f32.m real_f32.m \
	dot_f32.m

# rules
golden:
	$(RUN_TEST_SCRIPT) $(TEST_MACRO)

clean_golden:
	$(RM) golden*
