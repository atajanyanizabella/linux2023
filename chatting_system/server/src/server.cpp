#include "talker.hpp"

int main()
{
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
    return 0;
}
