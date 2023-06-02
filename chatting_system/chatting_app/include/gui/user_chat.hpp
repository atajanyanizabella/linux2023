#ifndef USER_CHAT_HPP
#define USER_CHAT_HPP

#include <string>
#include <QString>

class QTextDocument;

class user_chat
{

    public:
        user_chat(const std::string& username);

        std::string get_username();
        QTextDocument* get_chat();
        void append_sending_message(const QString& message);
        void append_receiving_message(const QString& message);

    private:
        QTextDocument* m_chat;
        std::string m_user_side_name;
};


#endif
