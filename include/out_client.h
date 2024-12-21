#pragma once

#include <iostream>
#include <boost/asio.hpp>
#include <string>

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

    // Метод для синхронного отправления сообщения
    void send_raw_message(const std::string& message) {
        if (is_connected_) {
            try {
                boost::asio::write(socket_, boost::asio::buffer(message + "\n"));  // Отправляем сообщение через сокет
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
        if (is_connected_) {
            try {
                // Создаем буфер для хранения полученных данных
                boost::asio::streambuf buffer;
                std::string received_message;

                // Читаем данные из сокета (предполагаем, что данные заканчиваются символом новой строки)
                boost::asio::read(socket_, buffer, boost::asio::transfer_at_least(1));  // Читаем хотя бы один байт

                // Извлекаем строку из буфера
                std::istream input(&buffer);
                std::getline(input, received_message);

                return received_message;  // Возвращаем полученную строку
            } catch (const boost::system::system_error& e) {
                std::cerr << "[ERROR] Ошибка при получении сообщения: " << e.what() << std::endl;
                disconnect();  // Отключаем клиента при ошибке
            }
        }
        return "";  // Если клиент не подключен или произошла ошибка
    }

};
