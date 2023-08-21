CC = gcc
RM = rm
RUN_TEST_SCRIPT = ../../run_test.sh

# modify these variables
TEST_MACRO = lms_f32.m lms_q15.m lms_q31.m



# rules
golden:
	$(RUN_TEST_SCRIPT) $(TEST_MACRO)

clean_golden:          #Octave do not support xcorr func.
	$(RM) golden*
