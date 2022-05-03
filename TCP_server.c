#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<arpa/inet.h>


int main(){
    
    //variables
    int serversocketfd, clientsocketfd;
    struct sockaddr_in, serveraddr, clientaddr;
    int clientaddrlen;

    //socket creation
    serversocketfd = socket(AF_INET, SOCK_STREAM, 0);
    if(serversocketfd == -1){
        printf("Failed to create a socket\n");
        exit(1);
    }
    else{
        printf("socket : %d\n", serversocketfd);
    }

    //binding socket to address
    bzero(&serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons(4444);
    if(bind(serversocketfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr)) != 0){
        printf("Failed to bind\n");
        exit(1);
    }
    else{
        printf("bound\n");
    }

    //listening
        //1 indicates backlog i.e, no of clients 
    if(listen(serversocketfd, 1) != 0){
        printf("Failed to listen to clients\n");
        exit(1);
    }
    else{
        printf("Listening...\n");
    }

    //accepting
    bzero(&clientaddr, sizeof(clientaddr));
    clientsocketfd = accept(serversocketfd, (struct sockaddr*)&clientaddr, &clientaddrlen);
    if(clientsocketfd == -1){
        printf("Failed to accept client request\n");
    }
    else{
        printf("accepted\n");
    }

    //writing (communication)
    char* msg = "Saiyan Levick";
    write(clientsocketfd, msg, strlen(msg));
    printf("Sent\n");

    //close
    close(clientsocketfd);
    printf("Client socket closed \n");
    close(serversocketfd);
    printf("server socket closed \n");

    return 0;
}