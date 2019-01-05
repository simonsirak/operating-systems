#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <errno.h>


#define PORT            8080
#define MESSAGE         "Hello"
#define SERVERHOST      "localhost"



void init_sockaddr (struct sockaddr_in *name,
               const char *hostname,
               uint16_t port) {
  struct hostent *hostinfo;

  name->sin_family = AF_INET;
  name->sin_port = htons (port);
  hostinfo = gethostbyname (hostname);
  if (hostinfo == NULL) 
    {
      fprintf (stderr, "Unknown host %s.\n", hostname);
      exit (EXIT_FAILURE);
    }
  name->sin_addr = *(struct in_addr *) hostinfo->h_addr;
}


void write_to_server (int filedes)
{
  int nbytes;

  nbytes = write (filedes, MESSAGE, strlen(MESSAGE) + 1);
  if (nbytes < 0)
    {
      perror ("write");
      exit (EXIT_FAILURE);
    }
}

int
main (void)
{
  int cfd;
  struct sockaddr_in servername;

  /* Create the socket. */
  if((cfd = socket (PF_INET, SOCK_STREAM, 0)) == -1) {
    perror ("socket (client)");
    exit (EXIT_FAILURE);
  }

  /* Connect to the server. */
  init_sockaddr (&servername, SERVERHOST, PORT);
  if (connect(cfd,(struct sockaddr *) &servername, sizeof (servername)) == -1) {
    perror ("connect (client)");
    exit (EXIT_FAILURE);
  }
  /* Send data to the server. */
  if(write (sock, MESSAGE, (strlen(MESSAGE) + 1)) == -1) {
      perror ("write");
      exit (EXIT_FAILURE);
  }
  return 0;
}
