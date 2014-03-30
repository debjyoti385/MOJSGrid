#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]){

FILE *fp;
int i=0,a,b,g,j=0,flag=0;
double c,old_c=-1,d,e,f;
char state[20];

printf("%s %s\n", argv[0], argv[1]);

fp = fopen(argv[1],"r");

if(fp){
	while(fgetc(fp)!=EOF){
		fseek(fp,-1,SEEK_CUR);
		fscanf(fp,"%d\t",&a);
		fscanf(fp,"%d\t",&b);
		fscanf(fp,"%lf\t",&c);
		if(c<old_c){
			printf("\n FILE %s FAILED ", argv[1]);
			printf("%d old = %lf  \t new = %lf",i, old_c,c);
			exit(0);
		}
		old_c=c;
		fscanf(fp,"%lf\t",&d);
		fscanf(fp,"%lf\t",&e);
		fscanf(fp,"%lf\t",&f);
		fscanf(fp,"%d\t",&g);
		fscanf(fp,"%d\t",&g);
		fscanf(fp,"%s",state);
		i++;
	}
}
fclose(fp);
			printf("\n FILE %s PASSED \n ", argv[1]);
	return 0;
}
