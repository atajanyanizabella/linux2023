#include "threadpool.hpp"
#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <cerrno>
#include <arpa/inet.h>
#include <pthread.h>

struct socket_t
{
    int connfd;
};

alita::output_t func(alita::input_t arg)
{
    socket_t* sock = (socket_t*)arg;
    char buff[4096];
    ssize_t bytes_written = 0;
    ssize_t bytes_read = 0;
    while (true) {
        bzero(buff, 4096);
        bytes_read = recv(sock->connfd, buff, sizeof(buff), 0);
        if (bytes_read == -1) {
            perror("Error: while receiving from client\n");
            exit(errno);
        }
        printf("From client: %s", buff);
        bzero(buff, 4096);
        printf("To client: ");
        int n = 0;
        while ((buff[n++] = getchar()) != '\n');
        if (strncmp("exit", buff, 4) == 0) {
            printf("Server exited...\n");
            send(sock->connfd, "Server exited...\n", 17, 0);
            break;
        }
        bytes_written = send(sock->connfd, buff, sizeof(buff), 0);
        if (bytes_written == -1) {
            perror("Error: while sending from server\n");
            exit(errno);
        }
    }
    return NULL;
}

int main()
{
    int sockfd = 0;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("Server: Socket creation failed\n");
        exit(errno);
    }
    printf("Socket successfully created...\n");
    struct sockaddr_in servaddr;
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(8080);
    int bind_status = bind(sockfd, (sockaddr*)&servaddr, sizeof(servaddr));
    if (bind_status == -1) {
        perror("Server: Socket bind failed\n");
        exit(errno);
    }
    printf("Socket successfully binded\n");
    int listen_status = listen(sockfd, 5);
    if (listen_status == -1) {
        printf("Server: Listen failed\n");
        exit(errno);
    }
    printf("Server listening\n");
    int n = 0;
    while (true) {
        int connfd = 0;
        struct sockaddr_in claddr;
        socklen_t claddr_size = sizeof(claddr);
        connfd = accept(sockfd, (sockaddr*)&claddr, &claddr_size);
        n++;
        if (connfd == -1) {
            perror("Server: Accept failed\n");
            exit(errno);
        }
        printf("Server accepted the client%d\n", n);
        socket_t* arg = new socket_t;
        arg->connfd = connfd;
        alita::threadpool::instance().enqueue_work(func, (void*)arg, NULL);
    }
    close(sockfd);
    return 0;
}
