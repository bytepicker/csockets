#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "die.h"

#define MAXPENDING 5

int main(int argc, char * * argv) {
  if (argc != 2) {
    printf("Usage: %s [port]\n", argv[0]);
    exit(1);
  }

  int servPort = atoi(argv[1]);

  int servSock;
  //create socket for incoming connections
  if ((servSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
    DieWithError("socket() failed");

  //make server structure
  struct sockaddr_in servAddress;
  servAddress.sin_family = AF_INET;
  servAddress.sin_port = htons(servPort);
  servAddress.sin_addr.s_addr = htonl(INADDR_ANY);

  //client part
  unsigned int clientLen;
  struct sockaddr_in clientAddress;
  int clientSock;

  //try to bind
  if (bind(servSock, (struct sockaddr * ) & servAddress, sizeof(servAddress)) < 0)
    DieWithError("bind() failed");

  //set socket to listen
  if (listen(servSock, MAXPENDING) < 0)
    DieWithError("listen() failed");

  int recvMsgSize;
  int RCVBUFSIZE = 32;
  char echoBuffer[RCVBUFSIZE];

  //main loop
  while (1) {
    //accept new connections
    clientLen = sizeof(clientAddress);

    if ((clientSock = accept(servSock, (struct sockaddr * ) & clientAddress, & clientLen)) < 0)
      DieWithError("accept() failed");

    if ((recvMsgSize = recv(clientSock, echoBuffer, RCVBUFSIZE, 0)) < 0)
      DieWithError("recv() failed");

    while (recvMsgSize > 0) {

      if (send(clientSock, echoBuffer, recvMsgSize, 0) != recvMsgSize)
        DieWithError("send() failed");

      if ((recvMsgSize = recv(clientSock, echoBuffer, RCVBUFSIZE, 0)) < 0)
        DieWithError("recv() failed");

      printf("client %s sends %s\n", inet_ntoa(clientAddress.sin_addr), echoBuffer);
    }
    close(clientSock);
  }
}
