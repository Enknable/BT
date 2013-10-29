// getChunk

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#define CHUNK_SIZE 1

char * getChunk(uint16_t sqNum, FILE * fp, char data[2048], long int sz){
    
    memset(data, 0, sizeof(data));
    
    lseek(fp, sqNum*CHUNK_SIZE, SEEK_SET);
    
    
    if(sz < 2048){
        fread(data, sizeof(char), sz, fp);
    }
    
    else{
        fread(data, CHUNK_SIZE , 1, fp);
    }


    return data;


}