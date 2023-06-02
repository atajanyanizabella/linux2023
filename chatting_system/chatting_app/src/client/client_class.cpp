#include "../../include/client/client_class.hpp"
#include "../../include/client/message_data.hpp"
#include <nlohmann/json.hpp>
#include <thread>
#include <chrono>
#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>

#define BUFF_SIZE 4096

using json = nlohmann::json;

client::client()
    : m_socket()
{
    m_socket.connect(get_ip_port().ip, get_ip_port().port);
    m_data = new chatting::message_data;
}

client::server_config_t client::get_ip_port()
{
    std::ifstream config_file("src/client/server.conf");
    if (!config_file.is_open()) {
        throw std::runtime_error{"couldn't open the users data file"};
    }
    std::unordered_map<std::string, std::string> config_data = {};
    std::string line = "";
    while (std::getline(config_file, line)) {
        std::size_t seperator_pos = line.find('=');
        if (seperator_pos != std::string::npos) {
            std::string key = line.substr(0, seperator_pos);
            std::string value = line.substr(seperator_pos + 1);
            config_data[key] = value;
        }
    }
    config_file.close();
    server_config server_config = {};
    server_config.ip = config_data["IP"];
    server_config.port = std::stoi(config_data["PORT"]);
    return server_config;
}

//function for receiving messages from the server
void client::waiting_authorization_confirm()
{
    try {
        std::cout << "started authorization\n";
        char* buff = new char[BUFF_SIZE]{}; //TODO // change to std::string or std::vector
        m_socket.recv(buff, BUFF_SIZE);
        std::cout << "received responce from server\n";
        json message = json::parse(buff);
        std::string type = message["type"];
        std::cout << "starting typing\n";
        if (type == "login") {
            std::cout << "calling login_confirm\n";
            login_confirm(message["UID"], message["username"]);
        } else if (type == "register") {
            register_confirm(message["UID"], message["username"]);
        } else if (type == "error") {
            set_error_message(message);
        }
    } catch (const std::exception& e) {
        std::cerr << "Failed to receive data from the server" << std::endl;
    }
}

void client::waiting_for_message()
{
    try {
        char* buff = new char[BUFF_SIZE]; // TODO
        m_socket.recv(buff, BUFF_SIZE);
        json message = json::parse(buff);
        delete [] buff;
        std::string type = message["type"];
        if (type == "message") {
            set_message(message);
        } else if (type == "users_online") {
            set_online_users_vector(message);
        }
    } catch (...) {
        set_connection(true); //TODO
    }
}

//function that saves the message and the sender
void client::set_message(const json& message_json)
{
    m_data->set_from(message_json["from"]);
    m_data->set_message(message_json["message"]);
}

void client::set_error_message(const json& temp)
{
    m_data->set_error(temp["message"]);
}

void client::create_login_json(const std::string& username,
                                        const std::string& password)
{
    json login_json = {
        {"type", "login"},
        {"username", username},
        {"password", password},
    };
    std::string message = login_json.dump();
    m_socket.send(message.c_str(), message.size());
    waiting_authorization_confirm();
}

void client::create_register_json(const std::string& username,
                                            const std::string& password)
{
    json register_json = {
        {"type", "register"},
        {"username", username},
        {"password", password},
    };
    std::string message = register_json.dump();
    m_socket.send(message.c_str(), message.size());
    waiting_authorization_confirm();
}

void client::create_message_json(const std::string& username,
                                            const std::string& message)
{
    unsigned int from = m_data->get_uid();
    unsigned int to = 0;
    for (const auto& user : m_users_vector) {
        if (user->get_username() == username) {
            to = user->get_uid();
            break;
        }
    }
    json message_json = {
        {"type", "message"},
        {"from", from},
        {"to", to},
        {"message", message},
    };
    std::string data = message_json.dump();
    m_socket.send(data.c_str(), data.size());
}

void client::refresh()
{
    json online_users_json = {
        {"type", "users_online"},
    };
    std::string message = online_users_json.dump();
    while (true) {
        m_socket.send(message.c_str(), message.size());
        std::this_thread::sleep_for(std::chrono::seconds(5));
    }
}

void client::set_online_users_vector(const json& message)
{
    json users_array_json = message["users"];
    m_users_vector.clear();
    for (const auto& user : users_array_json) {
        unsigned int uid = user["UID"];
        std::string username = user["username"];
        chatting::message_data* data = new chatting::message_data(uid, username);
        m_users_vector.push_back(data);
    }
}

std::vector<chatting::message_data*> client::get_online_users_vector() const
{
    return m_users_vector;
}

void client::login_confirm(unsigned int uid, const std::string& username)
{
    m_data -> set_uid(uid);
    m_data -> set_username(username);
}

void client::register_confirm(unsigned int uid, const std::string& username)
{
    m_data -> set_uid(uid);
    m_data -> set_username(username);
}

std::string client::get_sender_username() const
{
    std::string result = "";
    for (const auto& user : m_users_vector) {
        if (m_data->get_from() == user->get_uid()) {
            result = user->get_username();
            break;
        }
    }
    return result;
}
