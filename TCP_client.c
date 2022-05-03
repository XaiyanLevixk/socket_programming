#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<arpa/inet.h>

int main(){
    //variables
    int socketfd;
    struct sockaddr_in serveraddr;

    //socket creation
    socketfd = socket(AF_INET, SOCK_STREAM, 0);
    if(socketfd == -1){
        printf("Failed to create a socket\n");
        exit(1);
    }
    else{
        printf("socket : %d\n", socketfd);
    }   

    //connect
    bzero(&serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serveraddr.sin_port = htons(4444);
    if(connect() != 0){
        printf("Failed to connect\n");
        exit(1);
    }
    else{
        printf("socket : %d\n", socketfd);
    }

    //read
    char buf[200];
    read(socketfd, buf, 200);
    printf("recieved : %s\n", buf);

    //close
    close(socketfd);
    printf("socket closed \n")

    return 0;
}