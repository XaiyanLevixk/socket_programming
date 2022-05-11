#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<signal.h>
#include<sys/wait.h>

void read_status(int x){
    int status;
    pid_t pid = waitpid(-1, &status, WNOHANG);
    if(WIFEXITED(status)){
        printf("child process pid: %d terminated with status code: %d \n", pid, WEXITSTATUS(status));
    }

}

void handle_child_process(int *fds, int exitcode){
    //read msg from fds
    //when . is in msg terminate with exitcode
    char c;
    while(read(fds[0], &c,1)>0 && c!='.'){
        printf("%c",c);
    }
    exit(exitcode);
}

int main(){
    struct sigaction sa;
    sa.sa_handler = read_status;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGCHILD, &sa, 0);

    //1st process+ipc
    int fds1[2];
    pipe(fds);
    pid_t ch1 = fork();
    if (ch1 == 0){
        handle_child_process(fds1, 32);
    }
    else{
        //2nd child+ipc
        int fds2[2];
        pipe(fds2);
        pid_t ch2 = fork();
        if(ch2 == 0){
            //child scope
            handle_child_process(fds2, 69);
        }
        else{
            //parent scope
            char* msg = "hi to child 1.";
            write(fds1[1], msg, strlen(msg));

            msg = "hi to child 2.";
            write(fds2[1], msg, strlen(msg));

            //zombie
            sleep(5);

        }
    }
}