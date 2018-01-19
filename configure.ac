# -*- shell-script -*-
#
# Copyright 2017-2018   UT-Battelle, LLC.
#                       All rights reserved.
# See COPYING in top-level directory.
# 
# Additional copyrights may follow
# 
# $HEADER$
#

AC_INIT([moc], [0.1], [valleegr@ornl.gov])
AC_CONFIG_SRCDIR([include/moc.h])

AM_INIT_AUTOMAKE([dist-bzip2 subdir-objects foreign tar-ustar])
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])

AC_CONFIG_MACRO_DIR([m4])

CFLAGS="$CFLAGS -std=c99"

AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_MKDIR_P
AM_PROG_CC_C_O
m4_pattern_allow([AM_PROG_AR], [AM_PROG_AR])
LT_INIT

CPPFLAGS_save="$CPPFLAGS"
LIBS_save="$LIBS"
top_srcdir=`pwd`
AC_SUBST([CPPFLAGS],["-I$top_srcdir/include $CPPFLAGS_save"])

dnl# mpi

AC_MSG_CHECKING([MPI installation])
AC_ARG_WITH(mpi,
            [AS_HELP_STRING([--with-mpi=PATH],
             [Absolute path to the install directory for MPI])],
            [MPI_INSTALL_DIR="${withval}"],
            [MPI_INSTALL_DIR=""])

MPI_INC=""
MPI_LIB=""
if test "x$MPI_INSTALL_DIR" = "x" ; then
    AC_MSG_RESULT([no])
else
    AC_MSG_RESULT([yes])
    MPI_INC="-I$MPI_INSTALL_DIR/include"
    MPI_LIB="-L$MPI_INSTALL_DIR/lib"
fi

CPPFLAGS_save="$CPPFLAGS"
LIBS_save="$LIBS"
AC_SUBST([CPPFLAGS],["$MPI_INC $CPPFLAGS_save"])
AC_SUBST([LIBS],["$MPI_LIB $LIBS_save -lmpi"])
AC_CHECK_HEADERS([mpi.h],[AC_DEFINE(HAVE_MPI)],[AC_MSG_ERROR([Cannot find MPI])])
AC_CHECK_LIB([mpi], [MPI_Init],,AC_MSG_ERROR([Cannot use MPI lib]))

dnl# pmix

AC_MSG_CHECKING([PMIx installation])
AC_ARG_WITH(pmix,
            [AS_HELP_STRING([--with-pmix=PATH],
             [Absolute path to the install directory for PMIx])],
            [PMIX_INSTALL_DIR="${withval}"],
            [PMIX_INSTALL_DIR=""])

PMIX_INC=""
PMIX_LIB=""
if test "x$PMIX_INSTALL_DIR" = "x" ; then
    AC_MSG_RESULT([no])
else
    AC_MSG_RESULT([yes])
    PMIX_INC="-I$PMIX_INSTALL_DIR/include"
    PMIX_LIB="-L$PMIX_INSTALL_DIR/lib"
fi

CPPFLAGS_save="$CPPFLAGS"
LIBS_save="$LIBS"
AC_SUBST([CPPFLAGS],["$PMIX_INC $CPPFLAGS_save -std=c99"])
AC_SUBST([LIBS],["$PMIX_LIB $LIBS_save -lpmix"])
AC_CHECK_HEADERS([pmix.h],[AC_DEFINE(HAVE_PMIX)],[AC_MSG_ERROR([Cannot find PMIX])])
AC_CHECK_LIB([pmix], [PMIx_Init],,AC_MSG_ERROR([Cannot use PMIx lib]))

AC_CONFIG_FILES([Makefile include/Makefile src/Makefile test/Makefile])
AC_OUTPUT
