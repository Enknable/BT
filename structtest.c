#include <stdio.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>

struct protocol {
    
    char filename[]; //Chose a reasonable size for filenames and used char array so that they are human readable
    char md5[17]; //Started with a md5[16] array, but the printing of a string of 16 chars acted incorrectly
    char interested; // 0 or 1
    char ack; // 2 or 3
    int8_t data[]; // Not sure if this is the right size or format to send data. Assuming that this will translate to the data being stored in binary format? 
    int64_t request; // 9 Followed by piece # eg. 95463 
    int16_t datasize; // sizeof(data); Not sure if this will return the 65355 value of the data, or the used size(How?)? 
    int32_t length; // filesize in bytes or kB
    int64_t offset;  //Chose a large memory size so that every bit can be specified 
    };

main() {
  int size;
  
  int fd = open("file", O_RDWR);
  if(fd==-1)
        fprintf(stderr, "Error opening 'file'");
  
  //Find the size of the file, calculate the md5 of the file, readn/writen functions, write in predetermined chunks so seq# can be found "missing packet 5000" 5000*(sizeofchunks) = file offset
  
  struct protocol test, readtest;
  
  strcpy(test.filename,"The Size of this file can be up");
  strcpy(test.md5,"1234567890abcdef");
  test.offset=1000;
  
  
  size = write(fd, &test, sizeof(test));
  printf("%i\n",size);
  lseek(fd, 0, SEEK_SET);
  size =read(fd, &readtest, sizeof(test));
  printf("%i\n",size);
  
  printf("%s\n", readtest.filename);
}
