#pragma once

#include <iostream>
#include <boost/asio.hpp>
#include <string>
#include <functional>  // Для использования std::function
#include <message.h>   // Предполагаем, что этот заголовочный файл существует

using boost::asio::ip::tcp;

class Out_Client {
private:
    std::string login_;       // Логин клиента
    bool is_connected_;       // Подключён ли клиент
    tcp::socket socket_;      // Сокет клиента

public:
    // Конструктор принимает готовый сокет и логин клиента
    Out_Client(tcp::socket&& socket, const std::string& login)
        : login_(login), is_connected_(true), socket_(std::move(socket)) {}

    // Геттеры
    const std::string& get_login() const {
        return login_;
    }

    bool is_connected() const {
        return is_connected_;
    }

    tcp::socket& get_socket() {
        return socket_;
    }

    // Метод для отключения клиента
    void disconnect() {
        if (is_connected_) {
            socket_.close();
            is_connected_ = false;
        }
    }

    // Метод для отправки сообщения
    void send_raw_message(const Message& message) {
        if (is_connected_) {
            try {
                std::string message_str = message.to_string();  // Получаем строковое представление сообщения
                boost::asio::write(socket_, boost::asio::buffer(message_str));  // Отправляем сообщение через сокет
                std::cout << "[INFO] Сообщение отправлено: " << message.get_text() << std::endl;
            } catch (const std::exception& e) {
                std::cerr << "[ERROR] Ошибка при отправке сообщения: " << e.what() << std::endl;
            }
        } else {
            std::cerr << "[ERROR] Клиент не подключён!" << std::endl;
        }
    }

    // Метод для асинхронного получения сообщения от сервера
    void async_receive_message(std::function<void(const Message&)> handler) {
        if (is_connected_) {
            try {
                // Используем streambuf для асинхронного чтения
                boost::asio::streambuf buffer;

                // Читаем данные до символа новой строки
                boost::asio::async_read_until(socket_, buffer, '\n',
                    [this, handler, &buffer](const boost::system::error_code& error, std::size_t bytes_transferred) {
                        if (!error) {
                            std::istream input(&buffer);
                            std::string received_message;
                            std::getline(input, received_message);
                            Message message = Message::from_string(received_message); // Создаем объект Message
                            handler(message);  // Вызываем обработчик с полученным сообщением
                        } else {
                            std::cerr << "[ERROR] Ошибка при получении сообщения: " << error.message() << std::endl;
                        }
                    });
            } catch (const std::exception& e) {
                std::cerr << "[ERROR] Ошибка при настройке асинхронного получения: " << e.what() << std::endl;
            }
        } else {
            std::cerr << "[ERROR] Клиент не подключён!" << std::endl;
        }
    }

};  // Класс завершен
