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
    int status, RecvR, numbytes;    
    uint16_t sqNum = 0; //Max 65535    
    struct port bt;
    struct addrinfo hints, *res;  // will point to the results
    bt.sqNum = sqNum;
    
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

    for(;;){

    numb_bytes = recvfrom(SendR, &bt, sizeof bt, 0,(struct sockaddr *) &remoteaddr, &addrlen);
    
    printf("%lu\n", bt.sqNum);
    
    };
    //If non-sequencial open TCP and receive missing chunk
    
    
 
    return 0;
}