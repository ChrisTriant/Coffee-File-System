#ifndef __CFS_FUN__
#define __CFS_FUN__
#include "fun.h"
#define PERMS 0644
#define FILENAME_SIZE 100
#define BLOCK_SIZE 512



void cfs_create(char* filename,int block_size);
void cfs_workwith(char *filename,unsigned int* cur_nodeid,int* block_size);
void cfs_mkdir(char* dir_name,unsigned int parent,int block_size);
int find_space(int fd);
void cfs_touch(char* args,unsigned int parent,int block_size);
void cfs_pwd(unsigned int nodeid,int block_size);
unsigned int find_data(unsigned int nodeid,char*name,int block_size,int fd);
void cfs_cd(char* dest_name,unsigned int* current_nodeid,int block_size);
void cfs_ls(unsigned int current_nodeid,int block_size,options* op);
void call_ls(char* args,int current_nodeid,int block_size);

void cfs_rm(char* args,unsigned int current_nodeid, int block_size, int op);
void rm_file(MDS mds, int nodeid, int current_nodeid, int block_size, int fd);

void rm_nodeid(unsigned int nodeid,unsigned int dest_nodeid,int block_size, int fd);
void rec_rm(unsigned int nodeid,int block_size, int fd);

void cfs_writefile(unsigned int curNodeid, char *fileName, int blockSize);
void cfs_cat(char *args, int curNodeid, int block_size);
void cfs_ln(char *token, int curNodeid, int block_size);
void cfs_mv(char *args, int curNodeid, int block_size);
int find_path(char* dest_name,unsigned int* current_nodeid,int block_size);
void cfs_cp(char *args, int curNodeid, int block_size);

#endif