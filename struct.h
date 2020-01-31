#include<time.h>
#define FILENAME_SIZE 100

typedef struct {
    unsigned int nodeid ;
    unsigned int parent_nodeid ;
    char filename[FILENAME_SIZE] ;
    unsigned int size ;
    unsigned int type ;
    int counter;
    time_t creation_time ;
    time_t access_time ;
    time_t modification_time ;
    //Datastream data ;
} MDS ;

typedef struct{
  char fileType[4];
  int block_size;
  int numOfFiles;
  int counter;
  int node_counter;
}superBlockStruct;


typedef struct{
    int a_option;
    int r_option;
    int l_option;
    int u_option;
    int d_option;
    int h_option;
}options;