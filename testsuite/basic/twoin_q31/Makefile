CC = gcc
RM = rm
RUN_TEST_SCRIPT = ../../run_test.sh

# modify these variables
TEST_MACRO = add_q31.m mult_q31.m dot_prod_q31.m sub_q31.m


# rules
golden:
	$(RUN_TEST_SCRIPT) $(TEST_MACRO)

clean_golden:
	$(RM) golden*
