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

#define CHUNK_SIZE 1024

int main( int argc, char *argv[])
{

int status, SendR,  yes=1, fdmax, newfd, i;
struct addrinfo hints, *res;  // will point to the results
fd_set master;    // master file descriptor list
fd_set read_fds;  // temp file descriptor list for select()
char buf[256];    // buffer for client data
struct sockaddr_storage remoteaddr; // client address
socklen_t addrlen;

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
    read_fds = master; // copy it
    if (select(fdmax+1, &read_fds, NULL, NULL, NULL) == -1) {
        perror("select");
        exit(4);
    }

    // run through the existing connections looking for data to read
    for(i = 0; i <= fdmax; i++) {
        if (FD_ISSET(i, &read_fds)) { // we got one!!
            if (i == SendR) {
                // handle new connections
                addrlen = sizeof remoteaddr;

                recv(i, buf, sizeof buf, 0);
                printf("%s\n", buf);
            }
        }
    }
}
freeaddrinfo(res); // free the linked-list

return 0;
}