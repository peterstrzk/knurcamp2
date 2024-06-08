#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <errno.h>

const char* STATIC_RESPONSE =
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html; charset=UTF-8\r\n"
        "Content-Length: 155\r\n"
        "Server: KNURCAMP SERVER 8181\r\n"
        "Accept-Ranges: bytes\r\n"
        "Connection: close\r\n"
        "\r\n"
        "<html>\r\n"
        "<html>\r\n"
        "  <head>\r\n"
        "    <title>An Example Page</title>\r\n"
        "  </head>\r\n"
        "  <body>\r\n"
        "    <p>Hello World, this is a very simple HTML document.</p>\r\n"
        "  </body>\r\n"
        "</html>";

const char* STATIC_RESPONSE_SUCHA_KREWETA =
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html; charset=UTF-8\r\n"
        "Server: KNURCAMP SERVER 8181\r\n"
        "Accept-Ranges: bytes\r\n"
        "Connection: close\r\n"
        "\r\n"
        "<html>\r\n"
        "  <head>\r\n"
        "    <title>Suchy Kreweta</title>\r\n"
        "  </head>\r\n"
        "  <body>\r\n"
        "    <p>Hello this is suchy kreweta.</p>\r\n"
        "  </body>\r\n"
        "</html>";

#define INPUT_BUFFER_SIZE 4096
#define MAX_EVENTS 1024
//użyłem jak cos epolla, bo nie zrozumiałem czym był select xDD
void findPath(const char* request, char* target) {
    while (*(request)++ != ' ');
    while (*request != ' ') {
        *(target)++ = *(request)++;
    }
    *target = 0x00;
}

const char* pseudoRouter(const char* requestedPath) {
    if (strcmp("/suchyKreweta", requestedPath) == 0) {
        return STATIC_RESPONSE_SUCHA_KREWETA;
    } else if (strcmp("/vanish", requestedPath) == 0) {
        return NULL;
    } else {
        return STATIC_RESPONSE;
    }
}

int set_nonblocking(int fd) {
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1) return -1;
    return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

void handle_error(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

int main(void) {
    char inputBuffer[INPUT_BUFFER_SIZE + 1];
    int server = socket(AF_INET, SOCK_STREAM, 0);
    if (server == -1) handle_error("socket");

    if (set_nonblocking(server) == -1) handle_error("fcntl");

    struct sockaddr_in sai;
    sai.sin_addr.s_addr = inet_addr("127.0.0.1");
    sai.sin_family = AF_INET;
    sai.sin_port = htons(8181);
    memset(sai.sin_zero, 0, 8);

    if (bind(server, (struct sockaddr*)&sai, sizeof(sai)) != 0) handle_error("bind");
    if (listen(server, 1024) != 0) handle_error("listen");

    int epoll_fd = epoll_create1(0);
    if (epoll_fd == -1) handle_error("epoll_create1");

    struct epoll_event event;
    struct epoll_event events[MAX_EVENTS];

    event.data.fd = server;
    event.events = EPOLLIN;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server, &event) == -1) handle_error("epoll_ctl");

    while (1) {
        int num_fds = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
        if (num_fds == -1) {
            if (errno == EINTR) continue;
            handle_error("epoll_wait");
        }

        for (int i = 0; i < num_fds; ++i) {
            if (events[i].data.fd == server) {
                struct sockaddr_in clientData;
                socklen_t size = sizeof(clientData);
                int client = accept(server, (struct sockaddr*)&clientData, &size);
                if (client == -1) {
                    perror("accept");
                } else {
                    printf("Accepted connection from %s:%d\n",
                           inet_ntop(AF_INET, &clientData.sin_addr.s_addr, inputBuffer, 16),
                           ntohs(clientData.sin_port));

                    if (set_nonblocking(client) == -1) {
                        perror("fcntl");
                        close(client);
                    } else {
                        event.data.fd = client;
                        event.events = EPOLLIN;
                        if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client, &event) == -1) {
                            perror("epoll_ctl");
                            close(client);
                        }
                    }
                }
            } else {
                ssize_t received = recv(events[i].data.fd, inputBuffer, INPUT_BUFFER_SIZE, 0);
                if (received <= 0) {
                    close(events[i].data.fd);
                } else {
                    inputBuffer[received] = 0x00;
                    char pathBuffer[1024];
                    findPath(inputBuffer, pathBuffer);
                    printf("Requested path: %s\n", pathBuffer);

                    const char* response = pseudoRouter(pathBuffer);
                    if (response == NULL) {
                        shutdown(events[i].data.fd, SHUT_RDWR);
                        close(events[i].data.fd);
                    } else {
                        ssize_t sent = send(events[i].data.fd, response, strlen(response), 0);
                        shutdown(events[i].data.fd, SHUT_RDWR);
                        close(events[i].data.fd);
                        printf("Sent %zu bytes to client and closed connection.\n", sent);
                    }
                }
            }
        }
    }

    close(server);
    return 0;
}
