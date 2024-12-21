#pragma once

#include <iostream>
#include <boost/asio.hpp>
#include <vector>
#include <string>
#include <thread>
#include "out_client.h"  // Подключаем класс клиента

using boost::asio::ip::tcp;

class Server {
private:
    boost::asio::io_context& io_context_;      // Контекст ввода/вывода
    tcp::acceptor acceptor_;                  // Акцептор для подключения клиентов
    std::vector<Out_Client> clients_;         // Список подключённых клиентов

public:
    // Конструктор сервера
    Server(boost::asio::io_context& io_context, unsigned short port)
        : io_context_(io_context), acceptor_(io_context, tcp::endpoint(tcp::v4(), port)) {
        std::cout << "[INFO] Инициализация сервера на порту " << port << std::endl;
    }

    // Метод для начала прослушивания подключений
    void start_accept() {
        std::cout << "[INFO] Начало прослушивания на порту: " << acceptor_.local_endpoint().port() << std::endl;

        while (true) {
            // Принимаем подключение клиента
            tcp::socket socket(io_context_);
            acceptor_.accept(socket);  // Это блокирующая операция
            std::cout << "[INFO] Новый клиент подключён!" << std::endl;

            // Получаем логин клиента (для примера, создаём логин на основе номера подключения)
            std::string login = "Client_" + std::to_string(clients_.size() + 1);

            // Создаём объект клиента и добавляем в список
            Out_Client new_client(std::move(socket), login);
            clients_.push_back(std::move(new_client));

            // Обрабатываем сообщения от клиента в отдельном потоке
            std::thread([this, &new_client]() {
                process_client_messages(new_client);
            }).detach();
        }
    }

    // Метод для обработки сообщений от клиента
    void process_client_messages(Out_Client& client) {
        while (client.is_connected()) {
            std::string message = client.receive_raw_message();  // Получаем сообщение от клиента
            if (!message.empty()) {
                std::cout << "[INFO] Получено сообщение от клиента " << client.get_login() << ": " << message << std::endl;
            } else {
                std::cout << "[INFO] Клиент " << client.get_login() << " отключён." << std::endl;
                break;
            }
        }
    }

    // Метод для отправки сообщения всем подключённым клиентам
    void broadcast_message(const std::string& message) {
        for (auto& client : clients_) {
            if (client.is_connected()) {
                client.send_raw_message(message);
                std::cout << "[INFO] Сообщение отправлено клиенту: " << client.get_login() << std::endl;
            }
        }
    }

    // Метод для запуска сервера
    void run() {
        std::cout << "[INFO] Запуск сервера..." << std::endl;
        start_accept();  // Начинаем прослушивание входящих подключений
        io_context_.run();  // Запуск обработки ввода/вывода
    }

    // Метод для отключения всех клиентов
    void stop() {
        for (auto& client : clients_) {
            client.disconnect();
        }
        std::cout << "[INFO] Все клиенты отключены." << std::endl;
    }
};
