/* NSGA-II routine (implementation of the 'main' function) */

# include <stdio.h>
# include <stdlib.h>
# include <math.h>
# include <unistd.h>
# include <string.h>
# include <sys/time.h>
# include <float.h>
# include <map>
# include <queue>
# include "global.h"
# include "rand.h"
# include "mtrandgen.h"
/* #define MORE_INFO  1*/

extern std::queue<gridlet> qu[100];
extern std::map<int,gridlet> job_bag;

int nsga2r (double);

double module_running_time = 0;
int count;
int nreal;		
int NUM_JOBS;
int NUM_RESOURCES;
int COMPUTING_RESOURCES;
int STORAGE_RESOURCES;
double prev_makespan_time;
int *resource_id;
double *resource_cost;
int *resource_type;
double *normalized_power;
double *normalized_frequency_coeff;
double *resource_free_time;
double **temp_free_time;
double *split_time;		
struct timeval prog_start;


double *max_of_obj;
double *min_of_obj;

int *job_id;
double *job_time;
double *job_time_start;
double *job_time_end;
double *time_limit;
double *job_cost;
int *predecessor_job_id;
int *job_type;

int nbin;
int nobj;
int ncon;
int popsize;
double pcross_real;
double pcross_bin;
double pmut_real;
double pmut_bin;
double eta_c;
double eta_m;
int ngen;
int nbinmut;
int nrealmut;
int nbincross;
int nrealcross;
int *nbits;
int *min_realvar;				/* changed from double to int */
int  *max_realvar;				/* changed from double to int */
double *min_binvar;
double *max_binvar;
int bitlength;
int choice;
int obj1;
int obj2;
int obj3;
int angle1;
int angle2;

double chosen_split_time;


individual best;

int choose_split_time(double **free_time){
	double sq_sum=0, min_sq_sum=999999999999999999;
	int i,j,index=0;
	double mean[5];
	for(i=0;i<5;i++){
		for(j=0;j<NUM_RESOURCES;j++){
			mean[i]+= free_time[j][i];
		}
		mean[i]= mean[i]/NUM_RESOURCES;
	}

	for(i=0;i<5;i++){
		sq_sum=0;
		for(j=0;j<NUM_RESOURCES;j++){
		sq_sum+= (mean[i] - free_time[j][i]) * (mean[i] - free_time[j][i]);
		}
		if(min_sq_sum > sq_sum){
			min_sq_sum=sq_sum;
			index=i;
		}
	}
//	printf("\n----min_sq_sum = %lf -- index= %d ",min_sq_sum,index);
	return index;
}


