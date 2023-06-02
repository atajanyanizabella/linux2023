#ifndef MESSAGE_DATA_H
#define MESSAGE_DATA_H

#include <string>

namespace chatting
{
    class message_data
    {
        public:
            void set_uid(unsigned int uid);
            void set_username(const std::string& username);
            void set_error(const std::string& error_message);
            void set_from(unsigned int uid);
            void set_message(const std::string& message);
        public:
            unsigned int get_uid() const;
            std::string get_username() const;
            std::string get_error() const;
            unsigned int get_from() const;
            std::string get_message() const;
        public:
            message_data();
            message_data(unsigned int uid, const std::string& username);
        private:
            unsigned int m_uid = 0;
            std::string m_username = "";
            std::string m_error_message = "";
            unsigned int m_from = 0;
            std::string m_message = "";
    };
}

#endif
