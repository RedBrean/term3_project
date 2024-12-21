#pragma once


#include <message.h>



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
};
