#include "sys/socket.h"
#include "stdio.h"
struct sockaddr = { sa_family_t saFamily };
int server_fd;


if ((server_fd = socket(int domain, int type, int protocol))<0) {
    perror
};