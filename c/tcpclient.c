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

#define SERVER_PORT 4500

int
main (int argc, char *argv[]){
    struct sockaddr_in client;
    struct hostent *h;
    int sockfd;

    char *message = "Hello world\n";

    if(argc != 2){
        fprintf(stderr, "Use: ./client <server>\n");
        exit(1);
    }
    
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 ){
        perror("socket");
        exit(1);

    }

    if((h = gethostbyname(argv[1])) == NULL){
        perror("gethostbyname");
        exit(1);
    }

    client.sin_family = AF_INET;
    client.sin_port = htons(SERVER_PORT);
    client.sin_addr = *((struct in_addr *)h->h_addr);

    if(connect(sockfd, (struct sockaddr *)&client, sizeof(client)) < 0){
        perror("connect");
        exit(1);
    }

    send(sockfd, message, strlen(message), 0);
    close(sockfd);
    return 0;
}
