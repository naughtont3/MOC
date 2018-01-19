# MOC
MPI Open MP Coordination library

# Configuration

To configure the MOC library, please run the following:
$ ./autogen.sh && $ ./configure --prefix=<MOC_INSTALL_DIR> --with-mpi=<OMPI_MASTER_INSTALL_DIR> --with-pmix=<PMIX_INSTALL_DIR> [--enable-dynamic-coord]

CONFIGURE OPTIONS:
- enable-dynamic-coord: by default, MOC will interact with the Open MP runtime by setting
  environment variables. This mode is called static. When using a modified and
  compatible Open MP, MOC can coordinate Open MPI and the Open MP runtime. This mode is
  called dynamic. Therefore, the --enable-dynamic set the MOC library to dynamic mode.
- with-mpi: point to the directory where Open MPI is installed.
- with-pmix: point to the directory where PMIx is installed.

