#ifndef MOC_H
#define MOC_H

#include <mpi.h>
#include <pmix.h>
#include "moc_common.h"

#if defined(c_plusplus) || defined(__cplusplus)
extern "C" {
#endif

MOC_EXPORT moc_status_t MOC_Init (MPI_Comm comm);

MOC_EXPORT moc_status_t MOC_Fini (void);

MOC_EXPORT moc_status_t MOC_Policy_set (pmix_info_t *policy, size_t n);

#if defined(c_plusplus) || defined(__cplusplus)
}
#endif

#endif /* MOC_H */
