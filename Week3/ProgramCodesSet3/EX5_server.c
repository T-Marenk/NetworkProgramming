#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#define SERV_TCP_PORT 51012

void my_err(char *sptr)
{
  perror(sptr);
  exit(1);
}

void receiver(int fd_in, int maxr) {
  char *line = malloc(maxr); // Allocate space for reading data
  ssize_t nread;

  // While there is data being sent, receive it
  while ((nread = read(fd_in, line, maxr)) > 0) continue;

  free(line); // Free the allocated space
  close(fd_in); // Close the socket
}

int main(int argc, char *argv[], char *envp[]) {
  if (argc < 2) {
    my_err("Give max bytes to read");
  }
  int maxr = atoi(argv[1]); // Max bytes per read
  int listenfd, newsockfd, clilen, childpid;
  struct sockaddr_in cli_addr, serv_addr;

  // Create socket for server
  if ( (listenfd = socket(PF_INET, SOCK_STREAM, 0)) < 0)
    my_err("Failed to create socket");

  // Initialize server address
  bzero((void *) &serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  serv_addr.sin_port = htons(SERV_TCP_PORT);

  // Create the server
  if (bind(listenfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
    my_err("Failed to bind address") ;

  // Start the server
  listen(listenfd, 8);
  for (;;) {
    // Wait until there is a connection
    clilen = sizeof(cli_addr);

    // Create socket for connection
    newsockfd = accept(listenfd, (struct sockaddr *) &cli_addr, &clilen);

    if (newsockfd < 0) my_err("Failed to create connection");

    if ( (childpid = fork()) < 0) // Create child process for handling the data
      my_err("Failed to fork");
    else if (childpid == 0) {
      receiver(newsockfd, maxr);
      exit(0);
    }
    close(newsockfd); // Close socket for parent
  }
  return 0;
}