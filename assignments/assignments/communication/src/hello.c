#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <assert.h>


#define MAX 512
#define TEST "Hello"

#define SERVER_IP "127.0.0.1"
//#define SERVER_IP "192.168.1.65"
//#define SERVER_IP "130.237.215.140"

#define SERVER_PORT 8080

int main(void)  {

    int sock;
    char buffer[MAX];

    /* A socket is created */
    assert((sock = socket(AF_INET, SOCK_DGRAM, 0)) != -1);

    struct sockaddr_in name;

    name.sin_family = AF_INET;
    name.sin_port = 0;
    name.sin_addr.s_addr = htonl(INADDR_ANY);

    assert(bind(sock, (struct sockaddr *) &name, sizeof(name)) != -1);    

    /* Send messages, receive reply */

    struct sockaddr_in server;

    server.sin_family = AF_INET;
    server.sin_port = htons(SERVER_PORT);
    server.sin_addr.s_addr = inet_addr(SERVER_IP);

    int size = sizeof(struct sockaddr_in);
    
    int n = sizeof(TEST);

    assert(sendto(sock, TEST, n, 0, (struct sockaddr *) &server, size) == n);

    n = recvfrom(sock, buffer, MAX-1, 0, (struct sockaddr *) &server, &size);

    if (n == -1)
      perror("server");

    buffer[n] = 0;

    printf("Client received: %s\n", buffer);

    exit(0);
}
