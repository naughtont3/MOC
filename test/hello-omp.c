/*
 * Note: Use 'OMP_NUM_THREADS' to limit number of threads executing,
 *       (e.g., env OMP_NUM_THREADS=16 ./hello-omp_llvm )
 *
 */

#include <stdio.h>
#include <omp.h>

int main (int argc, char ** argv)
{
    int i;
    int ID = 0;

#pragma omp parallel
{
   ID = omp_get_thread_num();
   printf("hello(%d) ", ID);
   printf("world(%d)\n", ID);

#if 0
    #pragma omp for
    for (i=0; i < 4; i++) {
        printf("hello_(%d) ", ID);
    }
#endif
}
//    printf("world ID=%d\n", ID);

    printf("Done.\n");
    return (0);
}
