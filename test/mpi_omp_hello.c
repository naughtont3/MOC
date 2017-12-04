/******************************************************************************
* FILE: omp_hello.c
* DESCRIPTION:
*   MPI/OpenMP Example - Hello World - C/C++ Version
*   In this simple example, the master thread forks a parallel region.
*   All threads in the team obtain their unique thread number and print it.
*   The master thread only prints the total number of threads.  Two OpenMP
*   library routines are used to obtain the number of threads and each
*   thread's number.
* AUTHORS: Blaise Barney  5/99 (for the OpenMP part)
*          Geoffroy Vallee (for the MPI+OpenMP aspects)
******************************************************************************/
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <moc.h>

/********************************************/
/* Specific to inter-runtime communications */
/********************************************/

int
main (int argc, char *argv[]) 
{
    int nthreads, tid;
    moc_info_t *moc_policy_info;

    fprintf (stderr, "[%s:%s:%d] Calling MPI_Init()...\n", __FILE__, __func__, __LINE__);
    MPI_Init (&argc, &argv);
    fprintf (stderr, "[%s:%s:%d] MPI_Init() succeeded\n", __FILE__, __func__, __LINE__);

    MOC_Init (MPI_COMM_WORLD);

    MOC_INFO_CREATE (moc_policy_info, 1);
    MOC_INFO_LOAD (&moc_policy_info[0], "moc.policy.threadspan", "max", MOC_STRING);
    MOC_Policy_set (moc_policy_info, 1);
    MOC_INFO_FREE (moc_policy_info, 1);

    /* Fork a team of threads giving them their own copies of variables */
    #pragma omp parallel private(nthreads, tid)
    {

        /* Obtain thread number */
        fprintf (stderr, "[%s:%s:%d] In parallel loop: calling omp_get_thread_num()...\n", __FILE__, __func__, __LINE__);
        tid = omp_get_thread_num();
        fprintf (stderr, "[%s:%s:%d] omp_get_thread_num() succeeded\n", __FILE__, __func__, __LINE__);
        printf("Hello World from thread = %d\n", tid);

        /* Only master thread does this */
        if (tid == 0) 
        {
            nthreads = omp_get_num_threads();
            printf("Number of threads = %d\n", nthreads);
        }

    }  /* All threads join master thread and disband */

    MOC_Fini ();

    MPI_Finalize ();

    return EXIT_SUCCESS;
}
