/* Some utility functions (not part of the algorithm) */

# include <stdio.h>
# include <stdlib.h>
# include <math.h>
# include <unistd.h>
# include <sys/time.h>
# include <sys/types.h>
# include <string.h>

# include "global.h"
# include "rand.h"

/* Function to return the maximum of two variables */
double maximum (double a, double b)
{
    if (a>b)
    {
        return(a);
    }
    return (b);
}

/* Function to return the minimum of two variables */
double minimum (double a, double b)
{
    if (a<b)
    {
        return (a);
    }
    return (b);
}

void remove_rescheduled_jobs(double val,int n){
int i,a,b,g,flag=0;
double c,d,e,f;
char buf[20],state[20],ch;
unsigned long int size;
FILE **ofile;
for (i=0;i<NUM_RESOURCES;i++){
	flag=0;
	c=0;
	sprintf(buf,"schedule_%d.out",resource_id[i]);
	ofile= (FILE**) malloc(NUM_RESOURCES * sizeof(FILE*));
	ofile[i]= fopen(buf,"r+");
	if (ofile[i]){
			fseek(ofile[i],-6000,SEEK_END);
			while((ch=fgetc(ofile[i]))!='\n' && ch !=EOF);
			while(fgetc(ofile[i])!=EOF){
					fscanf(ofile[i],"%d",&a);
				fscanf(ofile[i],"%d",&b);
				fscanf(ofile[i],"%lf",&c);
				if(c >= val ){
					flag=1;
					break;
				}
				fscanf(ofile[i],"%lf",&d);
				fscanf(ofile[i],"%lf",&e);
				fscanf(ofile[i],"%lf",&f);
				fscanf(ofile[i],"%d",&g);
				fscanf(ofile[i],"%d",&g);
				fscanf(ofile[i],"%s",state);
			}
			if(flag==1){
				while(fgetc(ofile[i])!='\n') fseek(ofile[i],-2,SEEK_CUR);
				
				size=ftell(ofile[i]);
				fclose(ofile[i]);
				truncate(buf,size);
				}
				else 
					fclose(ofile[i]);
		}
	}


}


void read_scheduled_jobs(){
int i,a,b,g,flag=0,j=0;
double c,d,e,f;
char buf[20],state[20],ch;
unsigned long int size;
FILE **ofile;
for (i=0;i<NUM_RESOURCES;i++){
	flag=0;
	c=0;
	sprintf(buf,"schedule_%d.out",resource_id[i]);
	ofile= (FILE**) malloc(NUM_RESOURCES * sizeof(FILE*));
	ofile[i]= fopen(buf,"r+");



	while(fgetc(ofile[i])!=EOF){
		fscanf(ofile[i],"%d",&a);
		fscanf(ofile[i],"%d",&b);
		fscanf(ofile[i],"%lf",&c);
		fscanf(ofile[i],"%lf",&d);
		fscanf(ofile[i],"%lf",&e);
		fscanf(ofile[i],"%lf",&f);
		fscanf(ofile[i],"%d",&g);
		fscanf(ofile[i],"%d",&g);
		fscanf(ofile[i],"%s",state);
		if(!strcmp(state,"queued")){
			fseek(ofile[i],-6,SEEK_CUR);
			fprintf(ofile[i],"j_done");
			break;
		}
	}
	j=10;
	while(fgetc(ofile[i])!=EOF  ){
		if(j-- && fgetc(ofile[i])!=EOF ){
		fscanf(ofile[i],"%d",&a);
		fscanf(ofile[i],"%d",&b);
		fscanf(ofile[i],"%lf",&c);
		fscanf(ofile[i],"%lf",&d);
		fscanf(ofile[i],"%lf",&e);
		fscanf(ofile[i],"%lf",&f);
		fscanf(ofile[i],"%d",&g);
		fscanf(ofile[i],"%d",&g);
			fprintf(ofile[i],"\tj_done");
		}
	}
		fclose(ofile[i]);
	}
}


int read_unfinished_jobs(int file_id,int u_job_id[],double u_job_time[],double u_time_limit[],double  u_job_cost[],int  u_predecessor_job_id[],int  u_job_type[],int num){

int i,a,b,g,h,j=0;
double c,d,e,f;
char buf[20],state[20];
FILE *fp;
sprintf(buf,"schedule_%d.out",file_id);
fp= fopen(buf,"r+");

	while(fgetc(fp)!=EOF){
		fscanf(fp,"%d",&a);
		fscanf(fp,"%d",&b);
		fscanf(fp,"%lf",&c);
		fscanf(fp,"%lf",&d);
		fscanf(fp,"%lf",&e);
		fscanf(fp,"%lf",&f);
		fscanf(fp,"%d",&g);
		fscanf(fp,"%d",&h);
		fscanf(fp,"%s",state);
		if(!strcmp(state,"queued")){
			fseek(fp,-6,SEEK_CUR);
			fprintf(fp,"j_done");
			break;
		}
	}
	u_job_id[num] =a;
	u_job_time[num]=d;
	u_time_limit[num]=e;
	u_job_cost[num]=f;
	u_predecessor_job_id[num]=g;
	u_job_type[num]=h;
	num++;
	
	while(fgetc(fp)!=EOF  ){

		fscanf(fp,"%d",&u_job_id[num]);
		fscanf(fp,"%d",&b);
		fscanf(fp,"%lf",&c);
		fscanf(fp,"%lf",&u_job_time[num]);
		fscanf(fp,"%lf",&u_time_limit[num]);
		fscanf(fp,"%lf",&u_job_cost[num]);
		fscanf(fp,"%d",&u_predecessor_job_id[num]);
		fscanf(fp,"%d",&u_job_type[num]);
		fscanf(fp,"%s",state);
		num++;

	}
	fclose(fp);

return num;	
}

