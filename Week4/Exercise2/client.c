#include "inet.h"
#include <netinet/in.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <stdio.h>
#include <strings.h>

char *ipaddr(char *dns)
{
        int                s;
        struct addrinfo    hints;
        struct addrinfo    *response, *rp;
        struct sockaddr_in *sockaddr_ip;

        bzero(&hints, sizeof(hints));
        hints.ai_family = PF_UNSPEC;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_protocol = IPPROTO_TCP;

        s = getaddrinfo(dns, NULL, &hints, &response);

        if (s != 0) {
                fprintf(stderr, "getaddrinfo");
                exit(1);
        }

        sockaddr_ip = (struct sockaddr_in *) response->ai_addr;

        return inet_ntoa(sockaddr_ip->sin_addr);
}

int getport(char *service)
{
        struct servent *response;

        response = getservbyname(service, "tcp");

        return ntohs(response->s_port);
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

void echo_cli(int fp_in, int fp_out, int sockfd)
{
        char *sendline = malloc(124);
        char *recvline[124]; // Allocate space for reading lines
        ssize_t nread;

        // Loop, while there are lines left in the given file
        while ((nread = getlin(sendline, fp_in)) != -1) {
                write(sockfd, sendline, nread); // Send the line to the server
                nread = recvfrom(sockfd, recvline, 124, 0, NULL, NULL);
                write(fp_out, recvline, nread);
        }

        close(sockfd);
        close(fp_in);
        close(fp_out); // Close the socket
}

int main(int argc, char *argv[], char *envp[])
{
        int sockfd, port;
        char *addrstr;
        struct sockaddr_in serv_addr;

        if (argc != 2) {
                fprintf(stderr, "usage: client <server name>");
                exit(1);
        }

        addrstr = ipaddr(argv[1]);

        port = getport("echo");

        if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0) // Create socket
                handle_error("Failed to create socket");

        // Initialize server address
        bzero((void *) &serv_addr, sizeof(serv_addr));
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_addr.s_addr = inet_addr(addrstr);
        serv_addr.sin_port = htons(port);
        // Create connection to server
        if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
                handle_error("Failed to bind local address");

        echo_cli(STDIN_FILENO, STDOUT_FILENO, sockfd);

        return 0;
}

