#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<fcntl.h>
#include<unistd.h>
#include"cfs_fun.h"




int scan_options();



int main(int argc,char** argv){
    open_fd=-1;
    int err=scan_options();

}

int scan_options(){
    const char skip[2]=" ";
    char* token;
    char* buffer;
    unsigned int current_nodeid = 0;
    int block_size = BLOCK_SIZE;
    size_t bufsize=150;
    buffer=malloc(sizeof(char)*bufsize);
    while(1){
        if(open_fd!=-1){
            cfs_pwd(current_nodeid,block_size);
        }
        printf(">");
        getline(&buffer,&bufsize,stdin);
        token=strtok(buffer,"\n");
        token=strtok(buffer,skip);
        if(strcmp(token,"cfs_create")==0){
            if(token != NULL ) {
                token = strtok(NULL,skip);
            }
            if(token==NULL){
                printf("Input Error\n");
                continue;
            }else if(strcmp(token,"-bs")==0){
                printf("Give block size: ");
                scanf("%d",&block_size);
                if(block_size < BLOCK_SIZE){
                    block_size = BLOCK_SIZE;
                }
                if(token != NULL ) {
                    token = strtok(NULL,skip);
                }
            }
            if(token==NULL){
                printf("Input Error\n");
                continue;
            }

            char* filename=malloc(strlen(token)+1);
            strcpy(filename,token);
            token = strtok(NULL,"\n");
            if(token!=NULL){
                printf("Input Error\n");
                continue;
            }

            cfs_create(filename,block_size);
            free(filename);
            continue;
        }else if(strcmp(token,"cfs_exit")==0){
            close(open_fd);
            return -2;
        }else if(strcmp(token,"cfs_workwith")==0){
            token=strtok(NULL,"\n");
            if(token == NULL ) {
                printf("Input Error\n");
                continue;
            }
            char* filename=malloc(strlen(token)+1);
            strcpy(filename,token);
            token = strtok(NULL,"\n");
            if(token!=NULL){
                printf("Input Error\n");
                continue;
            }
            cfs_workwith(filename,&current_nodeid,&block_size);
            free(filename);
            continue;
        }else if(strcmp(token,"cfs_mkdir")==0){
            token=strtok(NULL,"\n");
            if(token == NULL ) {
                printf("Input Error\n");
                continue;
            }

            char* dir_name=malloc(strlen(token)+1);
            strcpy(dir_name,token);
            token = strtok(NULL,"\n");
            if(token!=NULL){
                printf("Input Error\n");
                continue;
            }
            cfs_mkdir(dir_name,current_nodeid,block_size);
            free(dir_name);
            continue;
        }else if(strcmp(token,"cfs_touch")==0){
            token=strtok(NULL,"\n");
            if(token == NULL ) {
                printf("Input Error\n");
                continue;
            }

            char* args=malloc(strlen(token)+1);
            strcpy(args,token);
            token = strtok(NULL,"\n");
            if(token!=NULL){
                printf("Input Error\n");
                continue;
            }
            cfs_touch(args,current_nodeid,block_size);
            free(args);
        }else if(strcmp(token,"cfs_pwd")==0) {
            token = strtok(NULL, "\n");
            if (token != NULL) {
                printf("Input Error\n");
                continue;
            }
            cfs_pwd(current_nodeid, block_size);
            continue;
        }else if(strcmp(token,"cfs_cd")==0){
            token = strtok(NULL,"\n");
            if(token==NULL){
                printf("Input Error\n");
                continue;
            }
            char* dest_name=malloc(strlen(token)+1);
            strcpy(dest_name,token);
            token = strtok(NULL,"\n");
            if(token!=NULL){
                printf("Input Error\n");
                continue;
            }
            cfs_cd(dest_name,&current_nodeid,block_size);
            free(dest_name);
            continue;
        }else if(strcmp(token,"cfs_ls")==0){
            token = strtok(NULL, "\n");
            call_ls(token,current_nodeid,block_size);
            continue;
        }else if(strcmp(token,"cfs_rm")==0){
            token = strtok(NULL, "\n");
            if(token==NULL){
                printf("Input Error\n");
                continue;
            }
            cfs_rm(token,current_nodeid,block_size, 0);
            continue;
        }else if(strcmp(token,"cfs_cat")==0){
            token = strtok(NULL, "\n");
            if(token==NULL){
                printf("Input Error\n");
                continue;
            }
            cfs_cat(token,current_nodeid,block_size);
            continue;
        }else if(strcmp(token,"cfs_writefile")==0){
            token = strtok(NULL, "\n");
            if(token==NULL){
                printf("Input Error\n");
                continue;
            }
            char*filename=malloc(strlen(token)+1);
            strcpy(filename,token);
            cfs_writefile(current_nodeid,filename,block_size);
            token = strtok(NULL, " ");
            if(token!=NULL){
                printf("Input Error\n");
                continue;
            }
            free(filename);
            continue;
        }else if(strcmp(token,"cfs_ln")==0) {
            token = strtok(NULL, "\n");
            if (token == NULL) {
                printf("Input Error\n");
                continue;
            }
            cfs_ln(token, current_nodeid, block_size);
            continue;
        }else if(strcmp(token,"cfs_mv")==0){
            token = strtok(NULL, "\n");
            if(token==NULL){
                printf("Input Error\n");
                continue;
            }
            cfs_mv(token,current_nodeid,block_size);
            continue;
        }else if(strcmp(token,"cfs_cp")==0){
            token = strtok(NULL, "\n");
            if(token==NULL){
                printf("Input Error\n");
                continue;
            }
            cfs_cp(token,current_nodeid,block_size);
            continue;
        }
        printf("\n");
    }
    free(buffer);
    return -1;
}




















