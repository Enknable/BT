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
#include <inttypes.h>

int main(){
    int status, RecvR, numbytes, SendR, yes=1, numb_bytes;
    struct sockaddr_storage remoteaddr;
    socklen_t addrlen;
    uint16_t sqNumb = 0; //Max 65535    
    struct port bt;
    FILE * fp;
    
    struct addrinfo hints, *res;  // will point to the results
    bt.sqNum = sqNumb;
    
    memset(&hints, 0, sizeof hints); // make sure the struct is empty
    hints.ai_family = AF_UNSPEC;     // don't care IPv4 or IPv6
    hints.ai_socktype = SOCK_DGRAM; // UDP datagram sockets
    hints.ai_flags = AI_PASSIVE;     // fill in my IP for me

    if ((status = getaddrinfo("192.168.1.77", "3490", &hints, &res)) != 0) {
        
        fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
        exit(1);
    }

    RecvR = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

    if(RecvR == -1)
        fprintf(stderr, "Socket Error: %s\n", strerror(errno));
    
    
    //craft header, send with seq 0
    if ((numbytes = sendto(RecvR, &bt, sizeof bt, 0,
             res->ai_addr, res->ai_addrlen)) == -1) {
        perror("talker: sendto");
        exit(1);
    }

    
    bt.ack=0;
    
    
    //select recvfrom/check seq # of received packets
    if ((status = getaddrinfo(NULL, "5000", &hints, &res)) != 0) {
    fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
    exit(1);
}

SendR = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
if(SendR == -1)
    fprintf(stderr, "Socket Error: %s\n", strerror(errno));


if(bind(SendR, res->ai_addr, res->ai_addrlen) == -1)
    fprintf(stderr,"Bind Error: %s\n", strerror(errno));



//lose the pesky "Address already in use" error message
if (setsockopt(SendR,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int)) == -1) {
    perror("setsockopt");
    exit(1);
} 

    addrlen = sizeof remoteaddr;
    numb_bytes = 0;

for(;;){

    numb_bytes += recvfrom(SendR, &bt, sizeof bt, 0,(struct sockaddr *) &remoteaddr, &addrlen);
    
    
    if(bt.sqNum != sqNumb){
        printf("dropped packet");
        bt.sqNum = sqNumb;
        bt.ack=0;
         if ((numbytes = sendto(RecvR, &bt, sizeof bt, 0,
             res->ai_addr, res->ai_addrlen)) == -1) {
        perror("talker: sendto");
        exit(1);
    }
    for(;;){
        numb_bytes=0;
numb_bytes += recvfrom(SendR, &bt, sizeof bt, 0,(struct sockaddr *) &remoteaddr, &addrlen);
    if(bt.sqNum != sqNumb){
        printf("nope");
    continue;
    }else{
        bt.ack = 1;
        if ((numbytes = sendto(RecvR, &bt, sizeof bt, 0,
             res->ai_addr, res->ai_addrlen)) == -1) {
        perror("talker: sendto");
        exit(1);
    }
        
        printf("%i 1 \n", sqNumb);
        printf("%i 2 \n", bt.sqNum);
        break;
    
    }
    }
    }
    else{
    printf("%i 3 \n", numb_bytes);
    }
}    //If non-sequencial open TCP and receive missing chunk
    

 
    return 0;
}