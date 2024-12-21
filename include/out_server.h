#pragma once

#include <message.h>

class Out_Server
{
void read_settings()
{
    std::ifstream file("../settings.json");
    json settings;
    file >> settings;

    SERVER_ADDRESS = settings["server_address"];
    SERVER_PORT = settings["server_port"];
}

using boost::asio::ip::tcp;

void connect_to_server(const std::string& server_ip, unsigned short server_port) {
    try {
        std::cout << "[INFO] Начало установки соединения с сервером " << server_ip << ":" << server_port << std::endl;

        // 1. Создаём io_context
        boost::asio::io_context io_context;
        std::cout << "[INFO] Контекст IO создан." << std::endl;

        // 2. Создаём сокет
        tcp::socket socket(io_context);
        std::cout << "[INFO] Сокет создан." << std::endl;

        // 3. Указываем IP-адрес и порт сервера
        tcp::endpoint endpoint(boost::asio::ip::address::from_string(server_ip), server_port);
        std::cout << "[INFO] Endpoint установлен: " << server_ip << ":" << server_port << std::endl;

        // 4. Подключаемся к серверу
        socket.connect(endpoint);
        std::cout << "[SUCCESS] Соединение установлено с сервером " << server_ip << ":" << server_port << std::endl;

        // 5. Отправляем сообщение серверу
        std::string message = "Привет, сервер!";
        boost::asio::write(socket, boost::asio::buffer(message));
        std::cout << "[INFO] Сообщение отправлено серверу: " << message << std::endl;

        // 6. Получаем ответ от сервера
        char data[1024];
        size_t length = socket.read_some(boost::asio::buffer(data));
        std::cout << "[SUCCESS] Ответ от сервера получен: " << std::string(data, length) << std::endl;

        // 7. Закрываем сокет
        socket.close();
        std::cout << "[INFO] Соединение закрыто." << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "[ERROR] Ошибка: " << e.what() << std::endl;
    }
}

}