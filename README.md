# MOC
MPI Open MP Coordination library

# Configuration

To configure the MOC library, please run the following:
$ ./autogen.sh && $ ./configure --prefix=<MOC_INSTALL_DIR> --with-mpi=<OMPI_MASTER_INSTALL_DIR> --with-pmix=<PMIX_INSTALL_DIR> CC=[gcc,clang] [--enable-dynamic-coord]

CONFIGURE OPTIONS:
- enable-dynamic-coord: by default, MOC will interact with the Open MP runtime by setting
  environment variables. This mode is called static. When using a modified and
  compatible Open MP, MOC can coordinate Open MPI and the Open MP runtime. This mode is
  called dynamic. Therefore, the --enable-dynamic set the MOC library to dynamic mode.
- with-mpi: point to the directory where Open MPI is installed.
- with-pmix: point to the directory where PMIx is installed.

Note that it is necessary to specify CC if you do not want to use the default compiler on your system.
Make sure you update PATH and LD_LIBRARY_PATH before running configure. Then configure will automatically
detect the requested compiler and set flags accordingly (e.g., gcc and clang do not use the same flags
to enable OpenMP).
