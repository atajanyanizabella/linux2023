#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <cerrno>
#include <arpa/inet.h>
#include <stdio.h>

void func(int sockfd)
{
    char buff[4096];
    ssize_t bytes_written = 0;
    ssize_t bytes_read = 0;
    while (true) {
        bzero(buff, 4096);
        printf("Write the message: ");
        int n = 0;
        while ((buff[n++] = getchar()) != '\n');
        if (strncmp("exit", buff, 4) == 0) {
            printf("Client exited...\n");
            send(sockfd, "Client exited...\n", 17, 0);
            return;
        }
        bytes_written = send(sockfd, buff, sizeof(buff), 0);
        if (bytes_written == -1) {
            perror("Error: while sending from client\n");
            exit(errno);
        }
        bzero(buff, sizeof(buff));
        bytes_read = recv(sockfd, buff, sizeof(buff), 0);
        if (bytes_read == -1) {
            perror("Error: while receiving from server\n");
            exit(errno);
        }
        printf("From server: %s", buff);
    }
}

int main()
{
        int client_fd = 0;
        client_fd = socket(AF_INET, SOCK_STREAM, 0);
        if (client_fd == -1) {
            perror("Client: Socket creation failed\n");
            exit(errno);
        }
        printf("Socket successfully created");
        struct sockaddr_in addr;
        bzero(&addr, sizeof(addr));
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = inet_addr("127.0.0.1");
        addr.sin_port = htons(8080);
        socklen_t addr_size = sizeof(addr);
        int connect_status = connect(client_fd, (sockaddr*)&addr, addr_size);
        if (connect_status == -1) {
            perror("Client: connection failed\n");
            exit(errno);
        }
        printf("Connected to server\n");
        func(client_fd);
        close(client_fd);
        return 0;
}
