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
    // Метод для отправки сообщения
void send_message(const Message& message) {
    if (is_connected_) {
        std::string message_str = message.to_string();  // Получаем строковое представление сообщения
        boost::asio::write(socket_, boost::asio::buffer(message_str));  // Отправляем сообщение через сокет
        std::cout << "[INFO] Сообщение отправлено: " << message.get_text() << std::endl;
    } else {
        std::cerr << "[ERROR] Клиент не подключён!" << std::endl;
    }
}

// Метод для получения сообщения
Message receive_message() {
    if (is_connected_) {
        char data[1024];
        size_t length = socket_.read_some(boost::asio::buffer(data));  // Чтение данных из сокета
        std::string received_message(data, length);  // Преобразуем полученные данные в строку
        std::cout << "[INFO] Сообщение получено: " << received_message << std::endl;
        return Message::from_string(received_message);  // Создаём объект Message из строки
    } else {
        std::cerr << "[ERROR] Клиент не подключён!" << std::endl;
        return Message();  // Возвращаем пустое сообщение, если клиент не подключён
    }
}

};
