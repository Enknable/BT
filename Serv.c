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
time_t t;
struct md5CTX md;
byte_t digest[MD5_SZ];
byte_t str[2048];
int SDARRAY[100];
int WHOAMI[100];
int ACK[100];
uint32_t SQARRAY[100];
int  j=0, numb_bytes = 0;
uint32_t sqNumb;
long int sz;
FILE * fp;
FILE * fp2;
long int szread=0;
int status, SendR,  yes=1, fdmax, newfd, i,q, count = 0, TempR;
long int sz2 = 0;
struct addrinfo hints, hints2, *res, *res2;  // will point to the results
fd_set master;    // master file descriptor list
fd_set read_fds, write_fds;  // temp file descriptor list for select()
char buf[256];    // buffer for client data
struct sockaddr_storage remoteaddr;
struct sockaddr *remoteaddrudp[100];// client address
socklen_t addrlen, addrlenudp[100];
int byte_count;
char ipstr[INET6_ADDRSTRLEN];
int broadcast = 1;
//char broadcast = '1'; // if that doesn't work, try this
srand((unsigned) time(&t));


memset(&SQARRAY, -1, sizeof(SQARRAY));
memset(&SDARRAY, -1, sizeof(SDARRAY));

fp = fopen("newfile", "rb");
memset(bt.data, 0, sizeof(bt.data));
fseek(fp, 0L, SEEK_END);
sz2 = sz = ftell(fp);
fseek(fp, 0L, SEEK_SET);

fp2 = fopen("newfiletest", "ab");
setbuf(fp2, NULL);
fseek(fp2, 0L, SEEK_SET);


FD_ZERO(&master);    // clear the master and temp sets
FD_ZERO(&read_fds);

memset(&hints, 0, sizeof hints); // make sure the struct is empty
hints.ai_family = AF_UNSPEC;     // don't care IPv4 or IPv6
hints.ai_socktype = SOCK_DGRAM; // UDP datagram sockets
hints.ai_flags = AI_PASSIVE;     // fill in my IP for me

memset(&hints2, 0, sizeof hints2); // make sure the struct is empty
hints2.ai_family = AF_UNSPEC;     // don't care IPv4 or IPv6
hints2.ai_socktype = SOCK_DGRAM; // UDP datagram sockets
hints2.ai_flags = AI_PASSIVE;     // fill in my IP for me

if ((status = getaddrinfo(NULL, "3490", &hints, &res)) != 0) {
    fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
    exit(1);
}

SendR = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
if(SendR == -1)
    fprintf(stderr, "Socket Error: %s\n", strerror(errno));


if(bind(SendR, res->ai_addr, res->ai_addrlen) == -1)
    fprintf(stderr,"Bind Error: %s\n", strerror(errno));



 if (setsockopt(SendR,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int)) == -1) {
    perror("setsockopt");
    exit(1);
} 

//lose the pesky "Address already in use" error message
//if (setsockopt(SendR,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int)) == -1) {
  //  perror("setsockopt");
    //exit(1);
//} 

// add the listener to the master set
    FD_SET(SendR, &master);

// keep track of the biggest file descriptor
    fdmax = SendR; // so far, it's this one

