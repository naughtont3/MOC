/*
 * Playing with getaffinity and thread identifiers.
 */
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <pthread.h>
#include <sched.h>

#define NUM_THREADS  5

void *print_hello (void *tid)
{
    pid_t mypid;
    pid_t mytid;
    cpu_set_t cpuset;
    cpu_set_t *cpusetp;
    int count = 0;
    pthread_t thread;

    //printf("[%s] Hello World ThreadNum = %ld\n", __func__, (long)tid);

    mypid = getpid();               /* Process ID */
    mytid = syscall(SYS_gettid);    /* Thread  ID */
    thread = pthread_self();

    /* If pid=0, return mask of calling process */
    //sched_getaffinity(0, sizeof(cpuset), &cpuset);
    //sched_getaffinity(mytid, sizeof(cpuset), &cpuset);
    pthread_getaffinity_np(thread, sizeof(cpuset), &cpuset);

    count = CPU_COUNT(&cpuset);

    printf ("[%s] ProcessID = %d    ThreadID = %d   CPU_Count = %d\n", 
            __func__, mypid, mytid, count);

    pthread_exit(NULL);

    return 0;
}

int main (int argc, char **argv)
{
    int rc;
    long i;
    pid_t pid;
    pid_t tid;
    pthread_t threads[NUM_THREADS];

    pid = getpid();  /* Process ID */
    tid = syscall(SYS_gettid);  /* Thread  ID */
    printf ("[%s] ProcessID = %d    ThreadID = %d\n", __func__, pid, tid);

    for (i=0; i < NUM_THREADS; i++) {
        rc = pthread_create(&threads[i], NULL, print_hello, (void*)i);
    }

    pthread_exit(NULL);

    return (EXIT_SUCCESS);
}
