#
# Copyright 2017        UT-Battelle, LLC
#                       All rights reserved.
#
# See COPYING in top-level directory.
# 
# Additional copyrights may follow
# 
# $HEADER$
#

bin_PROGRAMS = simple_test

check_PROGRAMS = 	\
	hello-omp 		\
	tst-thrdbind

TESTS = \
	$(check_PROGRAMS)

NPROCS ?= 2
LOG_COMPILER = $(TEST_RUNNER)

simple_test_SOURCES = simple_test.c
simple_test_LDADD = ../src/libmoc.la

hello_omp_SOURCES = hello-omp.c
hello_omp_LDFLAGS = -fopenmp
hello_omp_LDADD   = 

tst_thrdbind_SOURCES = tst-thrdbind.c
tst_thrdbind_LDADD   = -lpthread
