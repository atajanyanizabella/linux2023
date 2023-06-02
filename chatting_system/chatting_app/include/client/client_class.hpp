#ifndef CLIENT_H
#define CLIENT_H

#include "socket.hpp"
#include <nlohmann/json.hpp>
#include <string>
#include <vector>
#include "message_data.hpp"

using json = nlohmann::json;

class client
{
    public:
        client();
        void create_login_json(const std::string& username, const std::string& password); // for login slot
        void create_register_json(const std::string& username, const std::string& password); // for register slot
        void create_message_json(const std::string& username, const std::string& message); // for send message slot
        void waiting_authorization_confirm();
        void refresh();
        void login_confirm(unsigned int uid, const std::string& username);
        void register_confirm(unsigned int UID, const std::string& username);
        void set_online_users_vector(const json& users_online);
        void set_error_message(const json& temp);
        void set_message(const json& message_json);
        void waiting_for_message();
        std::string get_sender_username() const;
        std::vector<chatting::message_data*> get_online_users_vector() const;
    public:
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
        unsigned int get_client_uid()
        {
            return m_data->get_uid();
        }

        std::string get_client_username()
        {
            return m_data->get_username();
        }

        bool get_connection()
        {
            return m_connection;
        }

        void set_connection(bool connection_state)
        {
            m_connection = connection_state;
        }
    private:
        typedef struct server_config {
            std::string ip;
            int port;
        } server_config_t;
    private:
        chatting::socket m_socket;
        chatting::message_data* m_data;
        std::vector<chatting::message_data*> m_users_vector;
        bool m_connection = false;
        server_config_t get_ip_port();
};

#endif
