#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]){

FILE *fp;
int i=0,a=0,c,b,g,j=0,flag=0,type_0=0,type_1=0,p_job=0;
double d,e,f;
char state[20];

printf("%s %s\n", argv[0], argv[1]);

fp = fopen("../job.in","r");

if(fp){
    while(1){
    	fscanf(fp,"%d\t",&a);    
    	if(a==-1){
    		flag=1;
    		break;
    		}

    	fscanf(fp,"%d\t",&c); 

    	fscanf(fp,"%d\t",&c);  
    	  
    	fscanf(fp,"%lf\t",&d);   
    	
    	fscanf(fp,"%d\t",&b);   
    	if(b==-1){
    		p_job++;
    	}
    	
    	fscanf(fp,"%d\t",&b);
    	if(b==0){
    		type_0++;
    	}
    	else if(b==1){
    		type_1++;
    	}
    	
		i++;
    }
    d= (float)type_0/(float)i *100;
    e= (float)type_1/(float)i *100;
    f= (float)p_job/(float)i *100;
    printf("\ntype 0 = %lf%% \ntype 1 = %lf%%\nnon-predecessor jobs =%lf%%\n", d,e,f);
  
}
fclose(fp);
  return 0;
}
