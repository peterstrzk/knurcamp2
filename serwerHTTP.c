#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/select.h>

const char* STATIC_RESPONSE =
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html; charset=UTF-8\r\n"
        "Content-Length: 155\r\n"
        "Server: KNURCAMP SERVER 2137\r\n"
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
        "Server: KNURCAMP SERVER 2137\r\n"
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
    sai.sin_port = htons(2137);
    memset(sai.sin_zero, 0, 8);

    if (bind(server, (struct sockaddr*)&sai, sizeof(sai)) != 0) handle_error("bind");
    if (listen(server, 1024) != 0) handle_error("listen");

    fd_set master_set, working_set;
    FD_ZERO(&master_set);
    FD_SET(server, &master_set);
    int max_fd = server;

    while (1) {
        working_set = master_set;
        if (select(max_fd + 1, &working_set, NULL, NULL, NULL) < 0) handle_error("select");

        for (int i = 0; i <= max_fd; ++i) {
            if (FD_ISSET(i, &working_set)) {
                if (i == server) {
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
                            FD_SET(client, &master_set);
                            if (client > max_fd) max_fd = client;
                        }
                    }
                } else {
                    ssize_t received = recv(i, inputBuffer, INPUT_BUFFER_SIZE, 0);
                    if (received <= 0) {
                        close(i);
                        FD_CLR(i, &master_set);
                    } else {
                        inputBuffer[received] = 0x00;
                        char pathBuffer[1024];
                        findPath(inputBuffer, pathBuffer);
                        printf("Requested path: %s\n", pathBuffer);

                        const char* response = pseudoRouter(pathBuffer);
                        if (response == NULL) {
                            shutdown(i, SHUT_RDWR);
                            close(i);
                            FD_CLR(i, &master_set);
                        } else {
                            ssize_t sent = send(i, response, strlen(response), 0);
                            shutdown(i, SHUT_RDWR);
                            close(i);
                            FD_CLR(i, &master_set);
                            printf("Sent %zu bytes to client and closed connection.\n", sent);
                        }
                    }
                }
            }
        }
    }

    close(server);
    return 0;
}
