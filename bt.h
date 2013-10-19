#include <inttypes.h>
//Protocol guaranteeing a sequence number, length, md5 checksum and 448 bits for data.

struct prot {
    
    int16_t sqNum;
    int16_t length;
    int8_t md5[16];
    int64_t data[7];
    
} __attribute__ ((__packed__)) ;
    