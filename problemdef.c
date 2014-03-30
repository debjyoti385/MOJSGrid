/* Test problem definitions */

# include <stdio.h>
# include <stdlib.h>
# include <math.h>
# include <string.h>
# include <map>
# include "global.h"
# include "rand.h"

extern std::map<int,gridlet> job_bag;
/* #define MORE_INFO  1 */

#define JOB_SCHEDULER

/*  Test problem JOB_SCHEDULER
    # of real variables = # of Jobs = N    [0,NUM_RESOURCES-1]  M is number of resources
    # of bin variables = 0
    # of objectives = 4  (later will be extended to 5)
    
    # of constraints = M 
    
 changes  xreal converted from double to int
 Insert job_time[i], job_start_time[i], job_end_time[i] in main
 introduction of NUM_RESOURCES and NUM_JOBS
	*/
#ifdef JOB_SCHEDULER

/* merge sort START */

int c1[100002];
int c2[100002];
double c3[100002], c4[100002],c5[100002];
int sort(int arr[],int ind1[],int ind2[],double ind3[],double ind4[],double ind5[], int low,int mid,int high)
{
 int i,j,k,l;
 long int b[100002];
 l=low;
 i=low;
 j=mid+1;
 while((l<=mid)&&(j<=high))
   {
    if(arr[l]<=arr[j])
      {
       b[i]=arr[l];
       c1[i]=ind1[l];
       c2[i]=ind2[l];       
       c3[i]=ind3[l];              
       c4[i]=ind4[l];
       c5[i]=ind5[l];       
       l++;
      }
    else
      {
       b[i]=arr[j];
       c1[i]=ind1[j];
       c2[i]=ind2[j];
       c3[i]=ind3[j];       
       c4[i]=ind4[j];
       c5[i]=ind5[j];       
       j++;
      }
    i++;
   }
 if(l>mid)
   {
    for(k=j;k<=high;k++)
       {
        b[i]=arr[k];
        c1[i]=ind1[k];
        c2[i]=ind2[k];        
        c3[i]=ind3[k];                
        c4[i]=ind4[k];                     
        c5[i]=ind5[k];      
        i++;
       }
   }
 else
   {
    for(k=l;k<=mid;k++)
       {
        b[i]=arr[k];
        c1[i]=ind1[k];
        c2[i]=ind2[k];
        c3[i]=ind3[k];        
        c4[i]=ind4[k];                
        c5[i]=ind5[k];                
        i++;
       }
   }
 for(k=low;k<=high;k++)
    {
     arr[k]=b[k];
     ind1[k]=c1[k];
     ind2[k]=c2[k];
     ind3[k]=c3[k];
     ind4[k]=c4[k];     
     ind5[k]=c5[k];     
    }
    return 0;
}

int partition(int arr[], int ind1[], int ind2[],double ind3[],double ind4[],double ind5[], int low,int high)
{
 int mid;
 if(low<high)
   {
    mid=(low+high)/2;
    partition(arr,ind1,ind2,ind3,ind4,ind5, low,mid);
    partition(arr,ind1,ind2,ind3,ind4,ind5, mid+1,high);
    sort(arr,ind1,ind2,ind3,ind4,ind5, low,mid,high);
   }
   return 0;
}



/* merge sort END */	
extern int NUM_RESOURCES;
extern int NUM_JOBS;
extern int *job_id;
extern double *job_time;
extern double *job_time_start;
extern double *job_time_end;
extern double *time_limit;
extern double *normalized_power;
extern double *normalized_frequency_coeff;
extern double *resource_cost;
extern double *job_cost;
extern double *resource_free_time;
extern int *job_type;

