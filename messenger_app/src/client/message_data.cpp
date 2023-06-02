#include "client/message_data.hpp"
message_data::message_data()
{
}

message_data::message_data(unsigned int UID, const std::string& username)
    : UID(UID)
    , username(username)
{
}

unsigned int message_data::get_UID() const
{
    return UID;
}

std::string message_data::get_username() const
{
    return username;
}

void message_data::set_UID(unsigned int UID)
{
    this->UID = UID;
}

void message_data::set_username(const std::string& username)
{
    this->username = username;
}

void message_data::set_error(const std::string& error)
{
    this->error_message = error;
}

std::string message_data::get_error() const
{
    return error_message;
}

void message_data::set_from(unsigned int uid)
{
    this->from = uid;
}

void message_data::set_message(const std::string& mess)
{
    this->message = mess;
}

unsigned int message_data::get_from() const
{
    return from;
}

std::string message_data::get_message() const
{
    return message;
}
