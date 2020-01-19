#include<time.h>
#define FILENAME_SIZE 100

typedef struct {
    unsigned int nodeid ;
    char filename[FILENAME_SIZE] ;
    unsigned int size ;
    unsigned int type ;
    unsigned int parent_nodeid ;
    time_t creation_time ;
    time_t access_time ;
    time_t modification_time ;
    //Datastream data ;
} MDS ;

typedef struct{
  char fileType[4];
  int block_size;
  int numOfFiles;
}superBlockStruct;