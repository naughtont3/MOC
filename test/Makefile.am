#
# Copyright 2017-2018   UT-Battelle, LLC
#                       All rights reserved.
#
# See COPYING in top-level directory.
# 
# Additional copyrights may follow
# 
# $HEADER$
#

bin_PROGRAMS = simple_test mpi_omp_hello

simple_test_SOURCES = simple_test.c
simple_test_LDADD = ../src/libmoc.la

mpi_omp_hello_SOURCES = mpi_omp_hello.c
mpi_omp_hello_LDADD = ../src/libmoc.la
if USE_CLANG
mpi_omp_hello_CFLAGS = -fopenmp=libiomp5
endif
if USE_CC
mpi_omp_hello_CFLAGS = -fopenmp
endif
