#include "talker.hpp"
#include <iostream>

int main()
{
    try {
        chatting::socket server;
        server.bind(8080);
        server.listen(5);
        chatting::talker::read_file();
        while (1) {
            chatting::socket client = server.accept();
            chatting::talker* t = new chatting::talker(client);
            t->run();
        }
        server.close();
    } catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
    }
    return 0;
}
