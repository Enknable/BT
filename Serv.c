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


// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}


int main( int argc, char *argv[])
{

struct port bt;
int SDARRAY[100];
int status, SendR,  yes=1, fdmax, newfd, i, count = 0;
struct addrinfo hints, hints2, *res, *res2;  // will point to the results
fd_set master;    // master file descriptor list
fd_set read_fds, write_fds;  // temp file descriptor list for select()
char buf[256];    // buffer for client data
struct sockaddr_storage remoteaddr;
struct sockaddr * remoteaddrudp[100];// client address
socklen_t addrlen, addrlenudp[100];
int byte_count;
char ipstr[INET6_ADDRSTRLEN];

FD_ZERO(&master);    // clear the master and temp sets
FD_ZERO(&read_fds);

memset(&hints, 0, sizeof hints); // make sure the struct is empty
hints.ai_family = AF_UNSPEC;     // don't care IPv4 or IPv6
hints.ai_socktype = SOCK_DGRAM; // UDP datagram sockets
hints.ai_flags = AI_PASSIVE;     // fill in my IP for me

if ((status = getaddrinfo(NULL, "3490", &hints, &res)) != 0) {
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

// add the listener to the master set
    FD_SET(SendR, &master);

// keep track of the biggest file descriptor
    fdmax = SendR; // so far, it's this one

for(;;){
    read_fds = master;
        if (select(fdmax+1, &read_fds, &write_fds, NULL, NULL) == -1) {
        perror("select");
        exit(4);
    }

    // run through the existing connections looking for data to read
    for(i = 0; i <= fdmax; i++) {
        if (FD_ISSET(i, &read_fds)) { // we got one!!
            if (i == SendR) {
                // handle new connections
                addrlen = sizeof remoteaddr;
                byte_count = recvfrom(SendR, &bt, sizeof bt, 0,(struct sockaddr *) &remoteaddr, &addrlen);

                printf("recv()'d %d bytes of data in buf\n", byte_count); 
                //This is where the getchunk functions goes. recvfrom -> check struct (App header), if seq#=0 create new sd(socket/descriptor) and fire packets out else create
                //a TCP connection and send missing chunk.
                    inet_ntop(remoteaddr.ss_family,
                        get_in_addr((struct sockaddr*)&remoteaddr),
                        ipstr, sizeof ipstr);
                        printf("from IP address %s\n", ipstr);
                        
                        //Open file, get size, place pointer
                        //check for seq #: If 0 take file size /chunksize = total packets to completion for(totalpackets > 0){sendto(totalpackets, recvR, protocol struct)
                        //create new socket, add to master list
                        
                        if(bt.sqNum == 0){
                            if ((status = getaddrinfo("192.168.1.73", "5000", &hints2, &res2)) != 0) {
                            fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
                            exit(1);
}               
                    SDARRAY[count]=socket(res2->ai_family, res2->ai_socktype, res2->ai_protocol);
                        if(SDARRAY[count] == -1){
                        fprintf(stderr, "Socket Error: %s\n", strerror(errno));
                
}else{
    
    printf("%u\n", bt.sqNum);
    fdmax = SDARRAY[count];
    remoteaddrudp[count] = (struct sockaddr *) &remoteaddr;
    addrlenudp[count] = sizeof remoteaddr;
    FD_SET(SDARRAY[count], &write_fds);
    //printf("%i\n", remoteaddrudp[count]);
    count++;
    
}
                            
                        }
                        
                        //if seq# =! 0: send packet again
                        
                        
                        printf("%u\n", bt.sqNum);
            }
        }
            write_fds = master;
            
    if(FD_ISSET(i, &write_fds)){
            
            //printf("%i\n", remoteaddrudp[count-1]);
            byte_count = sendto(SDARRAY[count-1], "HI", 2, 0,  remoteaddrudp[count-1], addrlenudp[count-1]);
            printf("%u is set\n", bt.sqNum);
            if(byte_count==-1)
                fprintf(stderr, "sendto error: %s\n", gai_strerror(byte_count));
        
        }
        
    }
}
//freeaddrinfo(res); // free the linked-list

return 0;
}