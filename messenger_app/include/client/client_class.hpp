#ifndef CLIENT
#define CLIENT

#include "socket.hpp"
#include <nlohmann/json.hpp>
#include <string>
#include <vector>
#include "message_data.hpp"


class client
{
public:
    client();
    void create_login_json(const std::string& username, const std::string& password); //for login slot
    void create_register_json(const std::string& username, const std::string& password);// for register slot
    void create_message_json(const std::string& username, const std::string& message);//for send message slot
    void waiting_autorization_confirm();//the receive works in this function
    void refresh();//send server for online users
    void login_confirm(unsigned int UID, const std::string& usernmae);//when login is successful
    void register_confirm(unsigned int UID, const std::string& username);//When register is successful
    void set_online_users_vector(const json& users_online);//get online users from server
    void set_error_message(const json& temp);//when something is wrong
    void set_message(const json& message_json);//get message from other clients
    void waiting_for_message();
    std::string get_sender_username() const;
    std::vector<message_data*> get_online_users_vector() const;

public: // user functions for using Qt
    std::string get_error_message()
    {
        std::string result_error = m_data->get_error();
        m_data->set_error("");
        return result_error;
    }
    unsigned int get_sender()
    {
        return m_data->get_from();
    }
    std::string get_message()
    {
        std::string mess = m_data->get_message();
        m_data->set_message("");
        return mess;
    }

    unsigned int get_client_UID()
    {
        return m_data->get_UID();
    }

    std::string get_client_username()
    {
        return m_data->get_username();
    }

    bool get_connection()
    {
        return connection;
    }

    void set_connection(bool connection_state)
    {
        connection = connection_state;
    }

private:
    typedef struct server_config {
        std::string ip;
        int port;
    }server_config_t;

private:
    itc::socket sock;
    message_data* m_data;
    std::vector<message_data*> users_vector;
    bool connection = false;
    void send_to_server(nlohmann::json type_json);
    server_config get_ip_port();
};

#endif
