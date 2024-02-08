#include <bits/types/struct_timeval.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/time.h>
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

// Receive data from a client with this function
void receiver(int fd_in, int maxr) {
  // Assign needed variables
  struct timeval start, end;
  char *line = malloc(maxr); // Allocate space for reading lines
  unsigned long long  recv = 0;
  ssize_t nread;

  // Read the first bytes and then start the clock
  nread = read(fd_in, line, maxr);
  recv += nread;
  gettimeofday(&start, NULL);

  // Read from input while there is still something to be read and calculate the amount of bytes read
  while ((nread = read(fd_in, line, maxr)) > 0) recv += nread;
  gettimeofday(&end, NULL); // Stop the timer

  long elapsed = (end.tv_sec - start.tv_sec)*1000000 + end.tv_usec - start.tv_usec;

  printf("Time needed to read the data %ld us\n", elapsed);
  printf("Received %llu bytes\n", recv);
  printf("Throughput is %f bytes/s", (double)recv/((double)elapsed/1000000));

  free(line); // Free the allocated space
  close(fd_in); // Close the socket (child)
}

int main(int argc, char *argv[], char *envp[]) {
  if (argc < 2) {
    my_err("Give max bytes to read");
  }
  int maxr = atoi(argv[1]); // Max bytes that can be read with a single read() function
  int listenfd, newsockfd, clilen, childpid;
  struct sockaddr_in cli_addr, serv_addr;

  if ( (listenfd = socket(PF_INET, SOCK_STREAM, 0)) < 0) // Create the socket
    my_err("Failed to create socket");

  // Initialize the server address
  bzero((void *) &serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  serv_addr.sin_port = htons(SERV_TCP_PORT);

  // Bind the server address to the socket
  if (bind(listenfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
    my_err("Failed to bind address") ;

  // Start the tcp server
  listen(listenfd, 8);
  for (;;) {
    clilen = sizeof(cli_addr); // Client process connected

    newsockfd = accept(listenfd, (struct sockaddr *) &cli_addr, &clilen); // Open a socket for the client

    if (newsockfd < 0) my_err("Failed to create connection");

    if ( (childpid = fork()) < 0)
      my_err("Failed to fork");
    else if (childpid == 0) { // Create child process to handle the data receiving
      receiver(newsockfd, maxr);
      exit(0);
    }
    close(newsockfd); // Close the client socket for the parent
  }
  return 0;
}