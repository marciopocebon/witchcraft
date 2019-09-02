#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define SERVER_PORT 5500

int main (int argc, char *argv[]){
    struct sockaddr_in remote_addr;
    struct hostent *h;
    int sockfd, numbytes;
    
    char *message = "Hello UDP";

    if(argc != 2){
        fprintf(stderr, "Use: ./client <server>\n");
        exit(1);
    }
    
    if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ){
        perror("socket");
        exit(1);

    }

    if((h = gethostbyname(argv[1])) == NULL){
        perror("gethostbyname");
        exit(1);
    }

    remote_addr.sin_family = AF_INET;
    remote_addr.sin_port = htons(SERVER_PORT);
    remote_addr.sin_addr = *((struct in_addr *)h->h_addr);

    if((numbytes = sendto(sockfd, message, strlen(message), 0, (struct sockaddr *)&remote_addr, sizeof(struct sockaddr))) < 0){
        fprintf(stdout, "Send: %d bytes to %s\n", numbytes, inet_ntoa(remote_addr.sin_addr));
        close(sockfd);
    }

    return 0;
}
