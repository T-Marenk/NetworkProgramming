#include "inet.h"
#include <netinet/in.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <stdio.h>
#include <strings.h>

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

void echo_cli(int fp_in, int fp_out, int sockfd)
{
        char *sendline = malloc(124);
        char *recvline[124]; // Allocate space for reading lines
        ssize_t nread;

        // Loop, while there are lines left in the given file
        while ((nread = getlin(sendline, fp_in)) != -1) {
                write(sockfd, sendline, nread); // Send the line to the server
                nread = read(sockfd, recvline, 124);
                write(fp_out, recvline, nread);
        }

        close(sockfd);
        close(fp_in);
        close(fp_out);
}

int main(int argc, char *argv[], char *envp[])
{
        int sockfd;
        struct sockaddr_in serv_addr;

        if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0) // Create socket
                handle_error("Failed to create socket");

        // Initialize server address
        bzero((void *) &serv_addr, sizeof(serv_addr));
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
        serv_addr.sin_port = htons(SERV_TCP_PORT);

        // Create connection to server
        if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
                handle_error("Failed to bind local address");

        echo_cli(STDIN_FILENO, STDOUT_FILENO, sockfd);

        return 0;
}

