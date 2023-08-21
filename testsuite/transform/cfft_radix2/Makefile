CC = gcc
RM = rm
RUN_TEST_SCRIPT = ../../run_test.sh

# modify these variables
TEST_MACRO = cfft_radix2_f32.m cfft_radix2_q15.m cfft_radix2_q31.m



# rules
golden:
	$(RUN_TEST_SCRIPT) $(TEST_MACRO)

clean_golden:
	$(RM) golden*
