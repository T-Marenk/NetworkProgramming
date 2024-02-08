#include <stdlib.h>
#include <strings.h>
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

// Function to read a line from the line
ssize_t getlin(char* line, int fp) {
  int pos = 0; // Indicate how many bytes have been read
  int max = 123;

  // Loop until it is the end of the line or the file ends
  // The loop also ends, if the buffer becomes full
  // The read byte is added to the allocated line array
  while(read(fp, &line[pos], 1)) {
    if (line[pos] == *"\n" || line[pos] == *"\0" || pos == max) {
      return pos+1; // Return the amount of read bytes
    }
    pos++; // Increase the position
  }

  return -1; // Nothing has been read
}

// Function, that doubles each row
void doubler(int fd_in, int fd_out) {
  char *line = malloc(124); // Allocate space for reading lines
  ssize_t nread;

  // Loop, while there are lines being sent
  while ((nread = getlin(line, fd_in)) != -1) {
    write(fd_out, line, nread); // Print the lines to standard output
    write(fd_out, line, nread);
  }

  free(line); // Free the allocated space
  close(fd_in); // Close the socket
}

int main(int argc, char *argv[], char *envp[]) {
  int listenfd, newsockfd, clilen, childpid;
  struct sockaddr_in cli_addr, serv_addr;

  // Create server socket
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

  // Server started
  listen(listenfd, 8);
  for (;;) {
    // Wait until there is a connection to the server and create a socket for that connection/client
    clilen = sizeof(cli_addr);
    newsockfd = accept(listenfd, (struct sockaddr *) &cli_addr, &clilen);

    if (newsockfd < 0) my_err("Failed to create connection");

    if ( (childpid = fork()) < 0) // Create child process to handle the receiving of the data
      my_err("Failed to fork");
    else if (childpid == 0) {
      doubler(newsockfd, STDOUT_FILENO);
      exit(0);
    }
    close(newsockfd); // Close socket for parent
  }
  return 0;
}