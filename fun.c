#include "fun.h"


int cmpfilename(const void* mds1 ,const void* mds2){
    MDS** temp1,**temp2;
    temp1=(MDS**)mds1;
    temp2=(MDS**)mds2;
    return strcmp((*temp1)->filename,(*temp2)->filename);
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

void new_hole(int nodeid, int fd){
    superBlockStruct sb;
    get_superblock(fd,&sb);
    write_node_index(nodeid, sb.counter, fd);
    sb.counter++;
    set_superblock(fd, &sb);
}