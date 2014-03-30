# include <stdio.h>
# include <stdlib.h>
# include <math.h>
# include <unistd.h>
# include <string.h>


void add_resource(){
FILE *fp,*resource_file;
int id,type, flag, num, j, i=0, r_id[500], temp_i;
double watt, cost, freq, temp_d;

	printf("\n#### Enter details of resource with ID different from the following existing ID resources");
	resource_file = fopen("../resource.in","r");
	if (resource_file){
		while(fgetc(resource_file)!=EOF){
			fseek(resource_file,-1,SEEK_CUR);	
			fscanf(resource_file,"%d\t",&r_id[i]);
			printf("\n### ID : %d", r_id[i]);
			fscanf(resource_file,"%lf\t",&temp_d);
			fscanf(resource_file,"%d\t",&temp_i);
			fscanf(resource_file,"%lf\t",&temp_d);
			fscanf(resource_file,"%lf\t",&temp_d);		
			for(j=0;j<5;j++)
				fscanf(resource_file,"%lf\t",&temp_d);
			i++;
			}
	}
	num=i;
	printf("\n###################################################\n");
	fclose(resource_file);
	
do{
printf("### Enter resource id :");
scanf("%d",&id);
flag=0;
	for(i=0;i<num;i++){
		if(r_id[i]==id){
				flag=1;
				break;
		}
	}
}while(flag==1);


printf("\n### Enter resource cost :");
scanf("%lf",&cost);
do{
	printf("\n### Enter resource type 0/1 :");
	scanf("%d",&type);
}while(type<0 || type>1);
do{
	printf("\n### Enter resource power consumption (normalized) :");
	scanf("%lf",&watt);
}while(watt > 1 || watt < 0);
do{
	printf("\n### Enter resource frequency (normalized to 3GHz) :");
	scanf("%lf",&freq);
}while(freq > 1 || freq < 0);

	

fp = fopen("../resource_add.in","a");
	if (fp){			
			fprintf(fp,"\n%d\t",id);
			fprintf(fp,"%lf\t",cost);
			fprintf(fp,"%d\t",type);
			fprintf(fp,"%lf\t",watt);
			fprintf(fp,"%lf",freq);
	}
fclose(fp);
}


void modify_resource(){
FILE *resource_file, *mod;
int i=0,j=0,temp_i, r_id[500], num, flag,id,type;
double watt, cost, freq, temp_d;

	printf("\n#### Enter your option from following resource ID's given below and '-1' to EXIT");

	resource_file = fopen("../resource.in","r");
	if (resource_file){
		while(fgetc(resource_file)!=EOF){
			fseek(resource_file,-1,SEEK_CUR);	
			fscanf(resource_file,"%d\t",&r_id[i]);
			printf("\n### ID : %d", r_id[i]);
			fscanf(resource_file,"%lf\t",&temp_d);
			fscanf(resource_file,"%d\t",&temp_i);
			fscanf(resource_file,"%lf\t",&temp_d);
			fscanf(resource_file,"%lf\t",&temp_d);		
			for(j=0;j<5;j++)
				fscanf(resource_file,"%lf\t",&temp_d);
			i++;
			}
	}
	num=i;
	printf("\n###################################################\n");
	fclose(resource_file);

	
	flag=0;
	printf("\n\n### Enter ID : ");
	scanf("%d",&id);
		for(i=0;i<num;i++){
			if(r_id[i]==id){
				flag=1;
				break;
			}
		}

	mod=fopen("../resource_modify.in","a");
		if (mod && flag==1){
			fprintf(mod,"\n%d\t",id);
			printf("\n### Enter resource cost :");
			scanf("%lf",&cost);
			fprintf(mod,"%lf\t",cost);			
			do{
				printf("\n### Enter resource type 0/1 :");
				scanf("%d",&type);
			}while(type<0 || type>1);
			fprintf(mod,"%d\t",type);
			do{
				printf("\n### Enter resource power consumption (normalized) :");
				scanf("%lf",&watt);
			}while(watt > 1 || watt < 0);
			fprintf(mod,"%lf\t",watt);
			do{
				printf("\n### Enter resource frequency (normalized to 3GHz) :");
				scanf("%lf",&freq);
			}while(freq > 1 || freq < 0);
			fprintf(mod,"%lf",freq);
		}
		else {
			printf("\n ###Either resource does not exist OR 'resource_modify.in file not found'");
		}
fclose(mod);
}


void delete_resource(){

FILE *resource_file, *dlt;
int i=0,j=0,temp_i, r_id[500], num, flag;
double temp_d;

	printf("\n#### Enter your option from following resource ID's given below and '-1' to EXIT");

	resource_file = fopen("../resource.in","r");
	if (resource_file){
		while(fgetc(resource_file)!=EOF){
			fseek(resource_file,-1,SEEK_CUR);	
			fscanf(resource_file,"%d\t",&r_id[i]);
			printf("\n### ID : %d", r_id[i]);
			fscanf(resource_file,"%lf\t",&temp_d);
			fscanf(resource_file,"%d\t",&temp_i);
			fscanf(resource_file,"%lf\t",&temp_d);
			fscanf(resource_file,"%lf\t",&temp_d);		
			for(j=0;j<5;j++)
				fscanf(resource_file,"%lf\t",&temp_d);
			i++;
			}
	}
	num=i;
	printf("\n###################################################\n");
	fclose(resource_file);

	do{
	flag=0;
	printf("\n\n### Enter ID : ");
	scanf("%d",&temp_i);
		for(i=0;i<num;i++){
			if(r_id[i]==temp_i){
				flag=1;
				break;
			}
		}
		
	dlt=fopen("../resource_delete.in","a");
		if (dlt && temp_i!=-1 && flag==1){
			fprintf(dlt,"\n%d\t",temp_i);
		}
		else {
			printf("\n ###Either resource does not exist OR 'resource_add.in file not found'");
		}
	fclose(dlt);
	}while(temp_i!=-1);

}

int main(){

int choice;

do {
printf("%c[2J",27);
//System("clear");
printf("\n################ Resource Manager ################# ");
printf("\n#### 1. Add a Resource");
printf("\n#### 2. Delete an Existing resource");
printf("\n#### 3. Modify an Existing resource");
printf("\n#### 4. Exit");
printf("\n###################################################\n");
printf("\nEnter Your choice :  ");
scanf("%d",&choice);
	switch(choice){
		case 1:
			add_resource();
			break;
		case 2:
			delete_resource();
			break;
		case 3:
			modify_resource();
			break;
		case 4:
			break;
		default:
			printf("\n Re-enter your choice\n");
			break;
	}
}while(choice!=4);

}

