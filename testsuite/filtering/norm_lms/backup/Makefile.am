include $(top_srcdir)/testsuite/makefile.mk
SOURCES = t_lms_norm_f32.c t_lms_norm_q31.c t_lms_norm_q15.c
check_PROGRAM = t_lms_norm_f32 t_lms_norm_q31 t_lms_norm_q15
TESTS = $(check_PROGRAM)


$(check_PROGRAM): $(SOURCES) $(top_srcdir)/testsuite/include/nds32_test_accuracy.h
	$(CC) $(CFLAGS) $(CFLAGS_DSP) -o $@ $(srcdir)/$@.c ${LIBRARY} -lm
