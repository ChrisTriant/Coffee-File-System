#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<fcntl.h>

#define PERMS 0644

int cfs_created=0;

int scan_options();
void cfs_create(char* filename,int block_size);

int main(int argc,char** argv){


    while(1){
        int err=scan_options();
    }
    

}

int scan_options(){
    char buffer[150];
    const char skip[2]=" ";
    char* token;
    char* t=fgets(buffer,sizeof(buffer),stdin);
    if(t==NULL){
        return -1;
    }
    token=strtok(buffer,skip);
    if(strcmp(buffer,"cfs_create")==0){
        if(token != NULL ) {
                    token = strtok(NULL,skip);
                }
        if(token==NULL||strcmp(token,"-bs")!=0){
            return -1;
        }
        if(token != NULL ) {
            token = strtok(NULL,skip);
        }
        if(token==NULL){
            return -1;
        }
        char* filename=malloc(strlen(token)+1);
        strcpy(filename,token);
        printf("Give block size: ");
        int bs;
        scanf("%d",&bs);
        cfs_create(filename,bs);


    }
}

void cfs_create(char* filename,int block_size){
	int filedes;
	
	if ( (filedes=open(filename, O_CREAT|O_RDWR, PERMS)) == -1){
		perror("creating");
		exit(1);
		}
	else { 
		printf("Managed to get to the file successfully\n"); 
        cfs_created=1;
	}
    
    FILE* file=fopen(filename,"r+");
    fseek(file,0,SEEK_SET);
    char buffer[block_size];
    memset(buffer,0,block_size);
    printf("ok\n");
    memcpy(file,buffer,block_size);
    char token[4]="cfs";
    memcpy(file,token,sizeof(token));
        printf("ok\n");
    fseek(file,sizeof(token),SEEK_SET);
        printf("ok\n");
    memcpy(file,&block_size,sizeof(int));
    free(filename);
}