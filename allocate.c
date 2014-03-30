/* Memory allocation and deallocation routines */

# include <stdio.h>
# include <stdlib.h>
# include <math.h>

# include "global.h"
# include "rand.h"

/* Function to allocate memory to a population */
void allocate_memory_pop (population *pop, int size)
{
    int i;
    pop->ind = (individual *)malloc(size*sizeof(individual));
    for (i=0; i<size; i++)
    {
        allocate_memory_ind (&(pop->ind[i]));
    }
    return;
}

/* Function to allocate memory to an individual */
void allocate_memory_ind (individual *ind)
{
    int j;
    if (nreal != 0)
    {
        ind->xreal = (int*)malloc(nreal*sizeof(int ));			/* modified */
        ind->job_time_start = (double*)malloc(nreal*sizeof(double ));			/* modified */        
        ind->job_time = (double*)malloc(nreal*sizeof(double ));			/* modified */                
        ind->job_limit = (double*)malloc(nreal*sizeof(double ));			/* modified */                        
        ind->job_cost = (double*)malloc(nreal*sizeof(double ));			/* modified */                
        ind->resource_free_time = (double*)malloc(NUM_RESOURCES*sizeof(double ));			/* modified */        
        ind->job_id = (int*)malloc(nreal*sizeof(int )); 		/* modified */
		ind->predecessor_job_id = (int*)malloc(nreal*sizeof(int )); 		/* modified */
		ind->job_type = (int*)malloc(nreal*sizeof(int )); 		/* modified */		
    }
    if (nbin != 0)
    {
        ind->xbin = (double *)malloc(nbin*sizeof(double));
        ind->gene = (int **)malloc(nbin*sizeof(int *));
        for (j=0; j<nbin; j++)
        {
            ind->gene[j] = (int *)malloc(nbits[j]*sizeof(int));
        }
    }
    ind->obj = (double *)malloc(nobj*sizeof(double));
    if (ncon != 0)
    {
        ind->constr = (double *)malloc(ncon*sizeof(double));
    }
    return;
}

/* Function to deallocate memory to a population */
void deallocate_memory_pop (population *pop, int size)
{
    int i;
    for (i=0; i<size; i++)
    {
        deallocate_memory_ind (&(pop->ind[i]));
    }
    free (pop->ind);
    return;
}

/* Function to deallocate memory to an individual */
void deallocate_memory_ind (individual *ind)
{
    int j;
    if (nreal != 0)
    {
        free(ind->xreal);
        free(ind->job_time_start);
        free(ind->job_cost);        
        free(ind->job_time);                
        free(ind->job_limit);        
        free(ind->resource_free_time);
        free(ind->job_id);
        free(ind->predecessor_job_id);        
        free(ind->job_type);        
    }
    if (nbin != 0)
    {
        for (j=0; j<nbin; j++)
        {
            free(ind->gene[j]);
        }
        free(ind->xbin);
        free(ind->gene);
    }
    free(ind->obj);
    if (ncon != 0)
    {
        free(ind->constr);
    }
    return;
}
