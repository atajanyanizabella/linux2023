#ifndef SOCKET_H
#define SOCKET_H

#include <netinet/in.h>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace itc {
    class socket
    {
    public:
        enum type {STREAM, DATAGRAM};

        socket(type t);
        socket(const socket& t);
        void connect(const std::string& ip, unsigned short int port);
        int send(const json& data);
        json receive();
        void close();

    private:
        int m_socket_fd;
    };
}

#endif

