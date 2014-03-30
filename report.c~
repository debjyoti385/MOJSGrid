/* Routines for storing population data into files */

# include <stdio.h>
# include <stdlib.h>
# include <math.h>
# include <string.h>
# include <time.h>
# include <float.h>
# include "global.h"
# include "rand.h"

//#define MORE_INFO 1

extern int *job_id;
extern double *normalized_frequency_coeff;

void sort_strategy(int *job_id,int *xreal, double *job_time_start, double *job_time, double *job_cost, double *job_limit,int *p_job_id,int *job_type);
int merge_sort(double arr[], int ind1[], int ind2[], int ind3[], double ind4[],double ind5[], double ind6[],int ind7[],  int low,int high);
int sort_m ( double arr[],int ind1[], int ind2[], int ind3[], double ind4[],double ind5[], double ind6[],int ind7[], int low,int mid,int high);
/* Function to print the information of a population in a file */
void report_pop (population *pop, FILE *fpt)
{
    int i, j, k;
    for (i=0; i<popsize; i++)
    {
        for (j=0; j<nobj; j++)
        {
            fprintf(fpt,"%f\t",pop->ind[i].obj[j]);
        }
        if (ncon!=0)
        {
            for (j=0; j<ncon; j++)
            {
                fprintf(fpt,"%f\t",pop->ind[i].constr[j]);
            }
        }
        if (nreal!=0)
        {
            for (j=0; j<nreal; j++)
            {
                fprintf(fpt,"%d -- %d: %f\n",job_id[j],pop->ind[i].xreal[j], pop->ind[i].job_time_start[j]);
            }
        }
        if (nbin!=0)
        {
            for (j=0; j<nbin; j++)
            {
                for (k=0; k<nbits[j]; k++)
                {
                    fprintf(fpt,"%d\t",pop->ind[i].gene[j][k]);
                }
            }
        }
        fprintf(fpt,"%f\t",pop->ind[i].constr_violation);
        fprintf(fpt,"%d\t",pop->ind[i].rank);
        fprintf(fpt,"%f\n",pop->ind[i].crowd_dist);
        
        for(j=0; j<NUM_RESOURCES; j++){        
	        fprintf(fpt,"%d : %f \n",resource_id[j],pop->ind[i].resource_free_time[j]);
    	}
    }
    
    
    return;
}

/* Function to print the information of feasible and non-dominated population in a file */
void report_feasible (population *pop, FILE *fpt)
{
    int i, j, k;
    for (i=0; i<popsize; i++)
    {
        if (pop->ind[i].constr_violation == 0.0 && pop->ind[i].rank==1)
        {
            for (j=0; j<nobj; j++)
            {
                fprintf(fpt,"%e\t",pop->ind[i].obj[j]);
            }
            if (ncon!=0)
            {
                for (j=0; j<ncon; j++)
                {
                    fprintf(fpt,"%e\t",pop->ind[i].constr[j]);
                }
            }
            if (nreal!=0)
            {
                for (j=0; j<nreal; j++)
                {
                    fprintf(fpt,"%d\t",pop->ind[i].xreal[j]);
                }
            }
            if (nbin!=0)
            {
                for (j=0; j<nbin; j++)
                {
                    for (k=0; k<nbits[j]; k++)
                    {
                        fprintf(fpt,"%d\t",pop->ind[i].gene[j][k]);
                    }
                }
            }
            fprintf(fpt,"%e\t",pop->ind[i].constr_violation);
            fprintf(fpt,"%d\t",pop->ind[i].rank);
            fprintf(fpt,"%e\n",pop->ind[i].crowd_dist);
        }
    }
    return;
}


