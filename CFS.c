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
void init_MDS(MDS* mds,unsigned int node_id,unsigned int parent_nodeid,char* filename);
void print_time(time_t time);

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
            continue;
        }else if(strcmp(token,"cfs_exit")==0){
            return -2;
        }
        printf("\n");
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
    superBlockStruct superblock;
    lseek(*fd, 0, SEEK_SET);
    memcpy(&superblock,fd,sizeof(superblock));
    if(superblock.block_size==0){
        superblock.block_size=block_size;
        //superblock.numOfFiles=???
        strcpy(superblock.fileType,"cfs");
        lseek(*fd, 0, SEEK_SET);
        write(*fd, &superblock, sizeof(superBlockStruct));
        printf("Filling superBlock\n");
        MDS mds;
        init_MDS(&mds,0,0,"/");
        lseek(*fd,block_size,SEEK_SET);
        write(*fd,&mds,sizeof(MDS));
        close(*fd);
    }else{
        printf("File already exists\n");
    }
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

void print_time(time_t time){
    printf("%s\n",asctime(localtime(&time)));
}

void init_MDS(MDS* mds,unsigned int node_id,unsigned int parent_nodeid,char* filename){
    mds->nodeid=node_id;
    mds->parent_nodeid=parent_nodeid;
    memcpy(mds->filename,filename,sizeof(mds->filename));
    mds->type=0;
    mds->size=0;
    mds->creation_time=mds->access_time=mds->modification_time=time(NULL);
}