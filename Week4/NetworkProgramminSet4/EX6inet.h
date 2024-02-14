/* inet.h
* Definitions for clients and servers
*/
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#define SERV_TCP_PORT 51069

void handle_error(char *msg)
{
  perror(msg);
  exit(1);
}
