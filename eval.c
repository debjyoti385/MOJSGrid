/* Routine for evaluating population members  */

# include <stdio.h>
# include <stdlib.h>
# include <math.h>

# include "global.h"
# include "rand.h"

/* #define MORE_INFO 1 */

/* Routine to evaluate objective function values and constraints for a population */
void evaluate_pop (population *pop)
{
    int i;
	FILE *f;    
#ifdef MORE_INFO     
   	f=fopen("debug","w");    	    
    fprintf(f,"Generation \n");   	
	fclose(f);    
#endif     
    for (i=0; i<popsize; i++)
    {

        evaluate_ind (&(pop->ind[i]));
    }
#ifdef MORE_INFO
   	f=fopen("debug","a");    
    fprintf(f,"population evalution complete\n");
	fclose(f); 
#endif     
    return;
}

/* Routine to evaluate objective function values and constraints for an individual */
void evaluate_ind (individual *ind)
{
    int j;
	FILE *f;
#ifdef MORE_INFO	
	f=fopen("debug","a");
    fprintf(f,"pop num %d\n",count);
	fclose(f);    
#endif

    test_problem (ind->xreal, ind->xbin, ind->gene, ind->obj, ind->constr, ind->job_time_start, ind->resource_free_time, ind->job_time, ind->job_cost, ind->predecessor_job_id, ind->job_limit , ind->job_id, ind->job_type );
    


    
    if (ncon==0)
    {
        ind->constr_violation = 0.0;
    }
    else
    {
        ind->constr_violation = 0.0;
        for (j=0; j<ncon; j++)
        {
            if (ind->constr[j]<0.0)
            {
                ind->constr_violation += ind->constr[j];
            }
        }
    }
    
#ifdef MORE_INFO	
	f=fopen("debug","a");
    fprintf(f,"exiting evaluate ind for pop %d\n",count++);
	fclose(f);    
#endif
    return;
}
