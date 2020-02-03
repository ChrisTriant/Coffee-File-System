#ifndef __FUN__
#define __FUN__
#include "struct.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<fcntl.h>
#include<unistd.h>


void init_MDS(MDS* mds,unsigned int node_id,unsigned int parent_nodeid,char* filename,unsigned int type);
void print_time(time_t time);
void get_superblock_data(int fd);
void get_node_mds(MDS* mds,unsigned int nodeid,int fd,int block_size);
void update_node_mds(MDS* mds,unsigned int nodeid,int block_size,int fd);
void get_node_data(int fd,unsigned int nodeid,int block_size);
void write_node_index(unsigned int nodeid,int counter,int fd);
void get_superblock(int fd,superBlockStruct* superblock);
void set_superblock(int fd,superBlockStruct* superblock);
int GetNodeCounter(int fd);
int cmpfilename(const void* mds1 ,const void* mds2);
void update_size(unsigned int nodeid,int block_size, int size, int fd);
void new_hole(int nodeid, int fd);

#endif