void initialize_max_min_obj(){
int i;
	for (i=0;i<nobj;i++){
		min_of_obj[i]= DBL_MAX;
		max_of_obj[i]=	DBL_MIN;
	}
}
		
		
int modjsg (int ch, int bl_size, int pp, int gn, int s){

char c, buf[20];
int i,j,k,block_size,flag=0,init_seed;
double temp;

int start_flag=0;
int res_type_flag=0, index;
int unfinished_jobs=0, u_index=0;

std::map <int,int> resource_pool;
std::map <int,int>::iterator it_rp;


int u_job_id[1000], u_predecessor_job_id[1000], u_job_type[1000];
double u_job_time[1000], u_job_time_start[1000],u_job_time_end[1000],u_time_limit[1000],u_job_cost[1000];

gettimeofday(&prog_start, NULL);
/*
    if (argc< 5)
    {
        printf("\n Usage ./nsga2r GNUPLOT<1:YES 0: NO>  job_block_size   pop_size  number_of_generation seed<any unsigned integer>\n");
        exit(1);
	}
*/
	choice = ch;
    block_size = bl_size;
    popsize = pp;

    ngen = gn;
    init_seed = s;
	init_genrand(init_seed);
	seed=genrand_real3();
    nobj = 5;
    ncon = 0;
/*            modified for Job scheduling    start */

    FILE *job_file, *resource_file,*fp, *fp_debug, **outfile;



    fp_debug = fopen("debug","w");    
    fprintf(fp_debug,"# This file contains data for debugging \n");    
    fflush(fp_debug);
    fclose(fp_debug);

	randomize();
	
	max_of_obj=  (double *)malloc( nobj * sizeof(double));     /* constant initialization */    
	min_of_obj=  (double *)malloc( nobj * sizeof(double));     /* constant initialization */    
	
    resource_file = fopen("initial_resource.in","r");
    resource_id = (int *)malloc( 50 * sizeof(int));     /* constant initialization */
    resource_cost = (double *)malloc( 50 * sizeof(double));     /* constant initialization */    
	resource_type = (int *)malloc(50 * sizeof(int));    
	normalized_power = (double *)malloc(50 * sizeof(double));
	normalized_frequency_coeff = (double *)malloc(50 * sizeof(double));
	resource_free_time = (double *)malloc(50 * sizeof(double));
	split_time = (double *)malloc((5)* sizeof(double ));
	
	temp_free_time = (double **)malloc((50)* sizeof(double *));
	if(temp_free_time == NULL)
	{
			fprintf(stderr, "out of memory\n");
			printf("exited, can't allocate memory to diff in test_problem");
			exit(1);
	}
	for(i = 0; i < 50; i++)
	{
		temp_free_time[i] = (double *)malloc(6 * sizeof(double));
		if(temp_free_time[i] == NULL)
			{
				fprintf(stderr, "out of memory\n");
				printf("exited, can't allocate memory to diff in test_problem");
				exit(1);
			}
	}

    i=0;
    res_type_flag=0;
    while((c=fgetc(resource_file))!=EOF){
    	fscanf(resource_file,"%d",&resource_id[i]);
		
		resource_pool[resource_id[i]] = 1;
		
		fscanf(resource_file,"%lf",&resource_cost[i]);

		fscanf(resource_file,"%d",&resource_type[i]);		
		if(resource_type[i]==1 && res_type_flag==0){
			res_type_flag=1;
			COMPUTING_RESOURCES=i;
		}

		fscanf(resource_file,"%lf",&normalized_power[i]);
		fscanf(resource_file,"%lf",&normalized_frequency_coeff[i]);
		for(j=0;j<5;j++){
				fscanf(resource_file,"%lf",&temp_free_time[i][j]);
				temp_free_time[i][j]=0;								// for initialization
		}

		resource_free_time[i]=0; 									// for initialization
		i++;
    }
    STORAGE_RESOURCES = i-COMPUTING_RESOURCES;
    NUM_RESOURCES = i-1;

	fclose(resource_file);
	
	outfile = (FILE **)malloc((NUM_RESOURCES)* sizeof(FILE *));
	for(i=0;i<NUM_RESOURCES;i++){
		sprintf(buf,"schedule_%d.out",resource_id[i]);
		outfile[i] =fopen(buf,"w");
		fclose(outfile[i]);
	}
	
	
	
	resource_file = fopen("resource.in","w");
		i=0;
		res_type_flag = 0;
		while(i<NUM_RESOURCES){
			fprintf(resource_file,"%d\t",resource_id[i]);

			fprintf(resource_file,"%lf\t",resource_cost[i]);

			fprintf(resource_file,"%d\t",resource_type[i]);		

			if(resource_type[i]==1 && res_type_flag==0){
				res_type_flag=1;
				COMPUTING_RESOURCES=i;
			}
		
			fprintf(resource_file,"%lf\t",normalized_power[i]);
			fprintf(resource_file,"%lf\t",normalized_frequency_coeff[i]);
			for(j=0;j<5;j++)
					fprintf(resource_file,"%lf\t",temp_free_time[i][j]);	
			fprintf(resource_file,"\n");
			i++;
		}        
	    STORAGE_RESOURCES = i-COMPUTING_RESOURCES;
    	NUM_RESOURCES = i-1;		
		fclose(resource_file);	
	
    job_file = fopen("job.in","r");
    job_id = (int *)malloc((block_size + 50)*sizeof(int));
    job_time = (double *)malloc((block_size + 50 )*sizeof(double));
    job_time_start = (double *)malloc((block_size + 50 )*sizeof(double));
    job_time_end= (double *)malloc((block_size + 50 )*sizeof(double));
    time_limit= (double *)malloc((block_size + 50 )*sizeof(double));
    job_cost = (double *)malloc((block_size + 50 )*sizeof(double));
	predecessor_job_id = (int *)malloc((block_size + 50)*sizeof(int));
	job_type = (int *)malloc((block_size + 50)*sizeof(int));	
    i=0;
    
    while(1){

/* read current state of resources */    
	
	merge_resource_file();

		for(it_rp=resource_pool.begin();it_rp!=resource_pool.end();++it_rp){
			(it_rp->second)++;
		}

        resource_file = fopen("resource.in","r");
		i=0;
		res_type_flag=0;		
		while((c=fgetc(resource_file))!=EOF){
			fseek(resource_file,-1,SEEK_CUR);
			fscanf(resource_file,"%d\t",&resource_id[i]);
			
			if (resource_pool.find(resource_id[i]) != resource_pool.end()){			// resource found
					resource_pool[resource_id[i]] = 0;
			}
			else {
					resource_pool[resource_id[i]] = 0;
					sprintf(buf,"schedule_%d.out",resource_id[i]);
					fp =fopen(buf,"w");
					fclose(fp);
			}

			fscanf(resource_file,"%lf\t",&resource_cost[i]);

			fscanf(resource_file,"%d\t",&resource_type[i]);
		
			if(resource_type[i]==1 && res_type_flag==0){
				res_type_flag=1;
				COMPUTING_RESOURCES=i;
			}

			fscanf(resource_file,"%lf\t",&normalized_power[i]);
			fscanf(resource_file,"%lf\t",&normalized_frequency_coeff[i]);
			for(j=0;j<5;j++)
					fscanf(resource_file,"%lf\t",&temp_free_time[i][j]);
			i++;
		}
//		STORAGE_RESOURCES = i-COMPUTING_RESOURCES;
		STORAGE_RESOURCES = i-COMPUTING_RESOURCES+1;
//		NUM_RESOURCES = i-1;
		NUM_RESOURCES = i;		
		

		fclose(resource_file);
	        
/*    */
    i=0;
	j=0;
	if(start_flag){
	
		index = choose_split_time(temp_free_time);
	    chosen_split_time = split_time[index];   /* for now a dummy which chooses from 0 to 4 */	
	    for(k=0;k<NUM_RESOURCES;k++){
	    resource_free_time[k]= temp_free_time[k][index] ;
	    }

	    printf("helllo i am inside ------- split_time[i] = %lf \n ", chosen_split_time);
	    
	    remove_rescheduled_jobs(chosen_split_time,NUM_RESOURCES);
/* for demo part start 	    
	    if(randomperc()>0.5){
		    read_scheduled_jobs();
	    }
  for demo part  end*/



	while(best.job_time_start[j] < chosen_split_time && j < nreal){							// send to queue
	
	gridlet g;
	g.job_id = best.job_id[j];
	g.res_id = resource_id[best.xreal[j]];;
	g.start = best.job_time_start[j];
	g.exec_time = (best.job_time[j] / normalized_frequency_coeff[best.xreal[j]]);
	g.t_limit = best.job_limit[j];
	g.cost = best.job_cost[j];
	g.p_id = best.predecessor_job_id[j];
	g.j_type = best.job_type[j];
	g.status =0;
	
	qu[g.res_id].push(g);
	job_bag[g.job_id]=g;
//	gridlet test;
//	test = qu[g.res_id].back();
//	printf("%d\t%d\t%lf\t%lf\t%lf\t%lf\t%d\t%d\n",test.job_id,test.res_id,test.start,test.exec_time,test.t_limit,test.cost, test.p_id,test.j_type);
	
	
	j++;
	}
	
	while(j<nreal)																			// again run this jobs for scheduling
           {	
           		job_id[i] = best.job_id[j];
				job_time[i] = best.job_time[j]; 

				time_limit[i] = best.job_limit[j];  
				  
				job_cost[i] = best.job_cost[j];   
			
				predecessor_job_id[i] = best.predecessor_job_id[j];
				
				job_type[i] = best.job_type[j];
				

				
				i++;
				j++;
            }
        unfinished_jobs = 0;
		for(it_rp=resource_pool.begin();it_rp!=resource_pool.end();++it_rp){			
	
				if (it_rp->second !=0){												// check if the resource has stopped ? then reschedule their jobs by taking the jobs from queue
				
//					unfinished_jobs = read_unfinished_jobs(it_rp->first,u_job_id,u_job_time,u_time_limit, u_job_cost, u_predecessor_job_id, u_job_type,unfinished_jobs);
					resource_pool.erase(it_rp->first);
					unfinished_jobs += qu[it_rp->first].size();
					int tmp =it_rp->first;
					while (!qu[tmp].empty()){
								gridlet g = qu[tmp].front();
						    	job_id[i] = g.job_id ;    

								job_time[i] = g.exec_time; 

								time_limit[i] = g.t_limit ;  
						  
								job_cost[i]=g.cost;   
			
								predecessor_job_id[i]= g.p_id;   
			
								job_type[i] = g.j_type;

								qu[tmp].pop();
								i++;
								if (i==block_size){
									NUM_JOBS=i;
									i=0;
									temp=genrand_real3();
//									printf("unfinished jobs %lf",temp);   		
									nsga2r(temp);
								}
					}
				}
		}            

    }
    
/*  for unfinished jobs  
	u_index=0;
    while(unfinished_jobs > u_index){
    	job_id[i] = u_job_id[u_index] ;    

		job_time[i] = u_job_time[u_index]; 

		time_limit[i] = u_time_limit[u_index] ;  
  
    	job_cost[i]=u_job_cost[u_index];   
    	
    	predecessor_job_id[i]= u_predecessor_job_id[u_index];   
    	
    	job_type[i] = u_job_type[u_index];

		u_index++;    	
		i++;
		if (i==block_size){
			NUM_JOBS=i;
			i=0;
    		temp=genrand_real3();
			printf("unfinished jobs %lf",temp);   		
			nsga2r(temp);
		}
    }    
 */

    while(i < block_size){
    	fscanf(job_file,"%d",&job_id[i]);    
    	if(job_id[i]==-1){
    		flag=1;
    		break;
    		}

    	fscanf(job_file,"%lf",&job_time[i]); 

    	fscanf(job_file,"%lf",&time_limit[i]);  
    	  
    	fscanf(job_file,"%lf",&job_cost[i]);   
    	
    	fscanf(job_file,"%d",&predecessor_job_id[i]);   
    	
    	fscanf(job_file,"%d",&job_type[i]);
    	
		i++;
    }
    
	NUM_JOBS = i;    

	start_flag=1;
     choice = ch;
    if(flag==1){
    	if(NUM_JOBS>0){
    		temp=genrand_real3();
			printf("%lf",temp);    		
	    	nsga2r(temp);
	    }
    	break;
    }
	

	temp=genrand_real3();
	printf("%lf",temp);
	nsga2r(temp);

//	sleep(1);
	}
	fclose(job_file);
	
    
/*            modified for Job scheduling    end */

//	for(i = 0; i < 50; i++)
//			free(temp_free_time[i]);
//	free(temp_free_time);	
    free(job_id);
    free(job_time);
    free(job_time_start);
    free(job_time_end);
    free(time_limit);
    free(job_cost);
	free(predecessor_job_id);
	free(job_type);	
	free(outfile);
	free(max_of_obj);
	free(min_of_obj);
}













