#include "client/socket.hpp"
#include <iostream>

itc::socket::socket(itc::socket::type t)
{
    if (t == STREAM) {
        m_socket_fd = ::socket(AF_INET, SOCK_STREAM, 0);
    } else if (t == DATAGRAM) {
        m_socket_fd = ::socket(AF_INET, SOCK_DGRAM, 0);
    }
    if (m_socket_fd == -1) {
        std::cerr << "Failed to create socket\n";
        exit(1);
    }
}

itc::socket::socket(const itc::socket& t)
{
    m_socket_fd = t.m_socket_fd;
}

void itc::socket::connect(const std::string& ip, unsigned short int port)
{
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(ip.c_str());
    serv_addr.sin_port = htons(port);
    if (::connect(m_socket_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        std::cerr << "Failed to connect to server\n";
        exit(1);
    }
}

int itc::socket::send(const json& data)
{
    std::string str = data.dump();
    const char* buff = str.c_str();

    int temp = ::send(m_socket_fd, buff, str.size(), 0);
    return temp;
}

void itc::socket::close()
{
    ::close(m_socket_fd);
}

json itc::socket::receive()
{
    char buffer[4096] = {0};
    int valread = recv(m_socket_fd, buffer, 4096, 0);
    if (valread == -1) {
        close();
        throw std::runtime_error("Failed to receive data");
    }
    std::string str(buffer);
    if (str.empty()) {
        throw std::runtime_error("Received empty data");
    }

    return json::parse(str);
}
