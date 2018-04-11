# MOC
MPI Open MP Coordination library

# Configuration

To configure the MOC library, please run the following:
$ ./autogen.sh && $ ./configure --prefix=<MOC_INSTALL_DIR> --with-mpi=<OMPI_MASTER_INSTALL_DIR> --with-pmix=<PMIX_INSTALL_DIR> CC=[gcc,clang]

Note that it is necessary to specify CC if you do not want to use the default compiler on your system.
Make sure you update PATH and LD_LIBRARY_PATH before running configure. Then configure will automatically
detect the requested compiler and set flags accordingly (e.g., gcc and clang do not use the same flags
to enable OpenMP).
