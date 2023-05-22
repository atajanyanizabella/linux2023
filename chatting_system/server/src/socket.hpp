#ifndef SOCKET_H
#define SOCKET_H

#include <string>

namespace chatting {
    class socket
    {
        public:
            void bind(unsigned short port);
            void listen(int size);
            socket accept();
            void connect(const std::string& ip, unsigned short port);
            int recv(void* buff, int length);
            int send(const void* buff, int length);
            void close();
            void shutdown();
        private:
            int m_socket_fd;
        public:
            socket();
            socket(int);
            socket(const socket&);
            ~socket() = default;
    };
}

#endif
