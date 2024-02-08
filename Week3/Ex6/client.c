#include <bits/types/struct_timeval.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#define SERV_TCP_PORT 51012
#define ll unsigned long long

void my_err(char *sptr)
{
  perror(sptr); exit(1);
}

// Generate data to be sent to the server
void generator(int fd_out, ll maxb, ll maxw) {
  char *line = malloc(maxw); // Allocate space for the generated line
  ssize_t nread;
  int sent = 0;
  struct timeval start, end;

  memset(line, 'A', maxw); // Set the data to be sent to AAAAA...

  /* Start the sending
   * if the data to be sent is less then the max allowed for one write,
   * sent just one write and stop */
  if (maxw >= maxb) {
    gettimeofday(&start, NULL);
    write(fd_out, line, maxb);
    gettimeofday(&end, NULL);
  }
  else {
    // Start the clock and send the first piece of data
    gettimeofday(&start, NULL);
    write(fd_out, line, maxw);
    sent += maxw;
    while (sent + maxw < maxb) { // Keep sending while there are enough bytes left
      sent += maxw;
      write(fd_out, line, maxw);
    }

    if (sent != maxb) { // If there still are bytes to be sent, send them
      int left = maxb - sent;
      write(fd_out, line, left);
    }
    gettimeofday(&end, NULL);
  }

  long elapsed = (end.tv_sec - start.tv_sec)*1000000 + end.tv_usec - start.tv_usec;

  printf("Time needed to write the data %ld us\n", elapsed);

  close(fd_out);
}

int main(int argc, char *argv[], char *envp[]) {
  int sockfd;
  char *stopstring;
  struct timeval start, end;
  struct sockaddr_in serv_addr;

  if (argc < 4)
    my_err("Usage: <ip_addr> <max_bytes> <bytes_per_send>");
  if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0) // Create the socket to communicate with the server
    my_err("Filed to create socket");

  // Initialize the server address with required data
  bzero((void *) &serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
  serv_addr.sin_port = htons(SERV_TCP_PORT);

  gettimeofday(&start, NULL);
  if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) // Create connection to the server
    my_err("Failed to bind local address");
  gettimeofday(&end, NULL);

  long elapsed = (end.tv_sec - start.tv_sec)*1000000 + end.tv_usec - start.tv_usec;

  printf("Time needed to establish the connection %ld us\n", elapsed);

  ll maxb = strtoull(argv[2], &stopstring, 10); // Max bytes to be sent
  ll maxw = strtoull(argv[3], &stopstring, 10); // Max bytes per write
  generator(sockfd, maxb, maxw);

  return 0;
}