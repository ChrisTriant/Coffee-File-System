#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<fcntl.h>
#include<unistd.h>
#include "struct.h"

#define PERMS 0644
#define FILENAME_SIZE 100
#define BLOCK_SIZE 512

int open_fd=-1;

int scan_options();
void cfs_create(char* filename,int block_size);
void init_MDS(MDS* mds,unsigned int node_id,unsigned int parent_nodeid,char* filename,unsigned int type);
void print_time(time_t time);
void cfs_workwith(char *filename,unsigned int* cur_nodeid,int* block_size);
void cfs_mkdir(char* dir_name,unsigned int parent,int block_size);
int find_space(int fd);
void get_superblock_data(int fd);
void get_node_mds(MDS* mds,unsigned int nodeid,int fd,int block_size);
void update_node_mds(MDS* mds,unsigned int nodeid,int block_size,int fd);
void get_node_data(int fd,unsigned int nodeid,int block_size);
void write_node_index(unsigned int nodeid,int counter,int fd);
void get_superblock(int fd,superBlockStruct* superblock);
void set_superblock(int fd,superBlockStruct* superblock);
int GetNodeCounter(int fd);
void cfs_touch(char* args,unsigned int parent,int block_size);
void cfs_pwd(unsigned int nodeid,int block_size);
unsigned int find_data(unsigned int nodeid,char*name,int block_size,int fd);
void cfs_cd(char* dest_name,unsigned int* current_nodeid,int block_size,int to_root);
void cfs_ls(unsigned int current_nodeid,int block_size,options* op);
void call_ls(char* args,int current_nodeid,int block_size);
int cmpfilename(const void* mds1 ,const void* mds2);
void cfs_rm(char* args,unsigned int current_nodeid, int block_size, int op);
void rm_file(MDS mds, int nodeid, int current_nodeid, int block_size, int fd);
void update_size(unsigned int nodeid,int block_size, int size, int fd);
void rm_nodeid(unsigned int nodeid,unsigned int dest_nodeid,int block_size, int fd);
void rec_rm(unsigned int nodeid,int block_size, int fd);
void new_hole(int nodeid, int fd);
void cfs_writefile(unsigned int curNodeid, char *fileName, int blockSize);
void cfs_cat(char *args, int curNodeid, int block_size);
void cfs_ln(char *token, int curNodeid, int block_size);
void cfs_mv(char *args, int curNodeid, int block_size);
int move_cd(char* dest_name,unsigned int* current_nodeid,int block_size,int to_root);


int main(int argc,char** argv){
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
            cfs_cd(dest_name,&current_nodeid,block_size, 1);
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
    }
    printf("Managed to get to the file successfully. File desc: %d\n",fd);
    superBlockStruct superblock;
    lseek(fd, 0, SEEK_SET);
    int bytes = read(fd,&superblock,sizeof(superblock));
    if(bytes == 0){
        superblock.block_size=block_size;
        superblock.numOfFiles=0;
        superblock.counter=0;
        superblock.node_counter=1;
        strcpy(superblock.fileType,"cfs");
        lseek(fd, 0, SEEK_SET);
        write(fd, &superblock, sizeof(superBlockStruct));
        printf("Filling superBlock\n");
        MDS mds;
        init_MDS(&mds,0,0,"/",0);
        lseek(fd,block_size,SEEK_SET);
        write(fd,&mds,sizeof(MDS));
        close(fd);
    }else{
        if(strcmp(superblock.fileType, "cfs") == 0) {
            printf("File already exists\n");
        } else{
            printf("This is not a cfs file\n");
        }
    }
}

