#include "bt.h"
#include <stdio.h>
#include <string.h>

int main(){


FILE * fp;
char data[2048];
long int sz;
fp = fopen("newfile", "rb");
memset(data, 0, sizeof(data));
    
    fseek(fp, 0L, SEEK_END);
    sz = ftell(fp);
    fseek(fp, 0L, SEEK_SET);
//getChunk gets put into a for loop that executes sizeoffile/chunk size times.

getChunk(0, fp, data, sz);
printf("%s\n", data);

return 0;
}