/* Function to print the information of best and non-dominated population in a file */
individual report_best (population *pop, FILE *fpt)
{
    int i, j, k;
    FILE *fpt_debug;
    individual best;
    double min_obj= DBL_MAX, test_obj=0.0;
    double weight_fun[5]={2,1,1,1,1};
//    double weight_fun[4]={.1,8,0.1,0.1};
    allocate_memory_ind (&best);

    for (i=0; i<popsize; i++)
    {
    	test_obj=0.0;
        for (j=0; j<nobj; j++)
        {

            test_obj += weight_fun[j] * ((pop->ind[i].obj[j] - min_of_obj[j])/(max_of_obj[j]-min_of_obj[j]));  // modified in v8.5
        }

        if(test_obj > min_obj){
        	continue;
        }
        min_obj=test_obj;
        
        for (j=0; j<nobj; j++)
        {
            best.obj[j] = pop->ind[i].obj[j];
        }
        
        if (ncon!=0)
        {
            for (j=0; j<ncon; j++)
            {
				best.constr[j] = pop->ind[i].constr[j];
            }
        }
        if (nreal!=0)
        {
	        memcpy(best.xreal , pop->ind[i].xreal, sizeof(int)*NUM_JOBS );
	        memcpy(best.job_limit , pop->ind[i].job_limit, sizeof(double)*NUM_JOBS );
	        memcpy(best.job_time , pop->ind[i].job_time, sizeof(double)*NUM_JOBS );	        
           	memcpy(best.job_time_start, pop->ind[i].job_time_start, sizeof(double)*NUM_JOBS );
	        memcpy(best.job_cost , pop->ind[i].job_cost, sizeof(double)*NUM_JOBS );
	        memcpy(best.job_id , pop->ind[i].job_id, sizeof(int)*NUM_JOBS );
	        memcpy(best.job_type , pop->ind[i].job_type, sizeof(int)*NUM_JOBS );	        
	        memcpy(best.predecessor_job_id , pop->ind[i].predecessor_job_id, sizeof(int)*NUM_JOBS );


        }
        if (nbin!=0)
        {
            for (j=0; j<nbin; j++)
            {
                for (k=0; k<nbits[j]; k++)
                {
                    best.gene[j][k] = pop->ind[i].gene[j][k];
                }
            }
        }
        best.constr_violation = pop->ind[i].constr_violation;
		best.rank = pop->ind[i].rank;
		best.crowd_dist = pop->ind[i].crowd_dist;
		
		for(j=0; j<NUM_RESOURCES; j++){
		   	best.resource_free_time[j] = pop->ind[i].resource_free_time[j];
		}
		
    }
    fpt_debug= fopen("debug","a");
    fprintf(fpt_debug,"%f\t",min_obj );
    fclose(fpt_debug);

    print_best(best, fpt);
    return best;
}

void print_best (individual ind, FILE *fpt)
{
    int i, j, k,count_log,i_temp;
    FILE *fpt_debug;
    FILE *outfile[NUM_RESOURCES];
    double temp;
	char buf[20];

	long long int elapsed;
	double *time_logger;
	int * flag;
	

		time_logger = (double *)malloc((NUM_RESOURCES)* sizeof(double ));

	for(i=0;i<NUM_RESOURCES;i++){
//		time_logger[i] = resource_free_time[i] > (elapsed + module_running_time) ? resource_free_time[i] : (elapsed + module_running_time);
		time_logger[i] = resource_free_time[i] ;
	}
	
	for(i=0;i<NUM_RESOURCES;i++){
		sprintf(buf,"schedule_%d.out",resource_id[i]);
		outfile[i] = fopen(buf,"a");
	}

/* split time initialized */
	temp = ind.obj[0] - (module_running_time + chosen_split_time);
	for (i=0;i<5;i++){
		split_time[i]= (module_running_time + chosen_split_time) + (temp * (i+1) /5);			// changed from prev makespan time to chosen split time
	}

    fpt_debug = fopen("debug","a");
            fprintf(fpt_debug,"\n");
		for(j=0; j<NUM_RESOURCES; j++){        
	        fprintf(fpt_debug,"%d : %f \n",resource_id[j],ind.resource_free_time[j]);
    	}
    	        
        sort_strategy((&ind)->job_id, (&ind)->xreal, (&ind)->job_time_start, (&ind)->job_time, (&ind)->job_cost, (&ind)->job_limit,(&ind)->predecessor_job_id, (&ind)->job_type );

        for (j=0; j<nobj; j++)
        {
            fprintf(fpt_debug,"%f\t",ind.obj[j]);
        }
        if (ncon!=0)
        {
            for (j=0; j<ncon; j++)
            {
                fprintf(fpt_debug,"%f\t",ind.constr[j]);
            }
        }
		i=0;

        if (nreal!=0)
        {
            for (j=0; j<nreal; j++)
            {
                fprintf(outfile[ind.xreal[j]],"%d\t%d\t%f\t%f\t%f\t%f\t%d\t%d\tqueued\n",ind.job_id[j],resource_id[ind.xreal[j]], ind.job_time_start[j], (ind.job_time[j] / normalized_frequency_coeff[ind.xreal[j]]), ind.job_limit[j], ind.job_cost[j], ind.predecessor_job_id[j],ind.job_type[j]);

		            if(ind.job_time_start[j] >= split_time[i]){
		            	for(k=0;k<NUM_RESOURCES;k++){
		            	temp_free_time[k][i]=time_logger[k];			
		            	}
					i++;
		            }
                time_logger[ind.xreal[j]] = ind.job_time_start[j] + ( ind.job_time[j] / normalized_frequency_coeff[ind.xreal[j]]);
            }
            i_temp=i;
        	for(k=0;k<NUM_RESOURCES;k++){
        			for(i=i_temp;i<5;i++){
	                	temp_free_time[k][i]=time_logger[k];
                	}
            }
        }

	for(i=0;i<NUM_RESOURCES;i++){
		fclose(outfile[i]);
	}


        if (nbin!=0)
        {
            for (j=0; j<nbin; j++)
            {
                for (k=0; k<nbits[j]; k++)
                {
                    fprintf(fpt_debug,"%d\t",ind.gene[j][k]);
                }
            }
        }

        fprintf(fpt_debug,"%f\t",ind.constr_violation);
        fprintf(fpt_debug,"%d\t",ind.rank);
        fprintf(fpt_debug,"%f\n",ind.crowd_dist);



     fflush(fpt_debug);
     fclose(fpt_debug);
    return;
}

