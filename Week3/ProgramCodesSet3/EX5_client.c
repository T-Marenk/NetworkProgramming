#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

void generator(int fd_out, int maxb, int maxw) {
  char *line = malloc(maxw); // Allocate space for the line to be sent
  ssize_t nread;
  int sent = 0;

  memset(line, 'A', maxw); // The line to be sent is AAAA...

  // Loop while there still are bytes to be sent
  while (sent + maxw < maxb) {
    sent += maxw;
    write(fd_out, line, maxw);
  }

  if (sent != maxb) { // If there are bytes left to be sent, send them
    int left = maxb - sent;
    write(fd_out, line, left);
  }

  close(fd_out);
}

int main(int argc, char *argv[], char *envp[]) {
  int sockfd;
  struct sockaddr_in serv_addr;

  if (argc < 4)
    my_err("Usage: <ip_addr> <max_bytes> <bytes_per_send>");
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

  generator(sockfd, atoi(argv[2]), atoi(argv[3]));

  return 0;
}