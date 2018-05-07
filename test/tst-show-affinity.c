/*
 * OpenMP Notes
 *  - 'OMP_NUM_THREADS=<x>' to limit number of threads executing
 *  - 'OMP_PROC_BIND=true'  to enable OpenMP binding
 *  - 'OMP_PLACES=<loc>'    to set places where thread may execute
 */

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/syscall.h>
//#include <pthread.h>
#include <sched.h>

#include <mpi.h>
#include <omp.h>


/*
 * Global Variables
 */

int     VERBOSE = 0;
int     HELP    = 0;


/*
 * Prototypes
 */
void usage(char *);
int cpuaff2str(cpu_set_t , char **);


/*
 * Functions
 */
void usage(char *exe_name)
{
    printf("Usage: %s  [OPTIONS]\n", exe_name);
    printf(" [OPTIONS]\n");
    printf(" %10s  %s\n", "-h", "Show usage.");
    printf(" %10s  %s\n", "-v", "Verbose mode (show field labels).");
    return;
}


int cpuaff2str(cpu_set_t mask, char **str)
{
    int i;
    int nprocs = sysconf(_SC_NPROCESSORS_ONLN);
    char buf[256], *ptr; 

    ptr = buf;

    for (i=0; i < nprocs; i++) {
        if (CPU_ISSET(i, &mask)) {
            sprintf(ptr, "%d ", i);
            ptr = buf + strlen(buf);
        }
    }

    *str = strdup(buf);

    return (0);
}

int main (int argc, char ** argv)
{
    int         i;
    int         rc;
    int         omp_tid = 0;
    pid_t       pid;
    int         rank = 0;
    int         size = 0;
    int         opt;
    cpu_set_t   proc_cpuset;
    cpu_set_t   *proc_cpusetp;
    char        *proc_str = NULL;
    int         proc_cpucount = 0;

    opterr = 1;
    while ((opt = getopt(argc, argv, "vh")) != -1) {
        switch (opt) {
        case 'h':
            HELP = 1;
            break;

        case 'v':
            VERBOSE = 1;
            break;

        case '?':
            fprintf (stderr, "ERROR: Unknown option '-%c'\n", optopt);
            exit (EXIT_FAILURE);
            break;

        default:
            /* nothing */
            break;
        }
    }

    if (HELP) {
        usage(argv[0]);
        exit (EXIT_SUCCESS);
    }

    /* Get Process ID (PID) */
    pid = getpid();

    /* Initialize MPI */
    if (0 != (rc = MPI_Init(&argc, &argv))) {
        fprintf (stderr, "[%d] ERROR: MPI_Init failed (%d)\n", rc);
        return (EXIT_FAILURE);
    }

    /* Get MPI Rank in World */
    if (0 != (rc = MPI_Comm_rank(MPI_COMM_WORLD, &rank))) {
        fprintf (stderr, "[%d] ERROR: MPI_Comm_rank failed (%d)\n", rc);
        return (EXIT_FAILURE);
    }

    /* Get MPI World Size */
    if (0 != (rc = MPI_Comm_size(MPI_COMM_WORLD, &size))) {
        fprintf (stderr, "[%d] ERROR: MPI_Comm_size failed (%d)\n", rc);
        return (EXIT_FAILURE);
    }

    /* Get Process CPU binding (affinity) */
    /* NOTE: If pid=0, return mask of calling process */
    sched_getaffinity(0, sizeof(proc_cpuset), &proc_cpuset);
    proc_cpucount = CPU_COUNT(&proc_cpuset);
    cpuaff2str(proc_cpuset, &proc_str);



#pragma omp parallel private(omp_tid)
{
    pid_t       tid;
    cpu_set_t   thrd_cpuset;
    cpu_set_t   *thrd_cpusetp;
    char        *thrd_str = NULL;
    int         thrd_cpucount = 0;
    //int         omp_tid = 0;

    /* Get (OpenMP) Thread ID */
    omp_tid = omp_get_thread_num();

    /* Get (system) Thread ID */
    tid = syscall(SYS_gettid);

    /* Get Thread CPU binding (affinity) */
    /*
     * thread = pthread_self();
     * pthread_getaffinity_np(thread, sizeof(thrd_cpuset), &thrd_cpuset);
     */
    /* If pid=0, return mask of calling process */
    sched_getaffinity(tid, sizeof(thrd_cpuset), &thrd_cpuset);
    thrd_cpucount = CPU_COUNT(&thrd_cpuset);

    /* Get string-ify version of cpuset mask */
    cpuaff2str(thrd_cpuset, &thrd_str);

    if (VERBOSE) {
        printf ("(%0d/%0d) PID:%6d TID:%6d OMP_TID:%3d pNCPU:%5d  pCPUAff: %s  tNCPU:%5d  tCPUAff: %s\n",
                rank, size, pid, tid, omp_tid, proc_cpucount, proc_str, thrd_cpucount, thrd_str);
    } else {
        printf ("(%0d/%0d) %6d %6d %3d (%d)pCPUAff: %s  (%d)tCPUAff: %s\n",
                rank, size, pid, tid, omp_tid, proc_cpucount, proc_str, thrd_cpucount, thrd_str);
    }

    fflush(stdout);
    
    if (NULL != thrd_str) {
        free(thrd_str);
    }
}
    if (NULL != proc_str) {
        free(proc_str);
    }

    MPI_Finalize();

    return (0);
}