void sort_strategy(int *job_id,int *xreal, double *job_time_start, double *job_time, double *job_cost, double *job_limit,int *p_job_id, int *job_type){

	merge_sort(job_time_start,job_id,xreal, p_job_id, job_time,job_limit,job_cost,job_type, 0, NUM_JOBS-1);


}


int c_1[10000],c_2[10000], c_3[10000],c_7[10000];
double c_4[10000],c_5[10000],c_6[10000];

int sort_m ( double arr[],int ind1[], int ind2[], int ind3[], double ind4[],double ind5[], double ind6[],int ind7[], int low,int mid,int high)
{
 int i,j,k,l;
 double b[100002];
 l=low;
 i=low;
 j=mid+1;
 while((l<=mid)&&(j<=high))
   {
    if(arr[l]<=arr[j])
      {
       b[i]=arr[l];
       c_1[i]=ind1[l];
       c_2[i]=ind2[l];       
       c_3[i]=ind3[l]; 
       c_4[i]=ind4[l];
       c_5[i]=ind5[l];       
       c_6[i]=ind6[l];              
       c_7[i]=ind7[l];         
       l++;
      }
    else
      {
       b[i]=arr[j];
       c_1[i]=ind1[j];
       c_2[i]=ind2[j];       
       c_3[i]=ind3[j];       
       c_4[i]=ind4[j];
       c_5[i]=ind5[j];       
       c_6[i]=ind6[j];       
       c_7[i]=ind7[j];              
       j++;
      }
    i++;
   }
 if(l>mid)
   {
    for(k=j;k<=high;k++)
       {
        b[i]=arr[k];
        c_1[i]=ind1[k];
        c_2[i]=ind2[k];
        c_3[i]=ind3[k];
        c_4[i]=ind4[k];
        c_5[i]=ind5[k];
        c_6[i]=ind6[k];
        c_7[i]=ind7[k];
        i++;
       }
   }
 else
   {
    for(k=l;k<=mid;k++)
       {
        b[i]=arr[k];
        c_1[i]=ind1[k];        
        c_2[i]=ind2[k];
        c_3[i]=ind3[k];
        c_4[i]=ind4[k];        
        c_5[i]=ind5[k];
        c_6[i]=ind6[k];
        c_7[i]=ind7[k];
        i++;
       }
   }
 for(k=low;k<=high;k++)
    {
     arr[k]=b[k];
     ind1[k]=c_1[k];
     ind2[k]=c_2[k];     
     ind3[k]=c_3[k];     
     ind4[k]=c_4[k];
     ind5[k]=c_5[k];     
     ind6[k]=c_6[k];     
     ind7[k]=c_7[k];     
    }
    return 0;
}

int merge_sort(double arr[], int ind1[], int ind2[], int ind3[], double ind4[],double ind5[], double ind6[],int ind7[],  int low,int high)
{
 int mid;
 if(low<high)
   {
    mid=(low+high)/2;
    merge_sort(arr,ind1,ind2, ind3,ind4, ind5, ind6,ind7,low,mid);
    merge_sort(arr,ind1,ind2, ind3,ind4, ind5, ind6,ind7, mid+1,high);
    sort_m(arr,ind1,ind2, ind3,ind4, ind5, ind6,ind7, low,mid,high);
   }
   return 0;
}