void cfs_workwith(char *filename,unsigned int* cur_nodeid,int* block_size){
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

void cfs_mkdir(char* dir_name,unsigned int parent,int block_size){
    if(open_fd==-1){
        printf("Error! No CFS file in use.\n");
        return;
    }
    char* token;
    token=strtok(dir_name," ");
    if(find_data(parent,token,block_size,open_fd)!=-1){
        printf("A directory with this name already exits\n");
        return;
    }
    MDS parent_mds;
    get_node_mds(&parent_mds,parent,open_fd,block_size);   //get parent mds to check counter and update
    if(parent_mds.counter<=(block_size-sizeof(MDS))/sizeof(int)){
        unsigned int nodeid=find_space(open_fd);
        MDS new_mds;
        init_MDS(&new_mds,nodeid,parent,token,0);          //create new mds
        lseek(open_fd,(nodeid+1)*block_size,SEEK_SET);
        write(open_fd,&new_mds,sizeof(MDS));
        lseek(open_fd, block_size- sizeof(int), SEEK_CUR);
        int buf = 0;
        write(open_fd, &buf, sizeof(char));


        get_node_data(open_fd,parent,block_size);
        write_node_index(nodeid,parent_mds.counter,open_fd);
        parent_mds.counter++;
        parent_mds.modification_time=time(NULL);
        update_node_mds(&parent_mds,parent,block_size,open_fd);
        update_size(parent,block_size, sizeof(MDS), open_fd);

        superBlockStruct super_block;
        get_superblock(open_fd,&super_block);
        super_block.node_counter++;
        set_superblock(open_fd,&super_block);
    }else{
        printf("This directory is full\n");
    }
    token=strtok(NULL,"\n");
    if(token!=NULL){
        cfs_mkdir(token,parent,block_size);
    }
}

void cfs_touch(char* args,unsigned int parent,int block_size){
    if(open_fd==-1){
        printf("Error! No CFS file in use.\n");
        return;
    }
    char* token;
    int option=0;
    token=strtok(args," ");
    if(strcmp(token,"-m")==0){
        option=1;
        token=strtok(NULL," ");
        if(token == NULL ) {
            printf("Input Error\n");
        }
    }else if(strcmp(token,"-a")==0){
        option=2;
        token=strtok(NULL," ");
        if(token == NULL ) {
            printf("Input Error\n");
        }
    }

    MDS parent_mds;
    get_node_mds(&parent_mds,parent,open_fd,block_size);   //get parent mds to check counter and update
    unsigned int nodeid=find_data(parent,token,block_size,open_fd);
    if(nodeid == -1){
        if(parent_mds.counter<=(block_size-sizeof(MDS))/sizeof(int)){
            nodeid=find_space(open_fd);
            MDS new_mds;
            init_MDS(&new_mds,nodeid,parent,token,1);          //create new mds
            lseek(open_fd,(nodeid+1)*block_size,SEEK_SET);
            write(open_fd,&new_mds,sizeof(MDS));
            lseek(open_fd, block_size- sizeof(int), SEEK_CUR);
            int buf = 0;
            write(open_fd, &buf, sizeof(char));

            get_node_data(open_fd,parent,block_size);
            write_node_index(nodeid,parent_mds.counter,open_fd);
            parent_mds.counter++;
            update_node_mds(&parent_mds,parent,block_size,open_fd);
            update_size(parent,block_size, sizeof(MDS), open_fd);

            superBlockStruct super_block;
            get_superblock(open_fd,&super_block);
            super_block.node_counter++;
            set_superblock(open_fd,&super_block);
        }else{
            printf("This directory is full\n");
        }
    }else{
        MDS mds;
        if(option==1){
            get_node_mds(&mds,nodeid,open_fd,block_size);
            mds.modification_time=time(NULL);
            update_node_mds(&mds,nodeid,block_size,open_fd);
        }else if(option==2){
            get_node_mds(&mds,nodeid,open_fd,block_size);
            mds.access_time=time(NULL);
            update_node_mds(&mds,nodeid,block_size,open_fd);
        }else{
            printf("File already exists\n");
        }
    }

    token=strtok(NULL,"\n");
    if(token!=NULL){
        cfs_touch(token,parent,block_size);
    }
}

void cfs_pwd(unsigned int nodeid,int block_size){
    if(open_fd==-1){
        printf("Error! No CFS file in use.\n");
        return;
    }
    MDS mds;
    get_node_mds(&mds,nodeid,open_fd,block_size);
    if(strcmp(mds.filename,"/")!=0){
        cfs_pwd(mds.parent_nodeid,block_size);
        printf("/%s",mds.filename);
    }
}

void cfs_cd(char* dest_name,unsigned int* current_nodeid,int block_size,int to_root){
    if(open_fd==-1){
        printf("Error! No CFS file in use.\n");
        return;
    }
    MDS mds;
    get_node_mds(&mds,*current_nodeid,open_fd,block_size);
    if(strcmp(dest_name,".")==0){
        *current_nodeid=mds.nodeid;
    }else if(strcmp(dest_name,"..")==0){
        *current_nodeid=mds.parent_nodeid;
    }else if(strcmp(dest_name,"/")==0 || (dest_name[0]=='/' && to_root==1)){
        *current_nodeid=0;
        char* token=strtok(dest_name,"\n");
        to_root=0;
        if(token!=NULL && strcmp(dest_name,"/")!=0){
            cfs_cd(token,current_nodeid,block_size,to_root);
        }
    }else{
        to_root=0;
        char* token;
        token=strtok(dest_name,"/");
        int check_id=find_data(*current_nodeid,token,block_size,open_fd);
        if(check_id != -1){
            get_node_mds(&mds,check_id,open_fd,block_size);
            if(mds.type==1){
                printf("This is not a directory\n");
                return;
            }
            *current_nodeid=check_id;
            get_node_mds(&mds,*current_nodeid,open_fd,block_size);
            mds.access_time=time(NULL);
            update_node_mds(&mds,*current_nodeid,block_size,open_fd);
            token=strtok(NULL,"\n");
            if(token!=NULL){
                cfs_cd(token,current_nodeid,block_size,to_root);
            }
        }else{
            printf("No such file or directory \n");
        }
    }
}

int move_cd(char* dest_name,unsigned int* current_nodeid,int block_size,int to_root){
    if(open_fd==-1){
        printf("Error! No CFS file in use.\n");
        return 1;
    }
    MDS mds;
    get_node_mds(&mds,*current_nodeid,open_fd,block_size);
    if(strcmp(dest_name,".")==0){
        *current_nodeid=mds.nodeid;
    }else if(strcmp(dest_name,"..")==0){
        *current_nodeid=mds.parent_nodeid;
    }else if(strcmp(dest_name,"/")==0 || (dest_name[0]=='/' && to_root==1)){
        *current_nodeid=0;
        char* token=strtok(dest_name,"\n");
        to_root=0;
        if(token!=NULL && strcmp(dest_name,"/")!=0){
            cfs_cd(token,current_nodeid,block_size,to_root);
        }
    }else{
        to_root=0;
        char* token;
        token=strtok(dest_name,"/");
        int check_id=find_data(*current_nodeid,token,block_size,open_fd);
        if(check_id != -1){
            get_node_mds(&mds,check_id,open_fd,block_size);
            if(mds.type==1){
                return 1;
            }
            *current_nodeid=check_id;
            get_node_mds(&mds,*current_nodeid,open_fd,block_size);
            mds.access_time=time(NULL);
            update_node_mds(&mds,*current_nodeid,block_size,open_fd);
            token=strtok(NULL,"\n");
            if(token!=NULL){
                cfs_cd(token,current_nodeid,block_size,to_root);
            }
        }else{
            return -1;
        }
    }
}

void cfs_ls(unsigned int current_nodeid,int block_size,options* op){
    MDS mds;
    get_node_mds(&mds,current_nodeid,open_fd,block_size);
    get_node_data(open_fd,current_nodeid,block_size);

    int data_id;
    int i;
    int offset = 1;

    if(mds.counter==0){
        return;
    }
    MDS** mds_array=(MDS**)malloc(mds.counter*sizeof(MDS*));
    int counter=0;
    for(i=0;i<mds.counter;i++){
        read(open_fd,&data_id,sizeof(int));
        if(data_id!=0){
            mds_array[i]=(MDS*)malloc(sizeof(MDS));
            get_node_mds(mds_array[i],data_id,open_fd,block_size);
            if(op->d_option==1){
                if(mds_array[i]->type==0){
                    counter++;
                }else{
                    free(mds_array[i]);
                }
            }else if(op->h_option==1){
                if(mds_array[i]->type==2){
                    counter++;
                }else{
                    free(mds_array[i]);
                }
            }else{
                counter++;
            }
        }else{
            i--;
        }
        get_node_data(open_fd,current_nodeid,block_size);
        lseek(open_fd,offset*sizeof(int),SEEK_CUR);
        offset++;
    }
    if(op->u_option!=1){
        qsort(mds_array,counter,sizeof(MDS*),cmpfilename);
    }
    if(op->a_option==1){
        printf(".\t..\t");
    }
    for(i=0;i<counter;i++){
        if(mds_array[i]->filename[0]=='.'){
            if(op->a_option==1){
                if(op->l_option!=1){
                    printf("%s\t",mds_array[i]->filename);
                }else{
                    printf("%s\t%d bytes\n",mds_array[i]->filename,mds_array[i]->size);
                    print_time(mds_array[i]->creation_time);
                    print_time(mds_array[i]->access_time);
                    print_time(mds_array[i]->modification_time);
                    printf("\n");
                }
                free(mds_array[i]);
            }
        }else{
            if(op->l_option!=1){
                printf("%s\t",mds_array[i]->filename);
            }else{
                printf("%s\t%d bytes\n",mds_array[i]->filename,mds_array[i]->size);
                print_time(mds_array[i]->creation_time);
                print_time(mds_array[i]->access_time);
                print_time(mds_array[i]->modification_time);
                printf("\n");
            }
            free(mds_array[i]);
        }
    }
    free(mds_array);
    if(op->r_option==1){
        offset=1;
        MDS data_mds;
        get_node_mds(&mds,current_nodeid,open_fd,block_size);
        for(i=0;i<mds.counter;i++){
            read(open_fd,&data_id,sizeof(int));
            if(data_id!=0){
                get_node_mds(&data_mds,data_id,open_fd,block_size);
                if(data_mds.type==0){
                    cfs_ls(data_id,block_size,op);
                }
            }else{
                i--;
            }
            get_node_data(open_fd,current_nodeid,block_size);
            lseek(open_fd,offset*sizeof(int),SEEK_CUR);
            offset++;
        }
    }
    printf("\n");
}

int cmpfilename(const void* mds1 ,const void* mds2){
    MDS** temp1,**temp2;
    temp1=(MDS**)mds1;
    temp2=(MDS**)mds2;
    return strcmp((*temp1)->filename,(*temp2)->filename);
}


void call_ls(char* args,int current_nodeid,int block_size){
    if(open_fd==-1){
        printf("Error! No CFS file in use.\n");
        return;
    }
    int req_files=0;
    char* token;
    options op;
    op.a_option=0;
    op.d_option=0;
    op.h_option=0;
    op.l_option=0;
    op.r_option=0;
    op.u_option=0;
    token=strtok(args," ");
    while(token != NULL) {
        if (strcmp(token, "-a") == 0) {
            op.a_option = 1;
        }else if (strcmp(token, "-r") == 0) {
            op.r_option = 1;
        }else if (strcmp(token, "-l") == 0) {
            op.l_option = 1;
        }else if (strcmp(token, "-u") == 0) {
            op.u_option = 1;
        }else if (strcmp(token, "-d") == 0) {
            op.d_option = 1;
        }else if (strcmp(token, "-h") == 0) {
            op.h_option = 1;
        }else{
            req_files++;
            char* dirname=malloc(strlen(token)+1);
            strcpy(dirname,token);
            int lsid=find_data(current_nodeid,dirname,block_size,open_fd);
            if(lsid==-1){
                token = strtok(NULL, " ");
                printf("%s not found in this directory\n",dirname);
                continue;
            }
            printf("%s\n",dirname);
            MDS mds;
            get_node_mds(&mds,lsid,open_fd,block_size);
            if(mds.type==0){
                cfs_ls(lsid,block_size,&op);
            }else{
                printf("%s is not a director\n",mds.filename);
            }
            free(dirname);
        }
        token = strtok(NULL, " ");
    }
    if(token==NULL && req_files==0){
        cfs_ls(current_nodeid,block_size,&op);
    }
}


void cfs_cat(char *args, int curNodeid, int block_size){
    if(open_fd==-1){
        printf("Error! No CFS file in use.\n");
        return;
    }
    char *token;
    char* buffer=malloc((block_size-sizeof(MDS))* sizeof(char));
    char *outputfile = NULL;
    int outsize = 0;
    int inputgiven=0;
    int outputgiven=0;
    token = strtok(args," ");
    while(token!=NULL){
        if(strcmp(token,"-o")==0){
            token=strtok(NULL," ");
            if(token==NULL){
                printf("No output file given\n");
                free(buffer);
                return;
            }else{
                outputfile=malloc(strlen(token)+1);
                strcpy(outputfile, token);
                outputgiven=1;
                break;
            }
        }else{
            int nodeid=find_data(curNodeid,token,block_size,open_fd);
            if(nodeid==-1){
                printf("File %s not found\n",token);
                free(buffer);
                return;
            }else{
                char* buff = malloc((block_size- sizeof(MDS))*sizeof(char));
                get_node_data(open_fd, nodeid, block_size);
                MDS mds;
                get_node_mds(&mds, nodeid, open_fd, block_size);
                int bytes = read(open_fd, buff, mds.size- sizeof(MDS));
                if(outsize+bytes < block_size){
                    strcat(buffer,buff);
                    outsize+=bytes;
                    inputgiven=1;
                    free(buff);
                }else{
                    printf("Output File has no free space for %s", args);
                    free(buff);
                    break;
                }
            }
        }
        token=strtok(NULL," ");
    }
    if(!inputgiven){
        printf("Error! No input files given\n");
        return;
    }
    if(!outputgiven){
        printf("Error! No output files given\n");
        return;
    }
    printf("%s\n",buffer);

    int nodeid=find_data(curNodeid,outputfile,block_size,open_fd);
    if(nodeid==-1){
        cfs_touch(outputfile, curNodeid, block_size);
        nodeid=find_data(curNodeid,outputfile,block_size,open_fd);
    }
    get_node_data(open_fd,nodeid,block_size);
    write(open_fd,buffer,block_size-sizeof(MDS));
    MDS mds;
    get_node_mds(&mds,nodeid,open_fd,block_size);
    mds.modification_time=time(NULL);
    update_node_mds(&mds,nodeid,block_size,open_fd);
    update_size(nodeid,block_size,outsize,open_fd);
}

void cfs_rm(char* args,unsigned int current_nodeid, int block_size, int op){
    if(open_fd==-1){
        printf("Error! No CFS file in use.\n");
        return;
    }
    char* token;
    int option=0;
    token=strtok(args," ");
    if(token != NULL) {
        if (strcmp(token, "-i") == 0) {
            option = 1;
            token = strtok(NULL, " ");
            if (token == NULL) {
                printf("Input Error\n");
            }
        } else if (strcmp(token, "-r") == 0) {
            option = 2;
            token = strtok(NULL, " ");
            if (token == NULL) {
                printf("Input Error\n");
            }
        }else{
            option = op;
        }
    }

    int nodeid = find_data(current_nodeid, token, block_size,open_fd);
    if(nodeid == -1) {
        printf("File %s not found\n",token);
        return;
    }
    if(option == 1){
        printf("Delete? y or n\n");
        char buf[1];
        scanf("%s", buf);
        if(strcmp(buf,"y") == 0){
            MDS mds;
            get_node_mds(&mds, nodeid, open_fd, block_size);
            if(mds.type == 1){
                rm_file(mds, nodeid, current_nodeid, block_size, open_fd);
            }else if(mds.type == 0){
                printf("rm: cannot remove '%s': Is a directory\n", token);
                return;
            }
        }
    } else if(option == 2){
        MDS mds;
        get_node_mds(&mds, nodeid, open_fd, block_size);
        if(mds.type == 1 || mds.size == sizeof(MDS)){
            rm_file(mds, nodeid, current_nodeid, block_size, open_fd);
        }else {
            rec_rm(nodeid, block_size, open_fd);
            rm_file(mds, nodeid, current_nodeid, block_size, open_fd);
        }
    }else{
        MDS mds;
        get_node_mds(&mds, nodeid, open_fd, block_size);
        if(mds.type == 1){
            rm_file(mds, nodeid, current_nodeid, block_size, open_fd);
        }else if(mds.type == 0){
            printf("rm: cannot remove '%s': Is a directory\n", token);
            return;
        }
    }
    token=strtok(NULL,"\n");
    if(token!=NULL){
        cfs_rm(token,current_nodeid, block_size, option);
    }
}

void rm_file(MDS mds, int nodeid, int current_nodeid, int block_size, int fd){
    update_size(mds.parent_nodeid, block_size, -(mds.size+sizeof(MDS)), fd);

    int *empty = malloc(block_size/sizeof(int)* sizeof(int));
    for (int i = 0; i < (block_size/sizeof(int)); ++i) {
        empty[i] = 0;
    }

    lseek(open_fd,(nodeid+1)*block_size,SEEK_SET);
    write(open_fd, empty, block_size);

    rm_nodeid(current_nodeid,nodeid,block_size, open_fd);
    superBlockStruct sb;
    get_superblock(fd, &sb);
    sb.node_counter--;
    set_superblock(fd, &sb);
    new_hole(nodeid, fd);
    free(empty);
}

void new_hole(int nodeid, int fd){
    superBlockStruct sb;
    get_superblock(fd,&sb);
    write_node_index(nodeid, sb.counter, fd);
    sb.counter++;
    set_superblock(fd, &sb);
}

void rec_rm(unsigned int nodeid,int block_size, int fd){
    MDS mds;
    int i;
    int data_id;
    int offset = 1;
    get_node_mds(&mds,nodeid,fd,block_size);
    for(i=0;i<mds.counter;i++) {
        read(fd, &data_id, sizeof(int));
        if (data_id != 0) {
            MDS data_mds;
            get_node_mds(&data_mds,data_id,fd,block_size);
            if (mds.type == 1 || mds.size == 0) {
                rm_file(data_mds, data_id, nodeid, block_size, fd);
            }else{
                rec_rm(data_id, block_size, fd);
            }
        } else {
            i--;
        }
        get_node_data(fd, nodeid, block_size);
        lseek(fd, offset * sizeof(int), SEEK_CUR);
        offset++;
    }
    get_node_mds(&mds,nodeid,fd,block_size);
    rm_file(mds, nodeid,mds.parent_nodeid, block_size, fd);
}

void rm_nodeid(unsigned int nodeid,unsigned int dest_nodeid,int block_size, int fd){
    MDS mds;
    get_node_mds(&mds,nodeid,fd,block_size);
    int i = 0;
    int data_id = 0;
    if(mds.counter==0){
        return;
    }
    int offset=1;
    for(i=0;i<mds.counter;i++){
        read(fd,&data_id,sizeof(int));
        if(data_id!=0){
            if(data_id == dest_nodeid){
                int empty = 0;
                lseek(fd,-1*sizeof(int),SEEK_CUR);
                write(fd, &empty, sizeof(int));
                mds.counter--;
                update_node_mds(&mds, nodeid, block_size, fd);
                break;
            }
        }else{
            i--;
        }
        get_node_data(fd,nodeid,block_size);
        lseek(fd,offset*sizeof(int),SEEK_CUR);
        offset++;
    }
}

void cfs_ln(char *token, int curNodeid, int block_size){
    if(open_fd==-1){
        printf("Error! No CFS file in use.\n");
        return;
    }
    token=strtok(token," ");
    char *sourcefile;
    char *output;
    if(token != NULL) {
        sourcefile = malloc(100 * sizeof(char));
        strcpy(sourcefile, token);
        token = strtok(NULL, " ");
        output = malloc(100 * sizeof(char));
        strcpy(output, token);
        if (token == NULL) {
            printf("Input Error\n");
        }
    }else{
        return;
    }

    unsigned int nodeid = find_data(curNodeid, sourcefile, block_size, open_fd);
    MDS mds;
    get_node_mds(&mds, nodeid, open_fd, block_size);
    if(mds.type == 0){
        printf("cfs_ln: %s: hard link not allowed for directory\n", sourcefile);
    }
    unsigned int outputid = find_data(curNodeid, output, block_size,open_fd);
    if(outputid == -1){
        cfs_touch(output, curNodeid, block_size);
    }else{
        printf("cfs_ln: failed to create hard link '%s': File exists\n", output);
    }
    outputid = find_data(curNodeid, output, block_size,open_fd);
    get_node_mds(&mds, outputid, open_fd, block_size);
    mds.type = 2;
    update_node_mds(&mds, outputid, block_size, open_fd);
    get_node_data(open_fd, outputid, block_size);
    write(open_fd, &nodeid, sizeof(int));
}

void cfs_mv(char *args, int curNodeid, int block_size){
    if(open_fd==-1){
        printf("Error! No CFS file in use.\n");
        return;
    }
    char *token;
    int option=0;
    token=strtok(args," ");
    if(token != NULL) {
        if (strcmp(token, "-i") == 0) {
            option=1;
            token = strtok(NULL, " ");
            if (token == NULL) {
                printf("Input Error\n");
            }
        }
    }
    MDS source_mds;
    int nodeid = find_data(curNodeid, token, block_size, open_fd);
    get_node_mds(&source_mds, nodeid, open_fd,block_size);
    token = strtok(NULL, "\n");
    if (token == NULL) {
        printf("Input Error\n");
        return;
    }
    int check = move_cd(token, &curNodeid,block_size, 1);
    if(check == 1){
        return;
    }

    if(option==1) {
        printf("Move? y or n\n");
        char buf[1];
        scanf("%s", buf);
        if (strcmp(buf, "y") != 0) {
            return;
        }
    }

    rm_nodeid(source_mds.parent_nodeid, nodeid, block_size, open_fd);
    update_size(source_mds.parent_nodeid, block_size, -source_mds.size, open_fd);
    source_mds.parent_nodeid = curNodeid;
    update_node_mds(&source_mds, nodeid, block_size, open_fd);
    MDS dir_mds;
    get_node_mds(&dir_mds, curNodeid, open_fd, block_size);
    //dir_mds.size += source_mds.size;
    //update_node_mds(&dir_mds, curNodeid, block_size, open_fd);
    update_size(dir_mds.nodeid, block_size, source_mds.size, open_fd);
    get_node_data(open_fd, curNodeid, block_size);
    write_node_index(nodeid, dir_mds.counter, open_fd);
}

void cfs_writefile(unsigned int curNodeid, char *fileName, int blockSize) {
    unsigned int nodeid = find_data(curNodeid, fileName, blockSize, open_fd);
    if (nodeid == -1) {
        return;
    }
    char *buff = malloc(blockSize - sizeof(MDS) * sizeof(char));
    getline(&buff, (size_t *) &blockSize, stdin);

    MDS mds;
    get_node_mds(&mds, nodeid, open_fd, blockSize);
    if (mds.size < blockSize) {
        get_node_data(open_fd, nodeid, blockSize);
        lseek(open_fd, mds.size- sizeof(MDS), SEEK_CUR);

        int bytes = write(open_fd, buff, strlen(buff));
        mds.size += bytes;
        update_size(mds.parent_nodeid, blockSize, bytes, open_fd);
        update_node_mds(&mds, nodeid, blockSize, open_fd);
    }
    free(buff);
}


void print_time(time_t time){
    printf("%s",asctime(localtime(&time)));
}

void init_MDS(MDS* mds,unsigned int node_id,unsigned int parent_nodeid,char* filename,unsigned int type){
    mds->nodeid=node_id;
    mds->parent_nodeid=parent_nodeid;
    memset(&mds->filename,0,FILENAME_SIZE);
    strcpy(mds->filename,filename);
    mds->counter=0;
    mds->type=type;
    mds->size=sizeof(MDS);
    mds->creation_time=mds->access_time=mds->modification_time=time(NULL);
}

void get_node_mds(MDS* mds,unsigned int nodeid,int fd,int block_size){
    lseek(fd,(nodeid+1)*block_size,SEEK_SET);
    read(fd,mds,sizeof(MDS));
}
void update_node_mds(MDS* mds,unsigned int nodeid,int block_size,int fd){
    lseek(fd,(nodeid+1)*block_size,SEEK_SET);
    write(fd,mds,sizeof(MDS));
}

void update_size(unsigned int nodeid,int block_size, int size, int fd){
    MDS mds;
    get_node_mds(&mds,nodeid,open_fd,block_size);
    mds.size += size;
    update_node_mds(&mds, nodeid, block_size, fd);
    if(strcmp(mds.filename,"/")!=0){
        update_size(mds.parent_nodeid,block_size, size,fd);
    }
}


void get_node_data(int fd,unsigned int nodeid,int block_size){
    lseek(fd,(nodeid+1)*block_size+sizeof(MDS),SEEK_SET);
}

void write_node_index(unsigned int nodeid,int counter,int fd){
    int data_id;
    for (int j = 0; j <= counter; ++j) {
        read(fd, &data_id, sizeof(int));
        if (data_id == 0) {
            lseek(fd,-1*sizeof(int),SEEK_CUR);
            write(fd, &nodeid, sizeof(int));
            break;
        }else{
            j--;
        }
    }
}

int find_space(int fd){
    int i;
    int data_id;
    superBlockStruct sb;
    get_superblock(fd, &sb);
    if(sb.counter == 0) {
        i = GetNodeCounter(fd);
        return i;
    }else{
        int offset = 1;
        for (int j = 0; j < sb.counter; ++j) {
            read(fd,&data_id,sizeof(int));
            if(data_id!=0){
                int empty = 0;
                lseek(fd, -1* sizeof(int), SEEK_CUR);
                write(fd, &empty, sizeof(int));
                sb.counter--;
                set_superblock(fd, &sb);
                return  data_id;
            }else{
                j--;
            }
            get_superblock_data(fd);
            lseek(fd,offset*sizeof(int),SEEK_CUR);
            offset++;
        }
    }
}

void get_superblock_data(int fd){
    lseek(fd, sizeof(superBlockStruct), SEEK_SET);
}

void get_superblock(int fd,superBlockStruct* superblock){
    lseek(fd,0,SEEK_SET);
    read(fd,superblock,sizeof(superBlockStruct));
}

void set_superblock(int fd,superBlockStruct* superblock){
    lseek(fd,0,SEEK_SET);
    write(fd,superblock,sizeof(superBlockStruct));
}

int GetNodeCounter(int fd){
    lseek(fd,0,SEEK_SET);
    superBlockStruct sb;
    read(fd,&sb,sizeof(superBlockStruct));
    return sb.node_counter;
}

unsigned int find_data(unsigned int nodeid,char*name,int block_size,int fd){
    MDS mds;
    MDS data_mds;
    get_node_mds(&mds,nodeid,fd,block_size);
    get_node_data(fd,nodeid,block_size);
    int data_id;
    int i;
    if(mds.counter==0){
        return -1;
    }
    int offset=1;
    for(i=0;i<mds.counter;i++){
        read(fd,&data_id,sizeof(int));
        if(data_id!=0){
            get_node_mds(&data_mds,data_id,fd,block_size);
            if(strcmp(data_mds.filename,name)==0){
                return data_mds.nodeid;
            }
        }else{
            i--;
        }
        get_node_data(fd,nodeid,block_size);
        lseek(fd,offset*sizeof(int),SEEK_CUR);
        offset++;
    }
    if(i==mds.counter){
        return -1;
    }
}
