#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<fcntl.h>
#include<unistd.h>
#include "struct.h"

#define PERMS 0644

int open_fd=-1;

int scan_options();
void cfs_create(char* filename,int block_size);
void init_MDS(MDS* mds,unsigned int node_id,unsigned int parent_nodeid,char* filename);
void print_time(time_t time);
void cfs_workwith(char *filename,int* cur_nodeid,int* block_size);
void cfs_mkdir(char* dir_name,unsigned int nodeid,unsigned int parent,int block_size,int fd);
int find_space(int fd,int block_size);

int main(int argc,char** argv){
    int err=scan_options();
    
}

int scan_options(){
    const char skip[2]=" ";
    char* token;
    char* buffer;
    unsigned int current_nodeid;
    int block_size; 
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
            cfs_create(filename,bs);
            free(filename);
            continue;
        }else if(strcmp(token,"cfs_exit")==0){
            return -2;
        }else if(strcmp(token,"cfs_workwith")==0){
            if(token != NULL ) {
                token = strtok(NULL,skip);
            }
            else{
                printf("Input Error\n");
                continue;
            }
            char* filename=malloc(strlen(token)+1);
            strcpy(filename,token);
            cfs_workwith(filename,&current_nodeid,&block_size);
            free(filename);
        }else if(strcmp(token,"cfs_mkdir")==0){
            if(token != NULL ) {
                token = strtok(NULL,skip);
            }
            else{
                printf("Input Error\n");
                continue;
            }
            char* dir_name=malloc(strlen(token)+1);
            strcpy(dir_name,token);
            printf("%s",dir_name);
            int nodeid=find_space(open_fd,block_size);
            cfs_mkdir(dir_name,nodeid,current_nodeid,block_size,open_fd);
        }
        printf("\n");
    }
    free(buffer);
    return -1;
}

void cfs_create(char* filename,int block_size){
    int fd;
    if ( (fd=open(filename, O_CREAT|O_RDWR, PERMS)) == -1){
        perror("creating");
        return;
	}else{
		printf("Managed to get to the file successfully. File desc: %d\n",fd);
	}
    superBlockStruct superblock;
    lseek(fd, 0, SEEK_SET);
    read(fd,&superblock,sizeof(superblock));
    if(strcmp(superblock.fileType,"cfs")!=0){
        superblock.block_size=block_size;
        //superblock.numOfFiles=???
        strcpy(superblock.fileType,"cfs");
        lseek(fd, 0, SEEK_SET);
        write(fd, &superblock, sizeof(superBlockStruct));
        printf("Filling superBlock\n");
        MDS mds;
        init_MDS(&mds,0,0,"/");
        lseek(fd,block_size,SEEK_SET);
        write(fd,&mds,sizeof(MDS));
        close(fd);
    }else{
        printf("File already exists\n");
    }
}

void cfs_workwith(char *filename,int* cur_nodeid,int* block_size){
    int fd;
    if(open_fd!=-1){
        close(open_fd);         //close the currently opened cfs file
    }
    if ( (fd=open(filename, O_RDWR, PERMS)) == -1){
        perror("opening");
        return;
    }else{
        superBlockStruct superblock;
        read(fd,&superblock,sizeof(superBlockStruct));
        if(strcmp(superblock.fileType,"cfs")!=0){
            printf("%s\n",superblock.fileType);
            printf("WRONG FILE TYPE\n");
        }else{
            open_fd=fd;
            *cur_nodeid=0;
            *block_size=superblock.block_size;
            printf("Managed to open the file successfully\n");
        }
    }
}

void cfs_mkdir(char* dir_name,unsigned int nodeid,unsigned int parent,int block_size,int fd){
    MDS mds;
    init_MDS(&mds,nodeid,parent,dir_name);
    lseek(fd,(nodeid+1)*block_size,SEEK_SET);
    write(fd,&mds,sizeof(MDS));
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

int find_space(int fd,int block_size){
    int i;
    lseek(fd,2*block_size,SEEK_SET);
    MDS mds;
    memcpy(&mds,&fd,sizeof(MDS));
    for(i=0;mds.nodeid!=0;i++){
        lseek(fd,block_size,SEEK_CUR);
    }
    return i;
}