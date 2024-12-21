#pragma once

#include <iostream>
#include <boost/asio.hpp>
#include <string>
using boost::asio::ip::tcp;

class Out_Server
{
private:
    tcp::socket socket;  // Сокет как член класса

public:
    // Конструктор с инициализацией сокета
    Out_Server(boost::asio::io_context& io_context) : socket(io_context) {}

    // Метод для подключения к серверу
    void connect_to_server(const std::string& server_ip, const std::string& server_port) {
        try {
            std::cout << "[INFO] Начало установки соединения с сервером " << server_ip << ":" << server_port << std::endl;

            // Преобразуем порт из строки в целое число
            unsigned short port = static_cast<unsigned short>(std::stoi(server_port));

            // Устанавливаем endpoint с указанным IP и портом
            tcp::endpoint endpoint(boost::asio::ip::address::from_string(server_ip), port);
            std::cout << "[INFO] Endpoint установлен: " << server_ip << ":" << port << std::endl;

            // Подключаемся к серверу
            socket.connect(endpoint);
            std::cout << "[SUCCESS] Соединение установлено с сервером " << server_ip << ":" << port << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "[ERROR] Ошибка при подключении: " << e.what() << std::endl;
        }
    }

    // Метод для отправки сообщения на сервер
    void send_raw_message(const std::string& message) {
        try {
            boost::asio::write(socket, boost::asio::buffer(message));
            std::cout << "[INFO] Сообщение отправлено серверу: " << message << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "[ERROR] Ошибка при отправке сообщения: " << e.what() << std::endl;
        }
    }

    // Метод для асинхронного получения сообщения от сервера
    void async_receive_message(std::function<void(const std::string&)> handler) {
        try {
            boost::asio::streambuf buffer;
            boost::asio::async_read_until(socket, buffer, '\n', 
                [this, handler, &buffer](const boost::system::error_code& error, std::size_t bytes_transferred) {
                    if (!error) {
                        std::istream input(&buffer);
                        std::string response;
                        std::getline(input, response);
                        handler(response);
                    } else {
                        std::cerr << "[ERROR] Ошибка при получении сообщения: " << error.message() << std::endl;
                    }
                });
        } catch (const std::exception& e) {
            std::cerr << "[ERROR] Ошибка при настройке асинхронного получения: " << e.what() << std::endl;
        }
    }

    // Метод для закрытия соединения
    void close_connection() {
        try {
            socket.close();
            std::cout << "[INFO] Соединение закрыто." << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "[ERROR] Ошибка при закрытии соединения: " << e.what() << std::endl;
        }
    }

    // Получить сокет для использования в других операциях
    tcp::socket& get_socket() {
        return socket;
    }
};