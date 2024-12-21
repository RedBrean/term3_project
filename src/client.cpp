#include <boost/asio.hpp>
#include <iostream>
#include <string>

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

int main() {
    read_settings();

    boost::asio::io_context io_context;

    // Создаем сокет для подключения к серверу
    tcp::socket socket(io_context);

    // Подключаемся к серверу по его IP-адресу и порту
    tcp::resolver resolver(io_context);

    std::cout << "try" << SERVER_ADDRESS << ":" << SERVER_PORT << std::endl;


    boost::asio::connect(socket, resolver.resolve(SERVER_ADDRESS, SERVER_PORT));  // IP сервера

    std::string message = "Привет, мир!";

    // Отправляем сообщение серверу
    boost::asio::write(socket, boost::asio::buffer(message));

    std::cout << "Message sent to server: " << message << std::endl;

    return 0;
}
