include $(top_srcdir)/testsuite/makefile.mk

check_PROGRAM = t_svm_linear_est_f32 t_svm_sigmoid_est_f32 t_svm_rbf_est_f32 t_svm_poly_est_f32
TESTS = $(check_PROGRAM)

$(check_PROGRAM): $(SOURCES) $(top_srcdir)/testsuite/include/riscv_dsp_test_accuracy.h
	$(CC) $(CFLAGS) $(CFLAGS_DSP) -o $@ $(srcdir)/$@.c $(LIBRARY) -lm