for(;;){
    

    read_fds = master;
    write_fds = master;
    
        if (select(fdmax+1, &read_fds, &write_fds, NULL, NULL) == -1) {
        perror("select");
        exit(4);
    }

    // run through the existing connections looking for data to read
    for(i = 0; i <= fdmax; i++) {
        bt.sqNum=0;
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
                            if ((status = getaddrinfo(ipstr, "5000", &hints2, &res2)) != 0) {
                            fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
                            exit(1);
}               
                    
                    SDARRAY[count]=socket(res2->ai_family, res2->ai_socktype, res2->ai_protocol);
                        if(SDARRAY[count] == -1){
                        fprintf(stderr, "Socket Error: %s\n", strerror(errno));
                        
      //                  if (setsockopt(SDARRAY[count], SOL_SOCKET, SO_BROADCAST, &broadcast,
	//	sizeof broadcast) == -1) {
	//	perror("setsockopt (SO_BROADCAST)");
	//	exit(1);
	//}
	if (setsockopt(SendR,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int)) == -1) {
    perror("setsockopt");
    exit(1);
	}       
	
}else{
    sqNumb=0;
    ACK[SDARRAY[count]] = 1;
    printf("%u jokes\n", bt.sqNum);
    fdmax = SDARRAY[count];
    remoteaddrudp[count] = res2->ai_addr;
    addrlenudp[count] = res2->ai_addrlen;
    //FD_SET(SDARRAY[i], &write_fds);
    SQARRAY[SDARRAY[count]] = sqNumb;
    //printf("%i\n", remoteaddrudp[count]);
    count++;
    
}
                            
                        }
                        
                        
                        
                        
            
            if(bt.sqNum > 0){
                    FD_CLR(bt.WHOAMI, &master);
                            printf("%u nojokes\n", bt.sqNum);
                            if(bt.ack == 1){
                                printf("%i imhere and should be zero\n", bt.WHOAMI);
                            ACK[bt.WHOAMI] = 1;
                            SQARRAY[bt.WHOAMI] = bt.sqNum;
                            printf("%i WHO?\n", SQARRAY[bt.WHOAMI]);
                            sleep(1);
                            
                            }else{
                            printf("%i ack \n", bt.ack);
                            if ((status = getaddrinfo(ipstr, "5000", &hints2, &res2)) != 0) {
                            fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
                            exit(1);
}               
                            
                            SQARRAY[bt.WHOAMI] = bt.sqNum;
                            printf("I'm HERE");
                            ACK[bt.WHOAMI] = bt.ack;
                            //getChunk
                            sendto(bt.WHOAMI, &bt, sizeof bt, 0, res2->ai_addr , res2->ai_addrlen);
                            sleep(1);
                            
                        printf("%u\n", bt.sqNum);
                            }
            }
        }
        }
             
             
             
             if(SDARRAY[i] != -1 && SQARRAY[SDARRAY[i]] !=-1 && ACK[SDARRAY[i]] == 1) 
             if(SQARRAY[SDARRAY[i]] <= sz/CHUNK_SIZE)
                FD_SET(SDARRAY[i], &master);
                
    
    if(FD_ISSET(SDARRAY[i], &write_fds)){
            
            
            //SET WRITE FD IF sqNum SQARRAY[i] is less than FILESIZE/CHUNKSIZE for each FD
            
            memset(bt.data, 0, sizeof(bt.data));
            szread = getChunk(SQARRAY[SDARRAY[i]], fp, bt.data, sz);
            memcpy(&str, bt.data, szread);
            md5Start(&md);
            md5Add(&md, str, sizeof(str));
            md5End(&md, digest);
            memcpy(&bt.md5, digest, sizeof(digest));
            
            //printf("%s\n", bt.data);
            printf("%lu\n", szread);
            fwrite(bt.data , 1 , szread , fp2);
            printf("%lu\n", sz2);
            for (q=0;q<16;q++){
                printf("%02x", bt.md5[q]);
                     }
                     for (q=0;q<16;q++){
                printf("%02x", digest[q]);
                     }
            
            
            //!!!!!!!This needs to be set for each socket!
            sz2 = sz2 - szread;
            printf("%i\n", i);
            //!!!!!!!
            
            //printf("%i\n", remoteaddrudp[count-1]);
        
            j=(rand()%10);
            printf("%i\n", j);
                if(j == 1)
                SQARRAY[SDARRAY[i]]++;
                
                bt.sqNum = SQARRAY[SDARRAY[i]];
                printf("%i IAM\n", SQARRAY[SDARRAY[i]]);
                
                printf("%i sqnum\n", bt.sqNum);
                printf("%i sqnumarray\n", SQARRAY[SDARRAY[i]]);
                
            bt.length = szread;
            bt.sz=sz;    
            bt.WHOAMI = SDARRAY[i];
            byte_count = sendto(SDARRAY[i], &bt, sizeof bt, 0, remoteaddrudp[i], addrlenudp[i]);
            numb_bytes += byte_count - 20;
                inet_ntop(remoteaddr.ss_family,
                        get_in_addr(remoteaddrudp[i]),
                        ipstr, sizeof ipstr);
                        printf("sendto IP address %s\n", ipstr);
                        
            if(byte_count==0)
                fprintf(stderr, "sendto error: %s\n", gai_strerror(byte_count));
                
                SQARRAY[SDARRAY[i]]++;
                    
                    
                
                    printf("%i\n", SQARRAY[SDARRAY[i]]);
                    
                    
                    if(sz2 == 0);
                FD_CLR(SDARRAY[i], &master); //ZERO THIS TO OPEN FOR REPEATS...
    }
        
        
    }
}
freeaddrinfo(res); // free the linked-list
freeaddrinfo(res2);
return 0;
}