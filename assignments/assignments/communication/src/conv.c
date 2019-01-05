#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <assert.h>
#include <ctype.h>
#include <string.h>

#define SERVER "lower"
#define CLIENT "help"
#define MAX 512
#define TEST "This is a TeSt to SEE if we Can cOnvert meSSages To LOweRcASe"

int main(void)  {

    int sock;
    char buffer[MAX];

    /* A socket is created */
    assert((sock = socket(AF_UNIX, SOCK_DGRAM, 0)) != -1);

    struct sockaddr_un client = {AF_UNIX, CLIENT};

    assert(bind(sock, (struct sockaddr *) &client, sizeof(struct sockaddr_un)) != -1);

    /* Send messages, receive reply */

    struct sockaddr_un server = {AF_UNIX, SERVER};
    int size = sizeof(struct sockaddr_un);
    
    int n = sizeof(TEST);

    assert(sendto(sock, TEST, n, 0, (struct sockaddr *) &server, size) == n);

    n = recvfrom(sock, buffer, MAX-1, 0, (struct sockaddr *) &server, &size);

    if (n == -1)
      perror("server");

    buffer[n] = 0;

    printf("Client received: %s\n", buffer);

    unlink(CLIENT);
    
    exit(0);
}
