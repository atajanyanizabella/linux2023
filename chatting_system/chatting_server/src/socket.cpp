#include "socket.hpp"
#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <strings.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <cassert>

void chatting::socket::bind(unsigned short port)
{
    struct sockaddr_in addr;
    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    int bind_status = ::bind(m_socket_fd, (struct sockaddr*)&addr, sizeof(addr));
    if (bind_status == -1) {
        throw std::runtime_error{"bind::Can't bind the socket\n"};
    }
}

void chatting::socket::listen(int size)
{
    assert(size > 0 && "Listen:Queue size can't be non-positive\n");
    assert(m_socket_fd >= 0);
    int listen_status = ::listen(m_socket_fd, size);
    if (listen_status == -1) {
        throw std::runtime_error{"listen::failed listening process\n"};
    }
}

chatting::socket chatting::socket::accept()
{
    assert(m_socket_fd >= 0);
    int sock_fd = ::accept(m_socket_fd, nullptr, nullptr);
    if (sock_fd == -1) {
        std::cerr << "Can't accept client" << std::endl;
    }
    return socket(sock_fd);
}

int chatting::socket::recv(void* buff, int length)
{
    return ::recv(m_socket_fd, buff, length, 0);
}

int chatting::socket::send(const void* buff, int length)
{
    std::cout << "RESPONSE:: " << (char*)buff << std::endl;
    return ::send(m_socket_fd, buff, length, 0);
}

void chatting::socket::close()
{
    ::close(m_socket_fd);
}

chatting::socket::socket()
{
    m_socket_fd = ::socket(AF_INET, SOCK_STREAM, 0);
    if (m_socket_fd < 0) {
        throw std::runtime_error("Can't create a socket\n");
    }
}

chatting::socket::socket(int fd)
    : m_socket_fd(fd)
{
}

chatting::socket::socket(const chatting::socket& other)
    : m_socket_fd(other.m_socket_fd)
{
}

void chatting::socket::shutdown()
{
    ::shutdown(m_socket_fd, SHUT_RDWR);
}
