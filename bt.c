// getChunk

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CHUNK_SIZE 1024

char * getChunk(int16_t sqNum, FILE * fp, char data[2048]){
    int sz;
    memset(data, 0, sizeof(data));
    fseek(fp, 0L, SEEK_END);
    sz = ftell(fp);
    fseek(fp, 0L, SEEK_SET);
    lseek(fp, sqNum*CHUNK_SIZE, SEEK_SET);
    if(sz < 2048){
        fread(data, sizeof(char), sz, fp);
    }else{
    fread(data, 2048 , 1, fp);
}
    return data;
}