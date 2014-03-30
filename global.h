/* This file contains the variable and function declarations */

# ifndef _GLOBAL_H_
# define _GLOBAL_H_

# define INF 1.0e14
# define EPS 1.0e-14
# define E  2.71828182845905
# define PI 3.14159265358979
# define GNUPLOT_COMMAND "gnuplot -persist"



extern int count;
extern struct timeval prog_start;
//extern queue<gridlet> qu[100];

typedef struct
{
    int rank;
    double constr_violation;
    int *xreal;    /* modified */
    int **gene;
    double *xbin;
    double *obj;
    double *constr;
    double crowd_dist;
    double *job_time_start; /* modified */
    double *job_cost; /* modified */ 
    double *job_time;    /* modified */ 
    int *job_id; /* modified */
    int *predecessor_job_id; /* modified */    
    int *job_type; /* modified */   
    double *resource_free_time; /* modified */
    double *job_limit;
}
individual;

typedef struct {
	int job_id;
	int res_id;
	double start;
	double exec_time;
	double t_limit;
	double cost;
	int p_id;
	int j_type;
	int status;
}gridlet;

typedef struct
{
    individual *ind;
}
population;

typedef struct lists
{
    int index;
    struct lists *parent;
    struct lists *child;
}
list;

extern int *resource_id;
extern double *job_time;
extern int *predecessor_job_id;
extern int *job_type;

extern double module_running_time;
extern double prev_makespan_time;
extern double *resource_free_time;
extern double **temp_free_time;
extern double *split_time;
extern double chosen_split_time;

extern double *max_of_obj;
extern double *min_of_obj;


extern int NUM_JOBS;
extern int NUM_RESOURCES;
extern int COMPUTING_RESOURCES;
extern int STORAGE_RESOURCES;
extern int nreal;
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
extern int *min_realvar;					/* changed from double to int */
extern int *max_realvar;					/* changed from double to int */
extern double *min_binvar;
extern double *max_binvar;
extern int bitlength;
extern int choice;
extern int obj1;
extern int obj2;
extern int obj3;
extern int angle1;
extern int angle2;

extern void remove_rescheduled_jobs(double val,int NUM_RESOURCES);
extern void read_scheduled_jobs();
extern int read_unfinished_jobs(int file_id,int u_job_id[],double u_job_time[],double u_time_limit[],double  u_job_cost[],int  u_predecessor_job_id[],int  u_job_type[],int unfinished_jobs);

extern void merge_resource_file();

void allocate_memory_pop (population *pop, int size);
void allocate_memory_ind (individual *ind);
void deallocate_memory_pop (population *pop, int size);
void deallocate_memory_ind (individual *ind);

double maximum (double a, double b);
double minimum (double a, double b);

void crossover (individual *parent1, individual *parent2, individual *child1, individual *child2);
void realcross (individual *parent1, individual *parent2, individual *child1, individual *child2);
void k_cross (individual *parent1, individual *parent2, individual *child1, individual *child2);
void fitness_cross (individual *parent1, individual *parent2, individual *child1, individual *child2);
void bincross (individual *parent1, individual *parent2, individual *child1, individual *child2);

void assign_crowding_distance_list (population *pop, list *lst, int front_size);
void assign_crowding_distance_indices (population *pop, int c1, int c2);
void assign_crowding_distance (population *pop, int *dist, int **obj_array, int front_size);

void decode_pop (population *pop);
void decode_ind (individual *ind);

void onthefly_display (population *pop, FILE *gp, int ii);

int check_dominance (individual *a, individual *b);

void evaluate_pop (population *pop);
void evaluate_ind (individual *ind);

void fill_nondominated_sort (population *mixed_pop, population *new_pop);
void crowding_fill (population *mixed_pop, population *new_pop, int count, int front_size, list *cur);

void initialize_pop (population *pop);
void initialize_ind (individual *ind);

void insert (list *node, int x);
list* del (list *node);

void merge(population *pop1, population *pop2, population *pop3);
void copy_ind (individual *ind1, individual *ind2);

void mutation_pop (population *pop);
void mutation_ind (individual *ind);
void bin_mutate_ind (individual *ind);
void real_mutate_ind (individual *ind);

void test_problem (int *xreal, double *xbin, int **gene, double *obj, double *constr, double *start_time, double *r_free_time, double *j_time, double *j_cost, int *p_job_id, double *j_limit, int *j_id, int *j_type);  

void assign_rank_and_crowding_distance (population *new_pop);

void report_pop (population *pop, FILE *fpt);
void report_feasible (population *pop, FILE *fpt);
void report_ind (individual *ind, FILE *fpt);
individual report_best (population *pop, FILE *fpt);
void print_best(individual ind, FILE *fpt);


void quicksort_front_obj(population *pop, int objcount, int obj_array[], int obj_array_size);
void q_sort_front_obj(population *pop, int objcount, int obj_array[], int left, int right);
void quicksort_dist(population *pop, int *dist, int front_size);
void q_sort_dist(population *pop, int *dist, int left, int right);

void selection (population *old_pop, population *new_pop);
individual* tournament (individual *ind1, individual *ind2);

# endif
