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

int main(){
    int status, RecvR, numbytes;    
    int16_t sqNum = 0; //Max 65535    
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
    if ((numbytes = sendto(RecvR, "Hi", 2, 0,
             res->ai_addr, res->ai_addrlen)) == -1) {
        perror("talker: sendto");
        exit(1);
    }

    
    
    
    
    //select recvfrom/check seq # of received packets
    
    
    
    
    //If non-sequencial open TCP and receive missing chunk
    
    
 
    return 0;
}