void merge_resource_file(){

FILE *resource_file, *add, *dlt,*mod;
int i,r_num,j,a,c,k;
double b,d,e;
double r_cost[50],r_power[50],r_freq[50],r_time[50][5];
int r_id[50],r_type[50];
int dlt_id[50], num_dlt,flag;
struct timeval prog_end, prog_elapsed,prog_start;
long long int elapsed=0;

resource_file = fopen("resource.in","r");
add = fopen("resource_add.in","r");
dlt = fopen("resource_delete.in","r");

mod = fopen("resource_modify.in","r");

if(dlt){
	i=0;
	while(fgetc(dlt)!=EOF){
		fseek(dlt,-1,SEEK_CUR);		
		fscanf(dlt,"%d",&dlt_id[i]);
		i++;
	}
	num_dlt=i-1;
//	printf("num_dlt %d",num_dlt);
}
		
	i=0;

	while(fgetc(resource_file)!=EOF){
		fseek(resource_file,-1,SEEK_CUR);	
		fscanf(resource_file,"%d\t",&r_id[i]);

		fscanf(resource_file,"%lf\t",&r_cost[i]);

		fscanf(resource_file,"%d\t",&r_type[i]);

		fscanf(resource_file,"%lf\t",&r_power[i]);
		
		fscanf(resource_file,"%lf\t",&r_freq[i]);		

		for(j=0;j<5;j++)
				fscanf(resource_file,"%lf\t",&r_time[i][j]);
		flag=0;				
		for(j=0;j<num_dlt;j++){
			if(dlt_id[j]==r_id[i]){
				flag=1;
				break;
			}
		}

		if(flag==0){	
//			printf("\n %d -- %d", r_id[i],i);		
			i++;
		}

	}
	r_num=i;
//	printf("r_num %d",r_num);	

if(mod){
while(fgetc(mod)!=EOF){
//		if(fgetc(add)!=EOF){
			fseek(mod,-1,SEEK_CUR);
			

			
			fscanf(mod,"%d\t",&a);
//			printf("\nid added %d",a);
			
			fscanf(mod,"%lf\t",&b);

			fscanf(mod,"%d\t",&c);

			fscanf(mod,"%lf\t",&d);
		
			fscanf(mod,"%lf\t",&e);
	i=0;
	while(i<r_num){
			if(r_id[i]==a){
			r_cost[i]=b;
/*			r_type[i]=c;		*/ // dont change type of resource
			r_power[i]=d;
			r_freq[i]=e;
			}
			i++;
	}
}			
			
			
}

	if (add){
		while(fgetc(add)!=EOF){
//		if(fgetc(add)!=EOF){
			fseek(add,-1,SEEK_CUR);
			

			
			fscanf(add,"%d\t",&a);
//			printf("\nid added %d",a);
			
			fscanf(add,"%lf\t",&b);

			fscanf(add,"%d\t",&c);

			fscanf(add,"%lf\t",&d);
		
			fscanf(add,"%lf\t",&e);		

			flag=0;				
			if(c==0){					// insert_front
				for(j=r_num;j>0;j--){
					r_id[j]=r_id[j-1];
					r_cost[j]=r_cost[j-1];
					r_type[j]=r_type[j-1];
					r_power[j]=r_power[j-1];
					r_freq[j]=r_freq[j-1];
				
					for(k=0;k<5;k++){
						r_time[j][k]=r_time[j-1][k];
					}
				}
			
				r_id[0]=a;
				r_cost[0]=b;
				r_type[0]=c;
				r_power[0]=d;
				r_freq[0]=e;
				gettimeofday(&prog_end,NULL);
				elapsed = prog_end.tv_sec - prog_start.tv_sec;				
					for(k=0;k<5;k++){
						r_time[0][k]=  elapsed + module_running_time;
					}
				r_num++;
			}
			else						// insert back
			{
				
				r_id[r_num]=a;
				r_cost[r_num]=b;
				r_type[r_num]=c;
				r_power[r_num]=d;
				r_freq[r_num]=e;
				gettimeofday(&prog_end,NULL);
				elapsed = prog_end.tv_sec - prog_start.tv_sec;									
					for(k=0;k<5;k++){
						r_time[r_num][k]= elapsed + module_running_time;
				}
				r_num++;
			}
			
		
//			}
		}
		fclose(add);
	}

	fclose(resource_file);

	fclose(dlt);

	resource_file = fopen("resource.in","w");
	add = fopen("resource_add.in","w");
	dlt = fopen("resource_delete.in","w");
	mod = fopen("resource_modify.in","w");
	i=0;
	while(i<r_num){
			fprintf(resource_file,"%d\t",r_id[i]);

			fprintf(resource_file,"%lf\t",r_cost[i]);

			fprintf(resource_file,"%d\t",r_type[i]);		
		
			fprintf(resource_file,"%lf\t",r_power[i]);
			fprintf(resource_file,"%lf\t",r_freq[i]);
			for(j=0;j<5;j++)
					fprintf(resource_file,"%lf\t",r_time[i][j]);			
			fprintf(resource_file,"\n");
			i++;
	}
	fclose(resource_file);
	fclose(add);
	fclose(dlt);
	fclose(mod);
}
