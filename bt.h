#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
//Protocol guaranteeing a sequence number, length, md5 checksum and 448 bits for data.

struct port {
    
    int16_t sqNum;
    int16_t length;
    int8_t md5[16];
    int64_t data[7];
    
} __attribute__ ((__packed__)) ;
    
//Open file, go to sqNum*"standard-offput", read into buffer, guarantee read = write, return pointer to the buffer.

char *getChunk(int16_t sqNum, FILE * fp);


