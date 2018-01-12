#define _GNU_SOURCE

#include <pmix.h>
#include <stdio.h>
#include <unistd.h>
#include "moc.h"

uint32_t        node_local_procs    = 0;
pmix_proc_t     myproc;

/*
 * 'USE_MOC_RANKED_PLACE' -- Have MOC set the OMP_PLACE based
 *    on MPI Rank during MOC_Init(). Use with "vanilla" omp runtimes.
 *
 *    To enable, set true (1),
 *   to disable, set false (0)
 */
#define USE_MOC_RANKED_PLACE  (1)

moc_status_t
MOC_Init (MPI_Comm comm)
{
    int             i, rc;
    char            *s_procNames        = NULL;
    char            *r_procNames        = NULL;
//    uint32_t        node_local_procs    = 0;
    pmix_pdata_t    *lookup_pdata;
    int             dummy               = 0;
    int             lookup_range        = PMIX_RANGE_GLOBAL;
    pmix_value_t    *val;
    pmix_proc_t     proc;
    int             numCPU;
    pmix_value_t    value;
  #if (USE_MOC_RANKED_PLACE)
    char            *_local_ranks_str   = NULL;
    int             _local_myrank       = -1;
    bool            _local_myrank_set   = false;
    int             _local_place_offset = -1;
    bool            _local_place_offset_set = false;
  #endif /* USE_MOC_RANKED_PLACE */

    rc = PMIx_Init (&myproc, NULL, 0);
    if (rc != PMIX_SUCCESS)
    {
        fprintf (stderr, "[%s:%s:%d] ERROR: PMIx_Init() failed\n", __FILE__, __func__, __LINE__);
        return MOC_ERROR;
    }

  #if (USE_MOC_RANKED_PLACE)
    /* Set our rank (self) */
    _local_myrank     = myproc.rank;
    _local_myrank_set = true;
  #endif /* USE_MOC_RANKED_PLACE */

    /* First we try to see if the info is already available */
    PMIX_PDATA_CREATE (lookup_pdata, 1);
    (void)strncpy(lookup_pdata[0].key, PMIX_LOCAL_SIZE, PMIX_MAX_KEYLEN);

    rc = PMIx_Lookup (lookup_pdata, 1, NULL, 0);
    rc = PMIX_ERROR;
    if (rc != PMIX_SUCCESS)
    {
        MPI_Comm        shmem_comm;
        int             nprocs;

        /* if not we set it */

        /* Rely on MPI3 features to figure out the number of local ranks: split the communicator
           to get ranks that can set a shared memory between them. Not perfect but does the job
           for now. */
        MPI_Comm_split_type (MPI_COMM_WORLD, MPI_COMM_TYPE_SHARED, 0, MPI_INFO_NULL, &shmem_comm);
        MPI_Comm_size (shmem_comm, &nprocs);

        PMIX_PROC_CONSTRUCT(&proc);
        (void)strncpy(proc.nspace, myproc.nspace, PMIX_MAX_NSLEN);
        proc.rank = PMIX_RANK_WILDCARD;

        PMIx_Fence (&proc, 1, NULL, 0);

        node_local_procs = (uint32_t)nprocs;

        PMIX_VAL_SET (&value, uint32_t, node_local_procs);
        rc = PMIx_Put (PMIX_GLOBAL, PMIX_LOCAL_SIZE, &value);
        if (rc != PMIX_SUCCESS)
        {
            fprintf (stderr, "[%s:%s:%d] ERROR: PMIx_Put() failed\n", __FILE__, __func__, __LINE__);
        }

        PMIx_Commit ();
        PMIx_Fence (&proc, 1, NULL, 0);

        MPI_Comm_free (&shmem_comm);
    }

    PMIX_PDATA_FREE (lookup_pdata, 1);

    numCPU = sysconf (_SC_NPROCESSORS_ONLN);
    fprintf (stderr, "[%s:%s:%d] numCPUS: %d\n", __FILE__, __func__, __LINE__, numCPU);
    {
        pmix_info_t *pubinfo;

        PMIX_INFO_CREATE (pubinfo, 1);
        (void)strncpy(pubinfo[0].key, "moc.ncpus", PMIX_MAX_KEYLEN);
        pubinfo[0].value.type = PMIX_UINT8;
        pubinfo[0].value.data.uint8 = (uint8_t)numCPU;
        PMIx_Publish (pubinfo, 1);
    }


  #if (USE_MOC_RANKED_PLACE)
  {
    /* Get list of ranks on local node */
    rc = PMIx_Get(&proc, PMIX_LOCAL_PEERS, NULL, 0, &val);
    if (rc != PMIX_SUCCESS)
    {
        fprintf (stderr, "[%s:%s:%d] ERROR: PMIx_Get() failed for 'PMIX_LOCAL_PEERS'\n", __FILE__, __func__, __LINE__);
        fprintf (stderr, "[%s:%s:%d] WARN: Unable to set OMP PLACMENT INFO'\n", __FILE__, __func__, __LINE__);
    }
    else
    {
        if (val->type != PMIX_STRING)
        {
            fprintf (stderr, "[%s:%s:%d] ERROR: Incorrect type for 'PMIX_LOCAL_PEERS' data\n", __FILE__, __func__, __LINE__);
        }
        else
        {
            int idx, tmp_rank;
            char *ptr, *saveptr, *token;
            ptr=saveptr=token=NULL;

            _local_ranks_str = strdup((char*)(val->data.string));

            /* Find our placement offset (index) in local ranks list */
            for (idx=0, ptr=_local_ranks_str, saveptr=NULL; ; idx++, ptr=NULL)
            {
                token = strtok_r(ptr, ",", &saveptr);

                if (NULL == token)
                    break;

                tmp_rank = atoi(token);

                if (_local_myrank == tmp_rank) {
                    _local_place_offset = idx;
                    _local_place_offset_set = true;
                    fprintf (stderr, "[%s:%s:%d] MOC-RankPlace: myank: %d placeOffset: %d\n", __FILE__, __func__, __LINE__, _local_myrank, _local_place_offset);
                    break;
                }
            }

            if (false == _local_place_offset_set)
            {
                fprintf (stderr, "[%s:%s:%d] ERROR: Rank (%d) not found local list\n", __FILE__, __func__, __LINE__, _local_myrank);
                fprintf (stderr, "[%s:%s:%d] WARN: Unable to set OMP PLACMENT INFO'\n", __FILE__, __func__, __LINE__);
            }
            else
            {
				// Place-stuff
				/* Setup Places Info (assuming consecutive core based places) */
				int stride, my_start_place, my_finish_place;
				int idx, cnt;
				char *tmp_str = NULL;

				/* Split local cores (cpus) among ranks */
				stride = numCPU / node_local_procs;

				/* Start of this rank placement set of cores */
				my_start_place  = _local_place_offset * stride;

				/* End of this rank placement set of cores */
				my_finish_place = ((_local_place_offset * stride) + stride) - 1;

				//fprintf (stderr, "[%s:%s:%d] MOC: %d  start: %d  finish: %d stride: %d\n",
				//           __FILE__, __func__, __LINE__, _local_myrank,
				//           my_start_place, my_finish_place, stride);

				/* XXX: For now just show this info */
				for (idx=my_start_place, cnt=0; idx <= my_finish_place; idx++) {
					if (tmp_str == NULL) {
						asprintf(&tmp_str, "{%d}", idx);
					}
					else
					{
						asprintf(&tmp_str, "%s, {%d}", tmp_str, idx);
					}
				}

			  #if 1
				/* Set rank specific places (avoid override if OMP_PLACES already defined) */
				if (NULL != getenv("OMP_PLACES"))
				{
					fprintf (stderr, "[%s:%s:%d] MOC: (pid:%d) Rank: %d OMP_PLACES ALREADY SET\n", __FILE__, __func__, __LINE__, (int)getpid(), _local_myrank);
				}
				else
				{
					fprintf (stderr, "[%s:%s:%d] MOC: (pid:%d) Rank: %d OVERRIDE OMP_PLACES INFO (%s)\n", __FILE__, __func__, __LINE__, (int)getpid(), _local_myrank, tmp_str);
					setenv("OMP_PLACES", tmp_str, 1);
				}
			  #endif

				fprintf (stderr, "[%s:%s:%d] MOC: (pid:%d) Rank: %d  places: %s\n", __FILE__, __func__, __LINE__, (int)getpid(), _local_myrank, tmp_str);

				if (NULL != tmp_str)
					free(tmp_str);

            } // Place-stuff
        }
    }
  }
  #endif /* USE_MOC_RANKED_PLACE */

    PMIx_Commit ();
    PMIx_Fence (&proc, 1, NULL, 0);

    return MOC_SUCCESS;
}

moc_status_t
MOC_Policy_set (pmix_info_t *policy, size_t npolicy)
{
    pmix_proc_t     proc;

    PMIX_PROC_CONSTRUCT(&proc);
    (void)strncpy(proc.nspace, myproc.nspace, PMIX_MAX_NSLEN);
    proc.rank = PMIX_RANK_WILDCARD;

    PMIx_Publish (policy, npolicy);
    PMIx_Commit ();
    PMIx_Fence (&proc, 1, NULL, 0);

    return MOC_SUCCESS;
}

moc_status_t
MOC_Fini (void)
{
    PMIx_Finalize (NULL, 0);

    return MOC_SUCCESS;
}
