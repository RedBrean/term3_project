#pragma once

#include <boost/asio.hpp>
#include <iostream>
#include <string>
#include <out_server.h>

#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
using boost::asio::ip::tcp;


std::string SERVER_ADDRESS;
std::string SERVER_PORT;

void read_settings()
{
    std::ifstream file("../settings.json");
    json settings;
    file >> settings;

    SERVER_ADDRESS = settings["server_address"];
    SERVER_PORT = settings["server_port"];
}

#pragma once

class Client
{
private:
    Out_Server server;  // Поле класса для хранения объекта Out_Server

public:
    // Конструктор класса Client, который инициализирует объект Out_Server
    Client(boost::asio::io_context& io_context) : server(io_context) {}

    // Метод для подключения к серверу через объект Out_Server
    void connect(const std::string& server_ip, const std::string& server_port) {
        server.connect_to_server(server_ip, server_port);
    }

    // Метод для отправки сообщения на сервер через объект Out_Server
    void send_message(const std::string& message) {
        server.send_raw_message(message);
    }

    // Метод для прослушивания сообщений от сервера
    void listen_for_messages() {
        // Асинхронное ожидание сообщений
        server.async_receive_message([this](const std::string& message) {
            std::cout << "[INFO] Получено сообщение: " << message << std::endl;
            // После получения сообщения снова начинаем слушать сервер
            listen_for_messages();
        });
    }

    // Метод для закрытия соединения через объект Out_Server
    void close_connection() {
        server.close_connection();
    }
};