int nsga2r (double seed)
{
    int i,j;
    FILE *fpt1;
    FILE *fpt2;
    FILE *fpt3;
    FILE *fpt4;
    FILE *fpt5;
    FILE *fpt6;    
    FILE *resource_file;
    FILE *gp;
    FILE *f;				/* modified  for debug  file */
    population *parent_pop;
    population *child_pop;
    population *mixed_pop;

    if (seed<=0.0 || seed>=1.0)
    {
        printf("\n Entered seed value is wrong, seed value must be in (0,1) \n");
        exit(1);
    }

#ifdef MORE_INFO    

    f= fopen("debug","w");
    fprintf(f,"# This file contains the population distribution and its objective values\n");
    fclose(f);
    
    fpt1 = fopen("initial_pop.out","w");
    fpt2 = fopen("final_pop.out","w");    
#endif    

    
#ifdef MORE_INFO        
    fpt3 = fopen("best_pop.out","w");
    fpt4 = fopen("all_pop.out","w");

    
    fpt5 = fopen("params.out","w");
    fpt6 = fopen("schedule.out","a");   
    
#endif
    
 

#ifdef MORE_INFO            
    fprintf(fpt1,"# This file contains the data of initial population\n");
    fprintf(fpt2,"# This file contains the data of final population\n");
    

    fprintf(fpt3,"# This file contains the data of final feasible population (if found)\n");
    fprintf(fpt4,"# This file contains the data of all generations\n");

    
    fprintf(fpt5,"# This file contains information about inputs as read by the program\n");
#endif
    

    if (popsize<4 || (popsize%4)!= 0)
    {
        printf("\n population size read is : %d",popsize);
        printf("\n Wrong population size entered, hence exiting \n");
        exit (1);
    }
    if (ngen<1)
    {
        printf("\n number of generations read is : %d",ngen);
        printf("\n Wrong nuber of generations entered, hence exiting \n");
        exit (1);
    }
    if (nobj<1)
    {
        printf("\n number of objectives entered is : %d",nobj);
        printf("\n Wrong number of objectives entered, hence exiting \n");
        exit (1);
    }
    if (ncon<0)
    {
        printf("\n number of constraints entered is : %d",ncon);
        printf("\n Wrong number of constraints enetered, hence exiting \n");
        exit (1);
    }
    nreal= NUM_JOBS;															/* modified */
    if (nreal<0)
    {
        printf("\n number of real variables entered is : %d",nreal);
        printf("\n Wrong number of variables entered, hence exiting \n");
        exit (1);
    }
	if (NUM_RESOURCES<1){
			printf("Number of Resources entered is : %d", NUM_RESOURCES);
			printf("Wrong number of resources entered, hence exiting");
			exit(1);
	}




    if (nreal != 0)
    {
        min_realvar = (int *)malloc(nreal*sizeof(int));
        max_realvar = (int *)malloc(nreal*sizeof(int));
        for (i=0; i<nreal; i++)
        {
			if(job_type[i]==0)
				min_realvar[i]=0;
			else 
				min_realvar[i]=COMPUTING_RESOURCES;
			if(job_type[i]==0)
				max_realvar[i]= COMPUTING_RESOURCES;
			else 
				max_realvar[i]=NUM_RESOURCES;
								
            if (max_realvar[i] <= min_realvar[i])
            {
                printf("\n Wrong limits entered for the min and max bounds of real variable, hence exiting \n");
                exit(1);
            }
        }
        

        
		pcross_real = genrand_range(0.8, 1.0);
        if (pcross_real<0.6 || pcross_real>1.0)
        {
            printf("\n Probability of crossover entered is : %e",pcross_real);
            printf("\n Entered value of probability of crossover of real variables is out of bounds, hence exiting \n");
            exit (1);
        }
        pmut_real = genrand_range(0.1, 0.5);
        if (pmut_real<0.0 || pmut_real>1.0)
        {
            printf("\n Probability of mutation entered is : %e",pmut_real);
            printf("\n Entered value of probability of mutation of real variables is out of bounds, hence exiting \n");
            exit (1);
        }
        eta_c=rndint(5,20);
        if (eta_c<=0)
        {
            printf("\n The value entered is : %e",eta_c);
            printf("\n Wrong value of distribution index for crossover entered, hence exiting \n");
            exit (1);
        }
        eta_m=rndint(5,50);        
        if (eta_m<=0)
        {
            printf("\n The value entered is : %e",eta_m);
            printf("\n Wrong value of distribution index for mutation entered, hence exiting \n");
            exit (1);
        }
    }
    nbin=0;
    if (nbin<0)
    {
        printf ("\n number of binary variables entered is : %d",nbin);
        printf ("\n Wrong number of binary variables entered, hence exiting \n");
        exit(1);
    }
    if (nbin != 0)
    {
        nbits = (int *)malloc(nbin*sizeof(int));
        min_binvar = (double *)malloc(nbin*sizeof(double));
        max_binvar = (double *)malloc(nbin*sizeof(double));
        for (i=0; i<nbin; i++)
        {
            printf ("\n Enter the number of bits for binary variable %d : ",i+1);
            scanf ("%d",&nbits[i]);
            if (nbits[i] < 1)
            {
                printf("\n Wrong number of bits for binary variable entered, hence exiting");
                exit(1);
            }
            printf ("\n Enter the lower limit of binary variable %d : ",i+1);
            scanf ("%lf",&min_binvar[i]);
            printf ("\n Enter the upper limit of binary variable %d : ",i+1);
            scanf ("%lf",&max_binvar[i]);
            if (max_binvar[i] <= min_binvar[i])
            {
                printf("\n Wrong limits entered for the min and max bounds of binary variable entered, hence exiting \n");
                exit(1);
            }
        }
        printf ("\n Enter the probability of crossover of binary variable (0.6-1.0): ");
        scanf ("%lf",&pcross_bin);
        if (pcross_bin<0.0 || pcross_bin>1.0)
        {
            printf("\n Probability of crossover entered is : %e",pcross_bin);
            printf("\n Entered value of probability of crossover of binary variables is out of bounds, hence exiting \n");
            exit (1);
        }
        printf ("\n Enter the probability of mutation of binary variables (1/nbits): ");
        scanf ("%lf",&pmut_bin);
        if (pmut_bin<0.0 || pmut_bin>1.0)
        {
            printf("\n Probability of mutation entered is : %e",pmut_bin);
            printf("\n Entered value of probability  of mutation of binary variables is out of bounds, hence exiting \n");
            exit (1);
        }
    }
    if (nreal==0 && nbin==0)
    {
        printf("\n Number of real as well as binary variables, both are zero, hence exiting \n");
        exit(1);
    }
    

    if (choice!=0 && choice!=1)
    {
        printf("\n Entered the wrong choice, hence exiting, choice entered was %d\n",choice);
        exit(1);
    }
    if (choice==1)
    {
        gp = popen(GNUPLOT_COMMAND,"w");
        if (gp==NULL)
        {
            printf("\n Could not open a pipe to gnuplot, check the definition of GNUPLOT_COMMAND in file global.h\n");
            printf("\n Edit the string to suit your system configuration and rerun the program\n");
            exit(1);
        }
        if (nobj==2)
        {
            printf("\n Enter the objective for X axis display : ");
			obj1=1;
            if (obj1<1 || obj1>nobj)
            {
                printf("\n Wrong value of X objective entered, value entered was %d\n",obj1);
                exit(1);
            }
            printf("\n Enter the objective for Y axis display : ");
			obj2=2;            
            if (obj2<1 || obj2>nobj)
            {
                printf("\n Wrong value of Y objective entered, value entered was %d\n",obj2);
                exit(1);
            }
            obj3 = -1;
        }
        else
        {
            printf("\n #obj > 2, 2D display or a 3D display ?, enter 2 for 2D and 3 for 3D :");
            choice=3;
            if (choice!=2 && choice!=3)
            {
                printf("\n Entered the wrong choice, hence exiting, choice entered was %d\n",choice);
                exit(1);
            }
            if (choice==2)
            {
                printf("\n Enter the objective for X axis display : ");
                scanf("%d",&obj1);
                if (obj1<1 || obj1>nobj)
                {
                    printf("\n Wrong value of X objective entered, value entered was %d\n",obj1);
                    exit(1);
                }
                printf("\n Enter the objective for Y axis display : ");
                scanf("%d",&obj2);
                if (obj2<1 || obj2>nobj)
                {
                    printf("\n Wrong value of Y objective entered, value entered was %d\n",obj2);
                    exit(1);
                }
                obj3 = -1;
            }
            else
            {
				obj1=1;
                if (obj1<1 || obj1>nobj)
                {
                    printf("\n Wrong value of X objective entered, value entered was %d\n",obj1);
                    exit(1);
                }
   				obj2=2;
                if (obj2<1 || obj2>nobj)
                {
                    printf("\n Wrong value of Y objective entered, value entered was %d\n",obj2);
                    exit(1);
                }
				obj3=3;
                if (obj3<1 || obj3>nobj)
                {
                    printf("\n Wrong value of Z objective entered, value entered was %d\n",obj3);
                    exit(1);
                }
                angle1=60;
                if (angle1<0 || angle1>180)
                {
                    printf("\n Wrong value for first angle entered, hence exiting \n");
                    exit(1);
                }
                angle2=30;
                if (angle2<0 || angle2>360)
                {
                    printf("\n Wrong value for second angle entered, hence exiting \n");
                    exit(1);
                }
            }
        }
    }
#ifdef MORE_INFO 
    printf("\n Input data successfully entered, now performing initialization \n");
    fprintf(fpt5,"\n Population size = %d",popsize);
    fprintf(fpt5,"\n Number of generations = %d",ngen);
    fprintf(fpt5,"\n Number of objective functions = %d",nobj);
    fprintf(fpt5,"\n Number of constraints = %d",ncon);
    fprintf(fpt5,"\n Number of real variables = %d",nreal);
    
    if (nreal!=0)
    {
        for (i=0; i<nreal; i++)
        {
            fprintf(fpt5,"\n Lower limit of real variable %d = %d",i+1,min_realvar[i]);
            fprintf(fpt5,"\n Upper limit of real variable %d = %d",i+1,max_realvar[i]);
        }
        fprintf(fpt5,"\n Probability of crossover of real variable = %e",pcross_real);
        fprintf(fpt5,"\n Probability of mutation of real variable = %e",pmut_real);
        fprintf(fpt5,"\n Distribution index for crossover = %e",eta_c);
        fprintf(fpt5,"\n Distribution index for mutation = %e",eta_m);
    }
    fprintf(fpt5,"\n Number of binary variables = %d",nbin);
    if (nbin!=0)
    {
        for (i=0; i<nbin; i++)
        {
            fprintf(fpt5,"\n Number of bits for binary variable %d = %d",i+1,nbits[i]);
            fprintf(fpt5,"\n Lower limit of binary variable %d = %e",i+1,min_binvar[i]);
            fprintf(fpt5,"\n Upper limit of binary variable %d = %e",i+1,max_binvar[i]);
        }
        fprintf(fpt5,"\n Probability of crossover of binary variable = %e",pcross_bin);
        fprintf(fpt5,"\n Probability of mutation of binary variable = %e",pmut_bin);
    }
    fprintf(fpt5,"\n Seed for random number generator = %e",seed);
    bitlength = 0;
    if (nbin!=0)
    {
        for (i=0; i<nbin; i++)
        {
            bitlength += nbits[i];
        }
    }
    fprintf(fpt1,"# of objectives = %d, # of constraints = %d, # of real_var = %d, # of bits of bin_var = %d, constr_violation, rank, crowding_distance\n",nobj,ncon,nreal,bitlength);
    fprintf(fpt2,"# of objectives = %d, # of constraints = %d, # of real_var = %d, # of bits of bin_var = %d, constr_violation, rank, crowding_distance\n",nobj,ncon,nreal,bitlength);

    fprintf(fpt3,"# of objectives = %d, # of constraints = %d, # of real_var = %d, # of bits of bin_var = %d, constr_violation, rank, crowding_distance\n",nobj,ncon,nreal,bitlength);
    fprintf(fpt4,"# of objectives = %d, # of constraints = %d, # of real_var = %d, # of bits of bin_var = %d, constr_violation, rank, crowding_distance\n",nobj,ncon,nreal,bitlength);
#endif
    
    nbinmut = 0;
    nrealmut = 0;
    nbincross = 0;
    nrealcross = 0;
    parent_pop = (population *)malloc(sizeof(population));
    child_pop = (population *)malloc(sizeof(population));
    mixed_pop = (population *)malloc(sizeof(population));
    allocate_memory_pop (parent_pop, popsize);
    allocate_memory_pop (child_pop, popsize);
    allocate_memory_pop (mixed_pop, 2*popsize);
    randomize();
	initialize_max_min_obj();    
    initialize_pop (parent_pop);
    printf("\n Initialization done, now performing first generation");
    decode_pop(parent_pop);
    evaluate_pop (parent_pop);
    assign_rank_and_crowding_distance (parent_pop);
    
#ifdef MORE_INFO            
    report_pop (parent_pop, fpt1);
    fprintf(fpt4,"# gen = 1\n");
    report_pop(parent_pop,fpt4);
#endif    
    
    printf("\n gen = 1");
    fflush(stdout);
    if (choice!=0)    onthefly_display (parent_pop,gp,1);
    
#ifdef MORE_INFO                
    fflush(fpt1);
    fflush(fpt2);
    fflush(fpt3);
    fflush(fpt4);
    fflush(fpt5);
#endif        
//    sleep(1);
    for (i=2; i<=ngen; i++)
    {
		initialize_max_min_obj();
        selection (parent_pop, child_pop);
        mutation_pop (child_pop);
        decode_pop(child_pop);
        evaluate_pop(child_pop);
        merge (parent_pop, child_pop, mixed_pop);
        fill_nondominated_sort (mixed_pop, parent_pop);
        /* Comment following four lines if information for all
        generations is not desired, it will speed up the execution */
        
#ifdef MORE_INFO            
        fprintf(fpt4,"# gen = %d\n",i);
        report_pop(parent_pop,fpt4);
        fflush(fpt4);
#endif
        
        if (choice!=0)    onthefly_display (parent_pop,gp,i);
        printf("\n gen = %d",i);
    }
#ifdef MORE_INFO            
    report_pop(parent_pop,fpt2);
    printf("\n Generations finished, now reporting solutions");
    report_feasible(parent_pop,fpt3);
#endif

	best = report_best(parent_pop, fpt6);

    
/* update new resource_free_time  in resource file start */

	for(j=0;j<NUM_RESOURCES;j++){
		   	resource_free_time[j] = best.resource_free_time[j];
	}
	prev_makespan_time = best.obj[0];
	
//	printf("prev makespan --------------- %lf",prev_makespan_time);
	
		resource_file = fopen("resource.in","w");
		i=0;
		
		while(i<NUM_RESOURCES){
			fprintf(resource_file,"%d\t",resource_id[i]);
			fprintf(resource_file,"%lf\t",resource_cost[i]);
			fprintf(resource_file,"%d\t",resource_type[i]);		
			fprintf(resource_file,"%lf\t",normalized_power[i]);
			fprintf(resource_file,"%lf\t",normalized_frequency_coeff[i]);			
			for(j=0;j<5;j++)
					fprintf(resource_file,"%lf\t",temp_free_time[i][j]);	
//			fprintf(resource_file,"%lf\n",resource_free_time[i]);
			fprintf(resource_file,"\n");
			i++;
		}        

		fclose(resource_file);

/* update new resource_free_time  in resource file end */



	
#ifdef MORE_INFO                
    if (nreal!=0)
    {
        fprintf(fpt5,"\n Number of crossover of real variable = %d",nrealcross);
        fprintf(fpt5,"\n Number of mutation of real variable = %d",nrealmut);
    }
    if (nbin!=0)
    {
        fprintf(fpt5,"\n Number of crossover of binary variable = %d",nbincross);
        fprintf(fpt5,"\n Number of mutation of binary variable = %d",nbinmut);
    }
    fflush(stdout);
    fflush(fpt1);
    fflush(fpt2);
    

    fflush(fpt3);
    fflush(fpt4);

    
    fflush(fpt5);
    fflush(fpt6);    
    fclose(fpt1);
    fclose(fpt2);

    fclose(fpt3);
    fclose(fpt4);


    fclose(fpt5);
    fclose(fpt6);
#endif            
    if (choice!=0)
    {
        pclose(gp);
    }
    if (nreal!=0)
    {
        free (min_realvar);
        free (max_realvar);
    }
    if (nbin!=0)
    {
        free (min_binvar);
        free (max_binvar);
        free (nbits);
    }
    deallocate_memory_pop (parent_pop, popsize);
    deallocate_memory_pop (child_pop, popsize);
    deallocate_memory_pop (mixed_pop, 2*popsize);
    free (parent_pop);
    free (child_pop);
    free (mixed_pop);
    printf("\n Routine successfully exited \n");
    return (0);
}
