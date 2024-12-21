#pragma once

#include <boost/asio.hpp>
#include <iostream>
#include <string>
#include <out_client.h>
#include <boost/asio.hpp>
#include <vector>
#include <string>
#include <iostream>
#include <memory>
#include "out_client.h"  // Включаем ваш класс Out_Client

using boost::asio::ip::tcp;

class Server {
private:
    boost::asio::io_context io_context_;
    tcp::acceptor acceptor_;
    std::vector<std::shared_ptr<Out_Client>> clients_;  // Вектор для хранения клиентов

public:
    // Конструктор, принимает порт и начинает слушать на этом порту
    Server(short port) 
        : acceptor_(io_context_, tcp::endpoint(tcp::v4(), port)) {}

    // Метод для запуска сервера и начала прослушивания
    void start() {
        std::cout << "[INFO] Сервер запущен, ожидаем подключения клиентов...\n";
        accept_connections();
        io_context_.run();
    }

private:
    // Метод для ожидания подключения клиента
    void accept_connections() {
        auto socket = std::make_shared<tcp::socket>(io_context_);
        acceptor_.async_accept(*socket, [this, socket](boost::system::error_code ec) {
            if (!ec) {
                // Клиент подключился, создаём объект Out_Client
                std::cout << "[INFO] Новый клиент подключился!\n";

                // Получаем логин клиента
                std::string login = receive_login(*socket);

                // Создаём объект Out_Client для этого клиента
                auto client = std::make_shared<Out_Client>(std::move(*socket), login);

                // Добавляем клиента в вектор
                clients_.push_back(client);

                // Отправляем приветственное сообщение клиенту
                client->send_message(Message("Привет, " + login + "!", "Server", login));

                // После подключения продолжаем ожидать новых клиентов
                accept_connections();
            }
        });
    }

    // Метод для получения логина от клиента
    std::string receive_login(tcp::socket& socket) {
        try {
            // Отправляем клиенту запрос на ввод логина
            std::string request = "Введите ваш логин:";
            boost::asio::write(socket, boost::asio::buffer(request));

            // Читаем логин от клиента
            char data[1024];
            size_t length = socket.read_some(boost::asio::buffer(data));
            std::string login(data, length);

            // Убираем возможный символ новой строки
            login.erase(std::remove(login.begin(), login.end(), '\n'), login.end());

            return login;
        } catch (const std::exception& e) {
            std::cerr << "[ERROR] Ошибка при получении логина: " << e.what() << std::endl;
            return "";
        }
    }
};
