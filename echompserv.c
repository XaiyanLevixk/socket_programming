#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<signal.h>
#include<sys/wait.h>
#define BUF_SIZE 30

void error_handling(char *message);
void read_childproc(int sig);

int main(int argc, char *argv[]){

    int serv_sock, clnt_sock;
    struct sockaddr_in serv_adr, clnt_adr;

    pid_t pid;
    struct sigaction act;
    socklen_t adr_sz;
    int str_len, state;
    char buf[BUF_SIZE];
    if(argc!=2){
        printf("Usage : %s <port>\n", argv[0]);
        exit(1);
    }
//clean up zombie process
    act.sa_handler = read_childproc;
    sigemptyset(&act.sa_mask);
    act.sa_flag = 0;
    state = sigaction(SIGCHILD, &act, 0);

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_adr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(atoi(argv[1]));

    if(bind(serv_sock, (struct sockaddr*) &serv_adr, sizeof(serv_adr)) == -1){
        error_handling("bind() error \n");
    } 
    //time log of 5 seconds
    if(listen(serv_sock, 5) ==-1){
        printf("listen() error \n");
    }

    while (1){
        adr_sz = sizeof(clnt_adr);
        clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &adr_sz);
        if(clnt_sock == -1){
            continue;
        }
        else{
            puts("new client connected");
        }
        pid = fork();
        if(pid == -1){
            close(clnt_sock);
            continue;
        }
        if(pid == 0){
            close(serv_sock);
            while ((str_len = read(clnt_sock, buf, BUF_SIZE))!=0){
                write(clnt_sock, buf, str_len);
            }
            close(clnt_sock);
            puts("client disconnected");
            return 0;
        }
        else{
            close(clnt_sock);
        }
    }
    close(serv_sock);
    return 0;
    
}

void error_handling(char *message){
    printf("%s",message);
}

void read_childproc(int sig){
    int status;
    pid_t id = waitpid(-1, &status, WNOHANG);
    if(WIFEXITED(status)){
        printf("removed proc id: %d \n", id);
        printf("child send: %d \n", WEXITSTATUS(status));
    }
}