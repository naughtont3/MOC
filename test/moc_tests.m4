
m4_ifdef([MOC_TESTS_PREP], [], [
    AC_DEFUN([MOC_TESTS_PREP])

    AC_SUBST([TEST_RUNNER],
             ['$(abs_top_builddir)/test/test-runner -np $(NPROCS)'])
])


MOC_TESTS_PREP()
