
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
    while(*(request)++ != ' ') {}
    while(*request != ' ') {
        *(target)++ = *(request)++;
    }
    *target = 0x00;
}

const char* pseudoRouter(const char* requestedPath)
{
    if(strcmp("/suchyKreweta", requestedPath) == 0) {
        return STATIC_RESPONSE_SUCHA_KREWETA;
    }
    else if(strcmp("/vanish", requestedPath) == 0) {
        return NULL;
    }
    else {
        return STATIC_RESPONSE;
    }
}

int set_nonblocking(int fd) {
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1) return -1;
    flags |= O_NONBLOCK;
    return fcntl(fd, F_SETFL, flags);
}

int main(void) {
    char inputBuffer[INPUT_BUFFER_SIZE + 1];
    int server = socket(AF_INET, SOCK_STREAM, 0);
    if(server == -1) {
        printf("Nie udalo sie stworzyc socketa aha12\n");
        exit(-1);
    }

    if (set_nonblocking(server) == -1) {
        printf("Nie udalo sie ustawic nieblokujacego socketa\n");
        exit(-1);
    }

    struct sockaddr_in sai;
    sai.sin_addr.s_addr = inet_addr("127.0.0.1");
    sai.sin_family = AF_INET;
    sai.sin_port = htons(8181);
    memset(sai.sin_zero, 0, 8);

    int ret = bind(server, (struct sockaddr*)&sai, sizeof(sai));
    if(ret != 0) {
        printf("Dupa nie dziala aha ok\n");
        exit(-1);
    }

    ret = listen(server, 1024);
    if(ret != 0) {
        printf("Dupa nie udalo sie nasluchiwac na komendzie w plocku\n");
        exit(-1);
    }

    fd_set master_set, working_set;
    FD_ZERO(&master_set);
    FD_SET(server, &master_set);

    while(1) {
        working_set = master_set;
        ret = select(FD_SETSIZE, &working_set, NULL, NULL, NULL);
        if(ret < 0) {
            printf("Dupa select error\n");
            exit(-1);
        }

        for(int i = 0; i < FD_SETSIZE; ++i) {
            if(FD_ISSET(i, &working_set)) {
                if(i == server) {
                    struct sockaddr_in clientData;
                    socklen_t size = sizeof(clientData);
                    int client = accept(server, (struct sockaddr*)&clientData, &size);
                    if(client == -1) {
                        printf("Dupa blond w accept\n");
                    } else {
                        printf("Accepted connection from %s:%d\n",
                               inet_ntop(AF_INET, &clientData.sin_addr.s_addr, inputBuffer, 16),
                               ntohs(clientData.sin_port));

                        if(set_nonblocking(client) == -1) {
                            printf("Nie udalo sie ustawic nieblokujacego socketa klienta\n");
                            close(client);
                        } else {
                            FD_SET(client, &master_set);
                        }
                    }
                } else {
                    size_t received = recv(i, inputBuffer, INPUT_BUFFER_SIZE, 0);
                    if(received <= 0) {
                        close(i);
                        FD_CLR(i, &master_set);
                    } else {
                        inputBuffer[received] = 0x00;
                        char pathBuffer[1024];
                        findPath(inputBuffer, pathBuffer);
                        printf("Requested path: %s\n", pathBuffer);

                        const char* response = pseudoRouter(pathBuffer);
                        if(response == NULL) {
                            shutdown(i, SHUT_RDWR);
                            close(i);
                            FD_CLR(i, &master_set);
                        } else {
                            size_t sent = send(i, response, strlen(response), 0);
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
