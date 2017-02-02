#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include "die.h"

#define RCVBUFSIZE 32

int main(int argc, char** argv){
    if(argc != 4){
       printf("Usage: %s [server] [port] [message]\n", argv[0]);
        exit(1);
    }

    char* servIP = (char*)argv[1];
    int servPort = atoi(argv[2]);
    char* msg = (char*)argv[3];


    int clientSock;
    //create stream socket using TCP
    if((clientSock=socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0){
        DieWithError("socket() failed");
    }

    //make server structure
    struct sockaddr_in echoServAddress;
    echoServAddress.sin_family = AF_INET;   // IPv4
    echoServAddress.sin_addr.s_addr = inet_addr(servIP);
    echoServAddress.sin_port = htons(servPort);
    if(connect(clientSock, (struct sockaddr *) &echoServAddress, sizeof(echoServAddress)) < 0){
        DieWithError("connect() failed");
    }

    //send message to server
    int msgLen = strlen(msg);
    char echoBuffer[RCVBUFSIZE];

    if(send(clientSock, msg, msgLen, 0) != msgLen){
        DieWithError("send() sent a wrong number of bytes");
    }

    int totalBytesRcvd, bytesRcvd;
    totalBytesRcvd= 0;

    printf("received: ");
    while (totalBytesRcvd < msgLen) {
        if ((bytesRcvd = recv(clientSock, echoBuffer, RCVBUFSIZE - 1, 0)) <= 0) {
            DieWithError("recv() failed or connection closed prematurely");
        }

        totalBytesRcvd += bytesRcvd;
        echoBuffer[bytesRcvd] = '\0';
        printf("%s\n", echoBuffer);
      }

    close(clientSock);
    exit(0);
}
