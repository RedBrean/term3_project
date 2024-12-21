#pragma once

#include <iostream>
#include <boost/asio.hpp>
#include <string>
#include <memory>

using boost::asio::ip::tcp;

class Out_Client {
private:
    std::string login_;                         // Логин клиента
    bool is_connected_;                         // Подключён ли клиент
    std::shared_ptr<tcp::socket> socket_;       // Сокет клиента

public:
    // Конструктор принимает rvalue socket и логин клиента
    Out_Client(tcp::socket&& socket, const std::string& login)
        : login_(login), is_connected_(true), socket_(std::make_shared<tcp::socket>(std::move(socket))) {}

    // Конструктор копирования
    Out_Client(const Out_Client& other)
        : login_(other.login_), is_connected_(other.is_connected_), socket_(other.socket_) {}

    // Оператор сравнения клиентов по логину
    bool operator==(const Out_Client& other) const {
        return login_ == other.login_;  // Сравниваем по логину
    }

    // Геттеры
    const std::string& get_login() const {
        return login_;
    }

    bool is_connected() const {
        return is_connected_;
    }

    std::shared_ptr<tcp::socket> get_socket() {
        return socket_;
    }

    // Метод для отключения клиента
    void disconnect() {
        if (is_connected_) {
            try {
                socket_->close();
                is_connected_ = false;
            } catch (const std::exception& e) {
                std::cerr << "[ERROR] Ошибка при отключении: " << e.what() << std::endl;
            }
        }
    }

    // Метод для синхронного отправления сообщения
    void send_raw_message(const std::string& message) {
        if (is_connected_) {
            try {
                boost::asio::write(*socket_, boost::asio::buffer(message + "\n"));  // Отправляем сообщение через сокет
                std::cout << "[INFO] Сообщение отправлено: " << message << std::endl;
            } catch (const std::exception& e) {
                std::cerr << "[ERROR] Ошибка при отправке сообщения: " << e.what() << std::endl;
            }
        } else {
            std::cerr << "[ERROR] Клиент не подключён!" << std::endl;
        }
    }

    // Метод для синхронного получения сообщения от сервера
    std::string receive_raw_message() {
        if (!is_connected_) {
            std::cerr << "[ERROR] Клиент не подключён!" << std::endl;
            return "";
        }

        try {
            boost::asio::streambuf buffer;
            boost::asio::read_until(*socket_, buffer, "||E");  // Читаем данные до символа конца строки

            std::istream stream(&buffer);
            std::string message;
            std::getline(stream, message);

            std::cout << "[INFO] Сообщение получено: " << message << std::endl;
            return message;
        } catch (const std::exception& e) {
            std::cerr << "[ERROR] Ошибка при получении сообщения: " << e.what() << std::endl;
            return "";
        }
    }
};
