#include <stdio.h>
#include <stdlib.h>
#include <map>
# define INF 1.0e14
# define EPS 1.0e-14


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



int main(int argc, char *argv[]){

FILE *fp;
int i,a,b,g,j=0,flag=0;
double c,d=0.0,f,sum=0.0,start,end, uptime,utilization;
char state[20];
int pred_id;
double s, e;
std::map <int,gridlet> job_bag;
std::map <int,gridlet>::iterator it;
std::map <int,gridlet>::iterator temp_it;
printf("%s %s\n", argv[0], argv[1]);
for(i=1;i<=argc;i++){
		fp = fopen(argv[i],"r");
		if(fp){
			while(fgetc(fp)!=EOF){
				fseek(fp,-1,SEEK_CUR);
				gridlet g;
				fscanf(fp,"%d\t",&g.job_id);
				fscanf(fp,"%d\t",&g.res_id);
				fscanf(fp,"%lf\t",&g.start);
				if(flag==0){
					start=c;
					flag=1;
				}
				fscanf(fp,"%lf\t",&g.exec_time);
				fscanf(fp,"%lf\t",&g.t_limit);
				fscanf(fp,"%lf\t",&g.cost);
				fscanf(fp,"%d\t",&g.p_id);
				fscanf(fp,"%d\t",&g.j_type);
				fscanf(fp,"%s",state);

				job_bag[g.job_id]=g;

			}
		fclose(fp);
		}

}

for(it=job_bag.begin();it!=job_bag.end();it++){

	pred_id = it->second.p_id; 
	if(pred_id!=-1){
	temp_it=job_bag.find(pred_id);
		if(temp_it!=job_bag.end()){
			e = temp_it->second.start + temp_it->second.exec_time ;
			s=it->second.start;
			if(s-e > EPS){
			sum += s-e;
			}
			else{
				printf("error -1\n");
			}			
			printf("job ID %d\tpred ID%d\tPred start= %lf\t exec time =%lf \t pred end= %lf\t ID start =%lf\n",it->first,temp_it->first,temp_it->second.start,temp_it->second.exec_time,temp_it->second.start+temp_it->second.exec_time,it->second.start );
		}
		else{
			printf("For ID %d could not find pred id %d in map\n",it->first, pred_id);
		}

	}
	else {
			printf("job ID %d\tpred ID%d\tPred end =%lf\t ID start =%lf\n",it->first,-1,0.0,it->second.start );
	}
}
	printf("sum = %lf\n",sum);
	return 0;
}
