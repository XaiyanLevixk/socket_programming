#include<stdio.h>
#include<string.h>
#include<arpa/inet.h>

int main(int argc, char *argv[]){

    unsigned short host_port = 0x1234;
    unsigned short net_port;
    unsigned long host_address = 0x12345678;
    unsigned long net_address;

    net_port = htons(host_port);
    net_address = htonl(host_address);

    printf("Host ordered port is %#x \n", host_port);
    
    return 0;
}