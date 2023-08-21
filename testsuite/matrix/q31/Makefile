CC = gcc
RM = rm
RUN_TEST_SCRIPT = ../../run_test.sh

# modify these variables
TEST_MACRO = mat_add_q31.m mat_inverse_q31.m mat_mult_q31.m  \
	mat_scale_q31.m mat_sub_q31.m mat_trans_q31.m

# rules
golden:
	$(RUN_TEST_SCRIPT) $(TEST_MACRO)

clean_golden:
	$(RM) golden*
