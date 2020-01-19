#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<fcntl.h>
#include<unistd.h>
#include "struct.h"

#define PERMS 0644

int cfs_created=0;
int* open_fd=NULL;

int scan_options();
void cfs_create(char* filename,int block_size,int* fd);

int main(int argc,char** argv){
    int err=scan_options();
    
}

int scan_options(){
    const char skip[2]=" ";
    char* token;
    char* buffer;
    size_t bufsize=150;
    buffer=malloc(sizeof(char)*bufsize);
   
    while(getline(&buffer,&bufsize,stdin)!=EOF){
        token=strtok(buffer,"\n");
        token=strtok(buffer,skip);
        if(strcmp(token,"cfs_create")==0){
            if(token != NULL ) {
                token = strtok(NULL,skip);
            }
            if(token==NULL||strcmp(token,"-bs")!=0){
                printf("Input Error\n");
                continue;
            }
            if(token != NULL ) {
                token = strtok(NULL,skip);
            }
            if(token==NULL){
                printf("Input Error\n");
                continue;
            }
            char* filename=malloc(strlen(token)+1);
            strcpy(filename,token);
            printf("Give block size: ");
            int bs;
            scanf("%d",&bs);
            int fd;
            cfs_create(filename,bs,&fd);
            free(filename);
        }else if(strcmp(token,"cfs_exit")==0){
            return -2;
        }
        printf("Input Error\n");
    }
    free(buffer);
    return -1;
}

void cfs_create(char* filename,int block_size, int *fd){
    if ( (*fd=open(filename, O_CREAT|O_RDWR, PERMS)) == -1){
        perror("creating");
        return;
	}else{
		printf("Managed to get to the file successfully\n");
		cfs_created = 0;
	}
    char fileType[4] = "cfs";

    superBlockStruct superblock;
    printf("Filling superBlock\n");
    memcpy(&superblock.block_size, &block_size, sizeof(int));
    memcpy(&superblock.fileType, fileType, strlen(fileType));
    memset(&superblock.numOfFiles, 0, sizeof(int));

    int bytes1;
    lseek(*fd, 0, SEEK_SET);
    bytes1 = write(*fd, &superblock, sizeof(superBlockStruct));

    MDS mds;
    mds.nodeid=mds.parent_nodeid=0;
    memcpy(mds.filename,"/",2*sizeof(char));
    mds.type=0;
    mds.size=0;
    mds.creation_time=time(NULL);
    mds.access_time=time(NULL);
    mds.modification_time=time(NULL);
    printf("cr time:%ld\n",mds.creation_time);
    close(*fd);
}

void cfs_workwith(char *filename, int *fd){
    if(open_fd!=NULL){
        close(*fd);
    }
    if ( (*fd=open(filename, O_RDWR, PERMS)) == -1){
        perror("opening");
        return;
    }else{
        open_fd=fd;
        printf("Managed to open the file successfully\n");
    }
}