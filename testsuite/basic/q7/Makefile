CC = gcc
RM = rm
RUN_TEST_SCRIPT = ../../run_test.sh

# modify these variables
TEST_MACRO = abs_q7.m neg_q7.m offset_q7.m scale_q7.m shift_q7.m


# rules
golden:
	$(RUN_TEST_SCRIPT) $(TEST_MACRO)

clean_golden:
	$(RM) golden*
