#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include "inet.h"
#define LINELEN 512

// Function to read a line from the line
ssize_t getlin(char* line, int fp, int max) {
        int pos = 0; // Indicate how many bytes have been read

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

void client(int fd_in, int fd_out, int sockfd, const struct sockaddr *pservaddr, int servlen) {
        char sendline[LINELEN], recvline[LINELEN * 2]; // Allocate space for reading lines
        ssize_t nread;

        // Loop, while there are lines left in the given file
        while ((nread = getlin(sendline, fd_in, LINELEN)) != -1) {
                if (nread <= 100) { // Check that file is not too big
                        if (sendto(sockfd, sendline, nread, 0, pservaddr, servlen) != nread) {
                                handle_error("Client error: socket sendto error");
                        }
                        nread = recvfrom(sockfd, recvline, LINELEN * 2, 0, NULL, NULL);
                        if (nread < 0) handle_error("Client error: error while receiving");
                        recvline[nread] = 0;
                        write(fd_out, recvline, nread); // Send the line to the server
                }
        }

        close(fd_out); // Close the socket
}

int main(int argc, char *argv[], char *envp[]) {
        int sockfd;
        struct sockaddr_in servaddr;

        if (argc != 2) {
                fprintf(stderr, "usage: client <server IP>"); 
                exit(1);
        }
        bzero((void *)&servaddr, sizeof(servaddr));
        servaddr.sin_family = AF_INET;
        servaddr.sin_port = htons(SERV_UDP_PORT);
        if ( inet_aton(argv[1], &servaddr.sin_addr) == 0) {
                fprintf(stderr, "Client error: invalid server IP %s\n", argv[1]);
                exit(1);
        }
        if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
                handle_error("Client socket error");
        client(STDIN_FILENO, STDOUT_FILENO, sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr));
}
