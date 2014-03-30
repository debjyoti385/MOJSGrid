#include <stdio.h>
# include <string.h>
#include <sys/time.h>
# include <stdlib.h>
# include <math.h>
# include <unistd.h>
#include "modjsg.h"
#include <mpi.h>
#include <map>
#include <pthread.h>
#include <queue>
#include "global.h"

struct thread_data{
int t_id;
int choice;
int block_size;
int popsize;
int  ngen;
int  init_seed;
};


struct thread_data thread_data_array[2];


std::queue<gridlet> qu[100];
std::map<int,gridlet> job_bag;

int function(double val){

	struct timeval start, temp, now;
   long int counter = 0;   
   gettimeofday(&start, NULL);
   gettimeofday(&now, NULL);
   int val_sec = (int)val;
   long long val_usec = (val - val_sec) *1000000;
   while( (now.tv_sec - start.tv_sec) < val  || ( (now.tv_sec -start.tv_sec)==val && (now.tv_usec - start.tv_usec) < val_usec)){
	counter++;
	gettimeofday(&now, NULL);
   }  
   return 0;
}


void *  schedule(void* threadargs){

	struct thread_data *mydata;
	double run_time, time_wait;
	int choice, block_size,popsize, ngen, init_seed;
	mydata = (struct thread_data *) threadargs;
   	gridlet job;
	int threadid=(int)mydata->t_id;

	
	if(threadid==0){
	choice = (int)mydata->choice;
	block_size = (int)mydata->block_size;
	popsize = (int)mydata->popsize;
	ngen = (int)mydata->ngen;
	init_seed = (int)mydata->init_seed;
		
	modjsg(choice, block_size, popsize,ngen, init_seed);
	
	
	}
	else if(threadid==1){
	//Data sender
	struct timeval temp, now;
		while(1){
			for (int i=0; i<100;i++){
				if (qu[i].empty()){
					continue;
				}
				else {
						job=qu[i].front();

						temp.tv_usec = (int)((job.start - (int)job.start)*1000000) + prog_start.tv_usec;
					
							if (temp.tv_usec > 1000000){
								temp.tv_sec = (int)job.start + prog_start.tv_sec + 1; 
								temp.tv_usec = temp.tv_usec - 1000000;
							}
							else {
								temp.tv_sec = (int)job.start + prog_start.tv_sec ; 
							}

						gettimeofday(&now, NULL);
							if(now.tv_sec > temp.tv_sec || (now.tv_sec == temp.tv_sec && now.tv_usec >= temp.tv_usec))
							{
								//start job
								//printf("%ld %ld %d resource\n", now.tv_sec-prog_start.tv_sec, now.tv_usec-prog_start.tv_usec, i);
								run_time = job.exec_time;
                                        int recv_id=job.res_id;
                                        MPI_Request request;
                                        printf("sending  %lf to %d \n",run_time,recv_id);
								MPI_Send(&run_time, 1, MPI_DOUBLE, recv_id, recv_id, MPI_COMM_WORLD);
                                        //scanf("%lf",&run_time);
								qu[i].pop();
							}
				}
				
			}
			
			usleep(100);
			
//			MPI_Isend(&run_time, 1, MPI_DOUBLE, recv_id, 1, MPI_COMM_WORLD, &request);
		}
	}
	
	

}




std::queue<double> process_time;
int me;
void * receivedeb(void* thread_id){


	printf("hello me %d\n",me);	
	long threadid=( long)thread_id;

	printf("thread id %ld",threadid);
	MPI_Status status;

	if(threadid==0){
	       while(1){	
       	       	double run_time;
               printf("me %d waiting to recv\n\n",me);
	       MPI_Recv(&run_time, 1, MPI_DOUBLE, 0, me, MPI_COMM_WORLD, &status);
		
               printf("Processo %d Received %lf \n",me,run_time);


	       		process_time.push(run_time);
			//function(run_time);
	       }

	}
	else if(threadid==1){
		while(1){
			if(!process_time.empty()){
				double run_time=process_time.front();
				process_time.pop();
				function(run_time);

			}

		}


	
	}

}


int main(int argc, char *argv[])
{
	int i, recvid;

//	int choice, block_size,popsize, ngen, init_seed;
    if (argc< 5)
    {
        printf("\n Usage ./nsga2r GNUPLOT<1:YES 0: NO>  job_block_size   pop_size  number_of_generation seed<any unsigned integer>\n");
       return 1;
	}
	
	thread_data_array[0].choice = atoi(argv[1]);
    thread_data_array[0].block_size = atoi(argv[2]);
    thread_data_array[0].popsize = atoi(argv[3]);

    thread_data_array[0].ngen = atoi(argv[4]);
    thread_data_array[0].init_seed = atoi(argv[5]);
    thread_data_array[0].t_id = 0;
    thread_data_array[1].t_id = 1;
    
	FILE *fp;
	//MPI_REQUEST request;
	
	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&me);
	
	if(me == 0)
	{
		pthread_t threads[2];
		
		pthread_create(&threads[0],NULL,schedule,(void*)&thread_data_array[0]);
		pthread_create(&threads[1],NULL,schedule,(void*)&thread_data_array[1]);
		
		
		pthread_exit(NULL);
		/*
		time_wait = ;
		run_time = ;
		recvid = ;
//		if(no line read)	{
//			MPI_Abort(MPI_COMM_WORLD, 911);
//		}
		// wait for time_wait
		MPI_Send(&run_time, 1, MPI_DOUBLE, recv_id, 1, MPI_COMM_WORLD);
		*/
		
	}
	else
	{

	
		pthread_t threads[2];
		
		pthread_create(&threads[0],NULL,receivedeb,(void*)0);
		pthread_create(&threads[1],NULL,receivedeb,(void*)1);
		
		
		pthread_exit(NULL);
	
        }
	
	
	MPI_Finalize();
	

	return 0;
}
