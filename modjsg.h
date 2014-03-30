# ifndef _MODJSG_H_
# define _MODJSG_H_


int nsga2r (double);

extern double module_running_time;
extern int count;
extern int nreal;		
extern int NUM_JOBS;
extern int NUM_RESOURCES;
extern int COMPUTING_RESOURCES;
extern int STORAGE_RESOURCES;
extern double prev_makespan_time;
extern int *resource_id;
extern double *resource_cost;
extern int *resource_type;
extern double *normalized_power;
extern double *normalized_frequency_coeff;
extern double *resource_free_time;
extern double **temp_free_time;
extern double *split_time;		

extern double *max_of_obj;
extern double *min_of_obj;

extern int *job_id;
extern double *job_time;
extern double *job_time_start;
extern double *job_time_end;
extern double *time_limit;
extern double *job_cost;
extern int *predecessor_job_id;
extern int *job_type;

extern int nbin;
extern int nobj;
extern int ncon;
extern int popsize;
extern double pcross_real;
extern double pcross_bin;
extern double pmut_real;
extern double pmut_bin;
extern double eta_c;
extern double eta_m;
extern int ngen;
extern int nbinmut;
extern int nrealmut;
extern int nbincross;
extern int nrealcross;
extern int *nbits;
extern int *min_realvar;				/* changed from extern double to extern int */
extern int  *max_realvar;				/* changed from extern double to extern int */
extern double *min_binvar;
extern double *max_binvar;
extern int bitlength;
extern int choice;
extern int obj1;
extern int obj2;
extern int obj3;
extern int angle1;
extern int angle2;

extern double chosen_split_time;



int choose_split_time(double **free_time);

int modjsg (int ch, int bl_size, int pp, int gn, int s);

# endif