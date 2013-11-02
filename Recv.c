#include <stdio.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <errno.h>
#include "bt.h"
#include "common.h"
#include <inttypes.h>

#define CHUNK_SIZE 2048
#define MD5_SZ (128 / 8)


int main(){
    int status, RecvR, numbytes, SendR, yes=1, numb_bytes, data_bytes = 0;
    struct sockaddr_storage remoteaddr;
    struct md5CTX md;
    int i,q;
    byte_t digest[MD5_SZ];
    byte_t str[2048];
    socklen_t addrlen;
    uint32_t sqNumb = 0; //Max 65535    
    struct port bt;
    FILE * fp;
    int broadcast = 1;
	//char broadcast = '1'; // if that doesn't work, try this
    
    
    fp = fopen("newfile.txt", "ab");
    setbuf(fp, NULL);
    struct addrinfo hints, *res, hints2, *res2;  // will point to the results
    bt.sqNum = sqNumb;
    
    memset(&hints, 0, sizeof hints); // make sure the struct is empty
    hints.ai_family = AF_UNSPEC;     // don't care IPv4 or IPv6
    hints.ai_socktype = SOCK_DGRAM; // UDP datagram sockets
    hints.ai_flags = AI_PASSIVE;     // fill in my IP for me
    
    memset(&hints2, 0, sizeof hints2); // make sure the struct is empty
    hints2.ai_family = AF_UNSPEC;     // don't care IPv4 or IPv6
    hints2.ai_socktype = SOCK_DGRAM; // UDP datagram sockets
    hints2.ai_flags = AI_PASSIVE;     // fill in my IP for me

    if ((status = getaddrinfo("192.168.1.77", "3490", &hints, &res)) != 0) {
        
        fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
        exit(1);
    }

    RecvR = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

    if(RecvR == -1)
        fprintf(stderr, "Socket Error: %s\n", strerror(errno));
    
    if (setsockopt(RecvR,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int)) == -1) {
    perror("setsockopt");
    exit(1);
} 
    
    
    
    //craft header, send with seq 0
    bt.ack=0;
    if ((numbytes = sendto(RecvR, &bt, sizeof bt, 0,
             res->ai_addr, res->ai_addrlen)) == -1) {
        perror("talker: sendto");
        exit(1);
    }

    
    
    
    
    //select recvfrom/check seq # of received packets
    if ((status = getaddrinfo(NULL, "5000", &hints2, &res2)) != 0) {
    fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
    exit(1);
}

SendR = socket(res2->ai_family, res2->ai_socktype, res2->ai_protocol);
if(SendR == -1)
    fprintf(stderr, "Socket Error: %s\n", strerror(errno));


if(bind(SendR, res2->ai_addr, res2->ai_addrlen) == -1)
    fprintf(stderr,"Bind Error: %s\n", strerror(errno));


if (setsockopt(SendR,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int)) == -1) {
    perror("setsockopt");
    exit(1);
}
//if (setsockopt(SendR, SOL_SOCKET, SO_BROADCAST, &broadcast,
//		sizeof broadcast) == -1) {
//		perror("setsockopt (SO_BROADCAST)");
//		exit(1);
//	}

 

    addrlen = sizeof remoteaddr;
    numb_bytes = 0;

for(;;){


    numb_bytes += recvfrom(SendR, &bt, sizeof bt, 0,(struct sockaddr *) &remoteaddr, &addrlen);
    
    memcpy(&str, bt.data, strlen(bt.data));
    md5Start(&md);
    md5Add(&md, str, strlen(str));
    md5End(&md, digest);
    
    for (q=0;q<16;q++){
                printf("%02x", bt.md5[q]);
                     }
                     for (q=0;q<16;q++){
                printf("%02x", digest[q]);
                     }
    

    printf("%i q \n", sqNumb);
    printf("%i z \n", bt.sqNum);
    
    if(bt.sqNum != sqNumb && bt.md5 != digest){
        printf("dropped packet");
        bt.sqNum = sqNumb;
        bt.ack=0;
         if ((numbytes = sendto(RecvR, &bt, sizeof bt, 0,
             res->ai_addr, res->ai_addrlen)) == -1) {
            sleep(5);
        perror("talker: sendto");
        exit(1);
    }
    for(;;){
        numb_bytes=0;
 recvfrom(SendR, &bt, sizeof bt, 0,(struct sockaddr *) &remoteaddr, &addrlen);
    if(bt.sqNum != sqNumb){
        bt.sqNum = sqNumb;
        if ((numbytes = sendto(RecvR, &bt, sizeof bt, 0,
             res->ai_addr, res->ai_addrlen)) == -1) {
        perror("talker: sendto");
        exit(1);
        sleep(10);
    }
        printf("nope");
    continue;
    }else{
        bt.ack = 1;
        bt.sqNum = sqNumb;
        if ((numbytes = sendto(RecvR, &bt, sizeof bt, 0,
             res->ai_addr, res->ai_addrlen)) == -1) {
        perror("talker: sendto");
        exit(1);
        sleep(1);
    }
        printf("%i 1 \n", sqNumb);
        printf("%i 2 \n", bt.sqNum);
        break;
    
    }
    }
    }
    else{
        sqNumb++;
         fprintf(fp, "%s", bt.data);


        //printf("%s", bt.data);
        fwrite(bt.data , 1 , strlen(bt.data) , fp );
      //  data_bytes += numb_bytes - 20;
    //if(bt.sz*1000 <= data_bytes)
    //exit(1);
    //printf("%i 3 \n", numb_bytes);
    }
}    //If non-sequencial open TCP and receive missing chunk
    
freeaddrinfo(res); // free the linked-list
freeaddrinfo(res2);
 
    return 0;
}