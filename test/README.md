Test README
-----------

Tests for MOC


OVERVIEW
--------

Some tests are run manually, others can be run via 'make check'.

 - For the manual tests, see the comments/code of the individual test or
   associated docs for how to run the test.

 - For the automated ('make check') tests, we use the test infrastructure
   provided with Automake, see `RUNNING TESTS` & `TEST RESULTS` below.


RUNNING TESTS
-------------

Use `make check` to build and run the automated tests.  The test binaries
are compiled (if needed) and then run with this single `make check` command.

The output will display PASS/FAIL style information for the
tests that are run.  The output is generally placed in `*.log`
files that match the given test's filename, e.g., `hello-omp.log`.

The default `TEST_RUNNER` uses the `test/test-runner` script.
You can override the defaults for `make check` in one of two ways:

 - Set number of MPI ranks (`NPROCS`)

    ```
         # Use 8 ranks
        make check NPROCS=8
    ```

 - Use fully customized command-line (`TEST_RUNNER`)

    ```
        make check TEST_RUNNER='mpirun -np 4 --report-bindings'
    ```

 - NOTE: If using fully custom `TEST_RUNNER` approach, the `NPROCS`
   value is ignored and must be provided in the `TEST_RUNNER` command.


TEST RESULTS
------------

The results of the test are written to the `.trs` file,
with the output from the test written to the `.log` file,
for example,

    ```
    iguazu:$ make --no-print-directory check
    make  hello-omp tst-thrdbind
    make[1]: `hello-omp' is up to date.
    make[1]: `tst-thrdbind' is up to date.
    make  check-TESTS
    PASS: hello-omp
    PASS: tst-thrdbind
    ============================================================================
    Testsuite summary for moc 0.1
    ============================================================================
    # TOTAL: 2
    # PASS:  2
    # SKIP:  0
    # XFAIL: 0
    # FAIL:  0
    # XPASS: 0
    # ERROR: 0
    ============================================================================
    iguazu:$ 
    iguazu:$ 
    iguazu:$ cat hello-omp.log 
    # Run via test wrapper 'test/test-runner'
    ####################################################################
    #
    # CMD: orterun -np 2 ./hello-omp
    #
    ####################################################################

    hello(0) world(0)
    Done.
    hello(0) world(0)
    Done.

    ####################################################################
    # RETVAL: 0
    ####################################################################
    PASS hello-omp (exit status: 0)
    [3t4@iguazu test]$
    ```


For more details on the Automake Test Harness, see:
 - https://www.gnu.org/software/automake/manual/html_node/Tests.html


ADDING NEW TESTS
----------------

 - Add source code to `test/` directory, e.g., `test/foobar.c`
   and update the existing `Makefile.am` as stated below.

    - Add `foobar.c` to the `check_PROGRAMS` list in
      `test/Makefile.am`
    - As needed, add the per-program Automake specifications in
      `test/Makefile.am`, e.g., `foobar_SOURCES = foobar.c`

