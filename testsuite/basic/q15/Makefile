CC = gcc
RM = rm
RUN_TEST_SCRIPT = ../../run_test.sh

# modify these variables
TEST_MACRO = abs_q15.m neg_q15.m offset_q15.m scale_q15.m


# rules
golden:
	$(RUN_TEST_SCRIPT) $(TEST_MACRO)

clean_golden:
	$(RM) golden*