void test_problem (int *xreal, double *xbin, int **gene, double *obj, double *constr, double *start_time, double *r_free_time,double *j_time, double *j_cost, int *p_job_id,double *j_limit, int *j_id, int *j_type)
{
int i,j;
/*double time_consumed[NUM_RESOURCES]={0.0}; */
double *time_consumed;
double max;
double time_tardiness=0.0;
double cost_tardiness=0.0;
double non_utilization=0.0;
double 	**diff;					/* difference of timelimit and job_time */
int **index;						/* maintain ramk of each job in that resource */

double calc_power_consumed=0.0;
double start=0.0, end=0.0;
double *temp_resource_free_time;
int *q, length=0;						

 int *t_predecessor_job_id;
 int *t_xreal;
 int *t_job_id;
 double *t_job_time;
 double *t_time_limit;
 double *t_job_cost;

t_predecessor_job_id = (int*) malloc(sizeof(int)*(NUM_JOBS+10));
t_xreal = (int*) malloc(sizeof(int)*(NUM_JOBS+10));
t_job_id = (int*) malloc(sizeof(int)*(NUM_JOBS+10));
t_job_time = (double*) malloc(sizeof(double)*(NUM_JOBS+10));
t_time_limit = (double*) malloc(sizeof(double)*(NUM_JOBS+10));
t_job_cost = (double*) malloc(sizeof(double)*(NUM_JOBS+10));

memcpy(t_predecessor_job_id , predecessor_job_id, sizeof(int)*NUM_JOBS );
memcpy(p_job_id , predecessor_job_id, sizeof(int)*NUM_JOBS );
memcpy(t_job_id , job_id, sizeof(int)*NUM_JOBS );
memcpy(j_id , job_id, sizeof(int)*NUM_JOBS );
memcpy(t_xreal , xreal, sizeof(int)*NUM_JOBS );
memcpy(t_job_time , job_time, sizeof(double)*NUM_JOBS );
memcpy(j_time , job_time, sizeof(double)*NUM_JOBS );
memcpy(t_time_limit , time_limit, sizeof(double)*NUM_JOBS );
memcpy(j_limit , time_limit, sizeof(double)*NUM_JOBS );
memcpy(t_job_cost , job_cost, sizeof(double)*NUM_JOBS );
memcpy(j_cost , job_cost, sizeof(double)*NUM_JOBS );
memcpy(j_type , job_type, sizeof(int)*NUM_JOBS );



FILE *f;

#ifdef MORE_INFO    

f=fopen("debug","a");
	fprintf(f,"Entered testproblem.... \n") ;
#endif

std::map <int,double> endtime;
std::map <int,double>::iterator it_e;

std::map <int,double> starttime;
std::map <int,double>::iterator it_s;

time_consumed = (double *)malloc(NUM_RESOURCES * sizeof(double));

temp_resource_free_time = (double *)malloc(NUM_RESOURCES * sizeof(double));
q = (int *) malloc(NUM_JOBS * sizeof(int));

memset(time_consumed,0,NUM_RESOURCES * sizeof(double));  

for(i=0;i<NUM_JOBS;i++){

#ifdef MORE_INFO   

	fprintf(f," xreal[%d] : %d \t",i, xreal[i]);
#endif	

	xreal[i]=(xreal[i])%NUM_RESOURCES; 
	xreal[i]=(xreal[i]+NUM_RESOURCES)%NUM_RESOURCES; 
}

#ifdef MORE_INFO    
fprintf(f,"\n");
#endif
#ifdef MORE_INFO    		
fclose(f);	
#endif



partition(t_predecessor_job_id,t_job_id,t_xreal,t_job_time,t_time_limit,t_job_cost,0,NUM_JOBS-1);
for(i=0;i<NUM_RESOURCES;i++){
	temp_resource_free_time[i]=resource_free_time[i];
}
for(i=0;i<NUM_JOBS;i++){
	// if pred id less than 0 
	time_consumed[xreal[i]] += (t_job_time[i] / normalized_frequency_coeff[t_xreal[i]]);
	if (t_predecessor_job_id[i] < 0 ){
		starttime[t_job_id[i]] = temp_resource_free_time[t_xreal[i]];
		endtime[t_job_id[i]] = starttime[t_job_id[i]] + (t_job_time[i] / normalized_frequency_coeff[t_xreal[i]]);
		temp_resource_free_time[t_xreal[i]] = endtime[t_job_id[i]];
	}
	else
	{
		if (endtime.find(t_predecessor_job_id[i]) != endtime.end()){
			starttime[t_job_id[i]] = endtime[t_predecessor_job_id[i]] > temp_resource_free_time[t_xreal[i]] ? endtime[t_predecessor_job_id[i]] : temp_resource_free_time[t_xreal[i]];
			endtime[t_job_id[i]] = starttime[t_job_id[i]] + (t_job_time[i] / normalized_frequency_coeff[t_xreal[i]] );
			temp_resource_free_time[t_xreal[i]] = endtime[t_job_id[i]];
		}
		else 
		{
			if (job_bag.find(t_predecessor_job_id[i]) != job_bag.end()){
				std::map<int,gridlet>::iterator temp_it = job_bag.find(t_predecessor_job_id[i]);
				starttime[t_job_id[i]] = (temp_it->second.start + temp_it->second.exec_time) > temp_resource_free_time[t_xreal[i]] ? (temp_it->second.start + temp_it->second.exec_time) : temp_resource_free_time[t_xreal[i]];
				endtime[t_job_id[i]] = starttime[t_job_id[i]] + (t_job_time[i] / normalized_frequency_coeff[t_xreal[i]] );
				temp_resource_free_time[t_xreal[i]] = endtime[t_job_id[i]];
			}
			else{
			starttime[t_job_id[i]] = temp_resource_free_time[t_xreal[i]];
			endtime[t_job_id[i]] = starttime[t_job_id[i]] + (t_job_time[i] / normalized_frequency_coeff[t_xreal[i]]);
			temp_resource_free_time[t_xreal[i]] = endtime[t_job_id[i]];
			}
		}
	}
	

	
	if(endtime[t_job_id[i]] > t_time_limit[i]){
		time_tardiness+= endtime[t_job_id[i]] - t_time_limit[i];
	}

	if(t_job_cost[i] < resource_cost[t_xreal[i]] * (t_job_time[i] / normalized_frequency_coeff[t_xreal[i]] ) ){					
					cost_tardiness += resource_cost[t_xreal[i]] * (t_job_time[i] / normalized_frequency_coeff[t_xreal[i]]) - t_job_cost[i];
	}


	
}

for(i=0;i<NUM_JOBS;i++){
	start_time[i] = starttime[job_id[i]];
}
max=0.0;
for(i=0;i<NUM_RESOURCES;i++){
	r_free_time[i] = temp_resource_free_time[i];
	non_utilization += (temp_resource_free_time[i] - resource_free_time[i]) - time_consumed[i] ;
	if(max < r_free_time[i]){
	max = r_free_time[i];
	}
	calc_power_consumed += time_consumed[i] * normalized_power[i] + non_utilization * normalized_power[i] * 0.6;  /* DVM tech */
}

	obj[0]= max;					//* max (time_consumed[j])  objective minimize this 

	obj[1]= calc_power_consumed;	//* minimize power consumption 
	
	obj[2]= time_tardiness;
	
	obj[3]= cost_tardiness;
	
	obj[4]= non_utilization;
	
	for(i=0;i<nobj;i++){
		if (obj[i] < min_of_obj[i]){
			min_of_obj[i]=obj[i];
		}
		if (obj[i] > max_of_obj[i]){
			max_of_obj[i]=obj[i];
		}
	}
#ifdef MORE_INFO    
f=fopen("debug","a");
fprintf(f,"obj[0]= %f \n",obj[0]);
fprintf(f,"obj[1]= %f \n",obj[1]);
fprintf(f,"obj[2]= %f \n",obj[2]);
fprintf(f,"obj[3]= %f \n",obj[3]);
fprintf(f,"obj[4]= %f \n",obj[4]);
            for (j=0; j<NUM_JOBS; j++)
            {
                fprintf(f,"%d\t%d\t%f\t%f\t%f \n",job_id[j],xreal[j], start_time[j], job_time[j], time_limit[j]);
            }
            for (j=0; j<NUM_RESOURCES; j++)
            {
                fprintf(f,"%d --- %f \n",resource_id[j], r_free_time[j]);
            }            
fclose(f);
#endif



	
	starttime.clear();
	endtime.clear();
	free(time_consumed);

	free(temp_resource_free_time);
	

	free(t_predecessor_job_id);
	free(t_xreal);
	free(t_job_id);
	free(t_job_time);
	free(t_time_limit);
	free(t_job_cost);	

    return;
}

#endif
