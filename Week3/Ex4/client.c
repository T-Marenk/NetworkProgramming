#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#define SERV_TCP_PORT 51012

void my_err(char *sptr)
{
  perror(sptr); exit(1);
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

void reader(int fd_in, int fd_out) {
  char *line = malloc(124); // Allocate space for reading lines
  ssize_t nread;

  // Loop, while there are lines left in the given file
  while ((nread = getlin(line, fd_in)) != -1) {
    if (nread <= 100) // Check that file is not too big
      write(fd_out, line, nread); // Send the line to the server
  }

  close(fd_out); // Close the socket
}

int main(int argc, char *argv[], char *envp[]) {
  int sockfd;
  struct sockaddr_in serv_addr;

  if (argc < 2)
    my_err("No server address given.");
  if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0) // Create socket
    my_err("Filed to create socket");

  // Initialize server address
  bzero((void *) &serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
  serv_addr.sin_port = htons(SERV_TCP_PORT);
  // Create connection to server
  if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    my_err("Failed to bind local address");

  reader(STDIN_FILENO, sockfd);

  return 0;
}