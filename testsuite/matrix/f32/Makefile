CC = gcc
RM = rm
RUN_TEST_SCRIPT = ../../run_test.sh

# modify these variables
TEST_MACRO = mat_add_f32.m mat_inverse_f32.m mat_mult_f32.m   \
			mat_scale_f32.m  mat_sub_f32.m mat_trans_f32.m

# rules
golden:
	$(RUN_TEST_SCRIPT) $(TEST_MACRO)

clean_golden:
	$(RM) golden*
