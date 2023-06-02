#ifndef USERS_LIST
#define USERS_LIST

#include <string>

class message_data
{
public:
    message_data();
    message_data(unsigned int UID, const std::string& username);
    unsigned int get_UID() const;
    std::string get_username() const;
    void set_UID(unsigned int UID);
    void set_username(const std::string& username);
    void set_error(const std::string& error_message);
    std::string get_error() const;
    void set_from(unsigned int uid);
    void set_message(const std::string& mess);
    unsigned int get_from() const;
    std::string get_message() const;

private:
    unsigned int UID = 0;
    std::string username = "";
    std::string error_message = "";
    unsigned int from = 0;
    std::string message = "";
};

#endif
