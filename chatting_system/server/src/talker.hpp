#ifndef TALKER_H
#define TALKER_H

#include "socket.hpp"
#include "json.hpp"
#include <unordered_map>
#include <mutex>

using json = nlohmann::json;

namespace chatting
{
    class talker
    {
        public:
            talker(const chatting::socket& s);
            void run();
        private:
            void client_close();
            void login();
            void talk_to_client();
            bool message_handler(json& message);
            bool buffer_handler(const char* buffer);
            void send_message(const std::string& type, const std::string& message);
            void send_response(const json& response);
            bool is_valid_json(json& message, const char* buffer);
            void attempt_login(const char* buffer);
            void login_check(json& msg_json);
            void talk();
            void registration(json& msg_json);
            bool user_exists(const std::string& username);
        public:
            static void write_to_file();
            static void read_file();
            static unsigned int generate_uid();
        private:
            static json all_users;
            static json online_users;
            static std::unordered_map<unsigned, chatting::socket*> users_fd;
            static const std::string users_file;
            static std::mutex* mtx_all_users;
            static std::mutex* mtx_online_users;
        private:
            socket m_socket;
            unsigned int m_uid;
            json m_msg = {
                {"type", ""},
                {"message", ""}
            };
    };
}

#endif
