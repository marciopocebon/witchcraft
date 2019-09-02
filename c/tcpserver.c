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
#define SERVER_BACKLOG 10

int
main (int argc, char *argv[]){
    struct sockaddr_in client;
    struct sockaddr_in server;

    int sockfd_server, sockfd_client, nbytes;
    int size, visits = 0;
    char msg[1024];

    if((sockfd_server = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        fprintf(stderr, "could not create socket\n");
        exit(1);
    }

    //clean struct sockaddr_in
    memset(&server, 0, sizeof(struct sockaddr_in));

    server.sin_family = AF_INET;
    server.sin_port = htons(SERVER_PORT);
    server.sin_addr.s_addr = INADDR_ANY;

    if(bind(sockfd_server, (struct sockaddr *)&server, sizeof(struct sockaddr)) < 0){
        fprintf(stderr, "could not create bind\n");
        exit(1);
    }

    if(listen(sockfd_server, SERVER_BACKLOG)){
        fprintf(stderr, "could not create listen\n");
        exit(1);
    }

    while(1){
        size = sizeof(struct sockaddr_in);

        if((sockfd_client = accept(sockfd_server, (struct sockaddr *)&client, &size)) < 0){
            fprintf(stderr, "could not create accept\n");
            exit(1);
        }
        
        visits++;
        fprintf(stderr, "Connection [%d]\n", visits);
        memset(msg, 0, sizeof(msg));

        if((nbytes = recv(sockfd_client, msg, 50, 0)) < 0){
            fprintf(stderr, "clould not receveid message\n");
            exit(1);
        }

        msg[nbytes] = '\0';

        fprintf(stdout, "Message: %s\n", msg);
        close(sockfd_client);
    }

    close(sockfd_server);
    return 0;
}
