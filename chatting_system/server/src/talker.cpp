#include "talker.hpp"
#include <thread>
#include <chrono>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <cassert>

#define BUFF_SIZE 4096

json chatting::talker::all_users = R"({})";
json chatting::talker::online_users = json::parse(R"({"type" : "users_online", "users" : []})");
std::unordered_map<unsigned, chatting::socket*> chatting::talker::users_fd = {};
const std::string chatting::talker::users_file = "database/users_data.json";
std::mutex* chatting::talker::mtx_all_users = new std::mutex();
std::mutex* chatting::talker::mtx_online_users = new std::mutex();

chatting::talker::talker(const chatting::socket& s)
    : m_socket(s)
    , m_uid(0)
{
}

void chatting::talker::run()
{
    std::thread th(&talker::talk, this);
    th.detach();
}

void chatting::talker::talk()
{
    try {
        login();
        if (m_uid <= 0) {
            client_close();
            return;
        }
        talk_to_client();
        client_close();
    } catch(...) {
        client_close();
    }
}

void chatting::talker::login()
{
    char* buff = nullptr;
    while (0 >= m_uid) {
        buff = new char[BUFF_SIZE]{};
        int received = m_socket.recv(buff, BUFF_SIZE);
        std::cout << buff << "\n";
        if (received <= 0) {
            m_socket.close();
            return;
        }
        attempt_login(buff);
        delete buff;
    }
}

void chatting::talker::talk_to_client()
{
    char* buff = nullptr;
    do {
        delete buff;
        buff = new char[BUFF_SIZE]{};
        int received = m_socket.recv(buff, BUFF_SIZE);
        std::cout << buff << "\n";
        if (received <= 0) {
            break;
        }
    } while (buffer_handler(buff));
}

void chatting::talker::client_close()
{
    mtx_online_users -> lock();
    for (auto& user : users_fd) {
        if (user.second == &m_socket) {
            auto it = online_users["users"].begin();
            for (; it != online_users["users"].end(); ++it) {
                if (it->at("UID") == user.first) {
                    online_users["users"].erase(it);
                    break;
                }
            }
            users_fd.erase(user.first);
            break;
        }
    }
    mtx_online_users->unlock();
    m_socket.close();
}

void chatting::talker::send_message(const std::string& type, const std::string& message)
{
    m_msg["type"] = type;
    m_msg["message"] = message;
    std::string message_text = m_msg.dump();
    m_socket.send(message_text.c_str(), message_text.size());
}

bool chatting::talker::is_valid_json(json& message, const char* buffer)
{
    try {
        message = json::parse(buffer);
    } catch (...) {
        send_message("error", "Invalid json");
        return false;
    }
    if(message["type"].is_null()) {
        send_message("error", "Invalid type");
        return false;
    }
    return true;
}

void chatting::talker::attempt_login(const char* buffer)
{
    json message = {};
    if(! is_valid_json(message, buffer)) {
        return;
    }
    std::string type = message["type"];
    if (type == "register") {
        registration(message);
        return;
    }
    if (type == "login") {
        login_check(message);
        return;
    }
    send_message("error", "Not logged in");
}

bool chatting::talker::buffer_handler(const char* buffer)
{
    json message = {};
    if(! is_valid_json(message, buffer)) {
        return false;
    }
    return message_handler(message);
}

bool chatting::talker::message_handler(json& message)
{
    std::string type = message["type"];
    if (type == "message") {
        if (message["from"] != m_uid || message["to"].is_null()
            || message["message"].is_null()) {
            send_message("error", "Invalid message");
            return false;
        }
        int to_uid = message["to"];
        if (users_fd.count(to_uid) != 0) {
            chatting::socket* s = users_fd[to_uid];
            std::string data = message.dump();
            int sent_status = s->send(data.c_str(), data.size());
            send_message("sent", std::to_string(sent_status));
            return true;
        }
    }
    if (type == "users_online" || type == "message") {
        std::string users = online_users.dump();
        m_socket.send(users.c_str(), users.size());
        return true;
    }
    send_message("error", "Invalid type");
    return false;
}

void chatting::talker::registration(json& msg_json)
{
    if(user_exists(msg_json["username"])) {
        send_message("error", "username exists");
        return;
    }
    if (msg_json["username"].is_null() || msg_json["password"].is_null()) {
        send_message("error", "check username or password");
        return;
    }
    unsigned int user_id = generate_uid();
    msg_json.erase("type");
    msg_json["UID"] = user_id;
    mtx_all_users->lock();
    all_users["users"].push_back(msg_json);
    write_to_file();
    mtx_all_users->unlock();
    msg_json["type"] = "register";
    msg_json.erase("password");
    send_response(msg_json);
}

void chatting::talker::login_check(json& msg_json)
{
    if (msg_json["username"].is_null() || msg_json["password"].is_null()) {
        send_message("error", "check username or password");
        return;
    }
    for (const auto& user: all_users["users"]) {
        if (user["username"] != msg_json["username"]) {
            continue;
        }
        if (user["password"] != msg_json["password"]) {
            break;
        }
        msg_json.erase("password");
        msg_json["UID"] = user["UID"];
        m_uid = user["UID"];
        send_response(msg_json);
        mtx_online_users->lock();
        if (users_fd.count(m_uid) != 0) {
            users_fd[m_uid]->shutdown();
        } else {
            msg_json.erase("type");
            online_users["users"].push_back(msg_json);
        }
        users_fd[m_uid] = &m_socket;
        mtx_online_users->unlock();
        return;
    }
    send_message("error", "check username or password");
}

void chatting::talker::send_response(const json& response)
{
    std::string resp = response.dump();
    m_socket.send(resp.c_str(), resp.size());
}

void chatting::talker::write_to_file()
{
    std::ofstream output_file(chatting::talker::users_file);
    if (!output_file.is_open()) {
        throw std::runtime_error{"Couldn't open file"};
    }
    output_file << all_users.dump();
    output_file.close();
}


void chatting::talker::read_file()
{
    std::ifstream input_file(chatting::talker::users_file);
    std::stringstream ss;
    if (!input_file.is_open()) {
        throw std::runtime_error{"couldn't open the users data file"};
    }
    ss << input_file.rdbuf();
    if (ss.fail()) {
        throw std::runtime_error{"couldn't read the users data file"};
    }
    all_users = json::parse(ss.str());
    input_file.close();
}

bool chatting::talker::user_exists(const std::string& username) {
    for (const auto& user : all_users["users"]) {
        if (user["username"] == username) {
            return true;
        }
    }
    return false;
}

unsigned int chatting::talker::generate_uid()
{
    static unsigned int counter = 0;
    unsigned int uid = static_cast<unsigned int>
        (std::chrono::duration_cast<std::chrono::milliseconds>
         (std::chrono::system_clock::now().time_since_epoch()).count()
         );
    uid = (uid << 8) | (counter++ % 256);
    return uid;
}
