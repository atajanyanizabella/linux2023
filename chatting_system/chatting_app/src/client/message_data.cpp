#include "../../include/client/message_data.hpp" // TODO

void chatting::message_data::set_uid(unsigned int uid)
{
    m_uid = uid;
}

void chatting::message_data::set_username(const std::string& username)
{
    m_username = username;
}

void chatting::message_data::set_error(const std::string& error_message)
{
    m_error_message = error_message;
}

void chatting::message_data::set_from(unsigned int uid)
{
    m_from = uid;
}

void chatting::message_data::set_message(const std::string& message)
{
    m_message = message;
}

unsigned int chatting::message_data::get_uid() const
{
    return m_uid;
}

std::string chatting::message_data::get_username() const
{
    return m_username;
}

std::string chatting::message_data::get_error() const
{
    return m_error_message;
}

unsigned int chatting::message_data::get_from() const
{
    return m_from;
}

std::string chatting::message_data::get_message() const
{
    return m_message;
}

chatting::message_data::message_data()
{
}

chatting::message_data::message_data(unsigned int uid,
                                    const std::string& username)
    : m_uid{uid}
    , m_username{username}
{
}
