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

#define INPUT_BUFFER_SIZE 4096
#define MAX_EVENTS 1024

const char* STATIC_RESPONSE =
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html; charset=UTF-8\r\n"
        "Content-Length: 155\r\n"
        "Server: KNURCAMP SERVER 8181\r\n"
        "Accept-Ranges: bytes\r\n"
        "Connection: close\r\n"
        "\r\n"
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

void findPath(const char* request, char* target) {
    while (*(request)++ != ' ');
    while (*request != ' ' && *request != '\0') {
        *(target)++ = *(request)++;
    }
    *target = '\0';
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
    char ipBuffer[INET_ADDRSTRLEN];
    char pathBuffer[1024];

    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) handle_error("socket");

    if (set_nonblocking(server_fd) == -1) handle_error("fcntl");

    struct sockaddr_in server_addr = {
            .sin_family = AF_INET,
            .sin_port = htons(8181),
            .sin_addr.s_addr = inet_addr("127.0.0.1")
    };

    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) != 0) handle_error("bind");
    if (listen(server_fd, 1024) != 0) handle_error("listen");

    int epoll_fd = epoll_create1(0);
    if (epoll_fd == -1) handle_error("epoll_create1");

    struct epoll_event event = {
            .data.fd = server_fd,
            .events = EPOLLIN
    };

    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_fd, &event) == -1) handle_error("epoll_ctl");

    while (1) {
        struct epoll_event events[MAX_EVENTS];
        int num_fds = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
        if (num_fds == -1) {
            if (errno == EINTR) continue;
            handle_error("epoll_wait");
        }

        for (int i = 0; i < num_fds; ++i) {
            if (events[i].data.fd == server_fd) {
                struct sockaddr_in client_addr;
                socklen_t addr_len = sizeof(client_addr);
                int client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &addr_len);
                if (client_fd == -1) {
                    perror("accept");
                    continue;
                }

                printf("Accepted connection from %s:%d\n",
                       inet_ntop(AF_INET, &client_addr.sin_addr, ipBuffer, sizeof(ipBuffer)),
                       ntohs(client_addr.sin_port));

                if (set_nonblocking(client_fd) == -1) {
                    perror("fcntl");
                    close(client_fd);
                    continue;
                }

                event.data.fd = client_fd;
                event.events = EPOLLIN;
                if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &event) == -1) {
                    perror("epoll_ctl");
                    close(client_fd);
                }
            } else {
                int client_fd = events[i].data.fd;
                ssize_t received = recv(client_fd, inputBuffer, INPUT_BUFFER_SIZE, 0);
                if (received <= 0) {
                    close(client_fd);
                } else {
                    inputBuffer[received] = '\0';
                    findPath(inputBuffer, pathBuffer);
                    printf("Requested path: %s\n", pathBuffer);

                    const char* response = pseudoRouter(pathBuffer);
                    if (response == NULL) {
                        shutdown(client_fd, SHUT_RDWR);
                        close(client_fd);
                    } else {
                        ssize_t sent = send(client_fd, response, strlen(response), MSG_NOSIGNAL);
                        if (sent == -1) {
                            perror("send");
                        }
                        shutdown(client_fd, SHUT_RDWR);
                        close(client_fd);
                        printf("Sent %zd bytes to client and closed connection.\n", sent);
                    }
                }
            }
        }
    }

    close(server_fd);
    return 0;
}
