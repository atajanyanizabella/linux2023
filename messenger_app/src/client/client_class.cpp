#include "client/client_class.hpp"
#include "client/message_data.hpp"
#include <nlohmann/json.hpp>
#include <cassert>
#include <thread>
#include <chrono>
#include <iostream>
#include <fstream>
#include <string>
#include <map>


using json = nlohmann::json;
client::client() : sock(itc::socket::STREAM)
{
    sock.connect(get_ip_port().ip, get_ip_port().port); // Connect to the server
    m_data = new message_data();
}

client::server_config_t client::get_ip_port() //take server ip and port from configuration file
{
    std::ifstream config_file("src/client/config.conf");
    assert (config_file.is_open() && "Failed to open the configuration file.");
    std::map<std::string, std::string> config_data;
    std::string line;
    while (std::getline(config_file, line)) {
        size_t separator_pos = line.find('=');
        if (separator_pos != std::string::npos) {
            std::string key = line.substr(0, separator_pos);
            std::string value = line.substr(separator_pos + 1);
            key.erase(0, key.find_first_not_of(" \t"));
            key.erase(key.find_last_not_of(" \t") + 1);
            value.erase(0, value.find_first_not_of(" \t"));
            value.erase(value.find_last_not_of(" \t") + 1);
            config_data[key] = value;
        }
    }
    config_file.close();
    server_config server_config;
    server_config.ip = config_data["IP"];
    server_config.port = std::stoi(config_data["PORT"]);
    return server_config;
}

void client::send_to_server(json type_json) // Function to send data to the server
{
    try {
        sock.send(type_json); // Send the JSON data to the server
    } catch (const std::exception& ex) {
        std::cerr << "Failed to send data to the server: " << ex.what() << std::endl;
    }
}

void client::waiting_autorization_confirm() // function for receiving messages from the server
{
    try {
        json temp = sock.receive(); // Receive a JSON message from the server
        assert(!temp.empty());
        if (temp["type"] == "login") { // Check the type of the received message
            login_confirm(temp["UID"], temp["username"]);
        } else if (temp["type"] == "register") {
            register_confirm(temp["UID"], temp["username"]);
        } else if (temp["type"] == "error") {
            set_error_message(temp);
        }
    } catch (const std::exception& ex) {
        std::cerr << "Failed to receive data from the server: " << ex.what() << std::endl;
    }
}

void client::waiting_for_message() // Thread function for receiving messages from the server
{
    try {
        json temp = sock.receive();
        assert(!temp.empty());
        std::string type = temp["type"];
        if (type == "message") {
            set_message(temp);
        } else if (type == "users_online") {
            set_online_users_vector(temp);
        }
    } catch (const std::exception& ex) {
        set_connection(true);
    }
}

void client::set_message(const json& message_json) // function that save message and sender
{
    assert(!message_json.empty());
    m_data->set_from(message_json["from"]);
    m_data->set_message(message_json["message"]);
}

void client::set_error_message(const json& temp) // function that save error message
{
    assert(!temp.empty());
    m_data->set_error(temp["message"]);
}

void client::create_login_json(const std::string& username, const std::string& password) // Function to perform login
{
    json login_json = {
        {"type", "login"},
        {"username", username},
        {"password", password},
    };
    send_to_server(login_json);
    waiting_autorization_confirm();
}

void client::create_register_json(const std::string& username, const std::string& password) // Function to perform registration
{
    json register_json = {
        {"type", "register"},
        {"username", username},
        {"password", password},
    };
    send_to_server(register_json);
    waiting_autorization_confirm();
}

void client::create_message_json(const std::string& username, const std::string& message) // Function to send a message
{
    unsigned int from = m_data->get_UID();
    unsigned int to = 0;
    for (const auto& user : users_vector) {
        if (user->get_username() == username) {
            to = user->get_UID();
            break;
        }
    }
    json message_json = {
        {"type", "message"},
        {"from", from},
        {"to", to},
        {"message", message},
    };
    send_to_server(message_json);
}
void client::refresh() // Function to refresh the list of online users
{
    json temp = {
        {"type", "users_online"},
    };
    while (true) {
        send_to_server(temp);
        std::this_thread::sleep_for(std::chrono::seconds(5)); // Sleep for 60 seconds before sending the next request
    }
}

void client::set_online_users_vector(const json& temp) // Function to handle the list of online users
{
    assert(!temp.empty());
    json usersArray = temp["users"];
    users_vector.clear();
    for (const auto& user_m : usersArray) {
        unsigned int UID = user_m["UID"];
        std::string username = user_m["username"];
        message_data* m_data = new message_data(UID, username);
        users_vector.push_back(m_data);
    }
}

std::vector<message_data*> client::get_online_users_vector() const // Function to get the vector of online users
{
    return users_vector;
}

void client::login_confirm(unsigned int UID, const std::string& username) //Function that confirm user login
{
    m_data -> set_UID(UID);
    m_data -> set_username(username);
}

void client::register_confirm(unsigned int UID, const std::string& username) //Function that confirm user register
{
    m_data -> set_UID(UID);
    m_data -> set_username(username);
}

std::string client::get_sender_username() const //Function to get username from message sender
{
    std::string result = "";
    for (const auto& user : users_vector) {
        if (m_data->get_from() == user->get_UID()) {
            result = user->get_username();
            break;
        }
    }
    return result;
}
