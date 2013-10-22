#include "bt.h"
#include <stdio.h>

int main(){


FILE * fp;
char data[2048];
fp = fopen("newfile", "rb");

getChunk(0, fp, data);
printf("%s\n", data);

return 0;
}