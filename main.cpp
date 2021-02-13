#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
//#include <sys/socket.h>

void error_handling(char *message){
    fputs(message,stderr);
    fputs("\n",stderr);
    exit(1);
}



int main(int argc, char *argv[]) {
    int sd;
    struct sockaddr_in serv_addr;
    int str_len;

    char message[]="block test message";
    if(argc != 3){
        printf("Usage : %s <IP> <port>\n",argv[0]);
        exit(1);
    }

    sd = socket(PF_INET,SOCK_STREAM,0);
    if(sd == -1){
        error_handling("socket() error");
    }
}

