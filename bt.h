#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
//Protocol guaranteeing a sequence number, length, md5 checksum and 448 bits for data.

struct port {
    
    uint16_t sqNum;
    uint16_t length;
    char md5[16];
    char data[2048];
    int WHOAMI;
    int ack;
    
    port(): ack(0);
    
} __attribute__ ((__packed__)) ;
    
//Open file, go to sqNum*"standard-offset", read into buffer, guarantee read = write, return pointer to the buffer.


char * getChunk(int16_t sqNum, FILE * fp, char data[2048], long int sz);

