#include <fcntl.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <strings.h>
#include "inet.h"
#define LINELEN 1024

// Function, that doubles each row
void doubler(int sockfd, struct sockaddr *pcliaddr, int clilen)
{
        int n, len;
        char mesg[LINELEN];

        for(;;){
                /* Receive messages from client
                 * This assumes that the client only sends one line at the time
                 * and that the server only needs to echo it back
                 * Assumes that there is the same 100 bytes limit per each send */
                len = clilen;
                n = recvfrom(sockfd, mesg, LINELEN, 0, pcliaddr, &len);
                if (n<0) handle_error("Error in recvfrom");

                // Copy the line
                memmove(mesg + n, mesg, n);
                // Send to client
                if (sendto(sockfd, mesg, n*2, 0, pcliaddr, len) != n*2)
                        handle_error("Error sending message");
        }
}

int main(int argc, char **argv)
{
        int sockfd;
        struct sockaddr_in servaddr, cliaddr;

        if ((sockfd = socket(PF_INET, SOCK_DGRAM, PF_UNSPEC)) < 0)
                handle_error("Can't create socket, exiting");

        bzero((void *)&servaddr, sizeof(servaddr));
        servaddr.sin_family = AF_INET;
        servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
        servaddr.sin_port = htons(SERV_UDP_PORT);

        if (bind(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) <0 )
                handle_error("Can't bind socket, exiting");
        doubler(sockfd, (struct sockaddr *) &cliaddr, sizeof(cliaddr));
}
