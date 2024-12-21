#pragma once

#include <iostream>
#include <boost/asio.hpp>
#include <vector>
#include <string>
#include <algorithm>  // Для std::remove_if
#include <message.h>  // Ваш заголовочный файл, содержащий определение Message

#include <out_client.h>

using boost::asio::ip::tcp;

class Server {
private:
    boost::asio::io_context& io_context_;  // Контекст ввода/вывода
    tcp::acceptor acceptor_;               // Акцептор для подключения клиентов
    std::vector<Out_Client> clients_;      // Вектор клиентов (не указатели)
    boost::asio::steady_timer cleanup_timer_;  // Таймер для очистки неподключенных клиентов
    bool is_accepting_;  // Флаг для проверки состояния акцептора

public:
    Server(boost::asio::io_context& io_context, unsigned short port)
        : io_context_(io_context),
          acceptor_(io_context, tcp::endpoint(tcp::v4(), port)),
          cleanup_timer_(io_context),
          is_accepting_(false) {  // Изначально акцептор не запущен
        std::cout << "[INFO] Инициализация сервера на порту " << port << std::endl;
    }

    // Синхронный прием нового подключения
    void accept_connection() {
        std::cout << "[INFO] Начало прослушивания на порту: " << acceptor_.local_endpoint().port() << std::endl;
        is_accepting_ = true;  // Устанавливаем флаг, что акцептор теперь работает

        // Создаем сокет для нового клиента
        tcp::socket socket(io_context_);

        try {
            // Блокирует выполнение, пока не примет новое соединение
            acceptor_.accept(socket);

            // Новый клиент подключился, создаем объект Out_Client
            clients_.emplace_back(std::move(socket), "client_login");
            std::cout << "[INFO] Новый клиент подключился. Количество подключённых клиентов: " << clients_.size() << std::endl;

            // Запускаем асинхронное чтение сообщений от этого клиента
            start_read(clients_.back());  // Используем ссылку на последнего клиента
        } catch (const boost::system::system_error& e) {
            std::cerr << "[ERROR] Ошибка при принятии соединения: " << e.what() << std::endl;
        }
    }

    // Асинхронное чтение сообщения от клиента
    void start_read(Out_Client& client) {
        client.async_receive_message([this, &client](const Message& message) {
            if (client.is_connected()) {
                // Обрабатываем полученное сообщение
                std::cout << "[INFO] Получено сообщение от клиента: " << std::endl;
                std::cout << message.get_text() << std::endl;
                
                // Отправляем это сообщение всем клиентам
                broadcast_message(message);
                
                // Снова запускаем асинхронное чтение для текущего клиента
                start_read(client);
            } else {
                std::cout << "[INFO] Клиент отключился, удаляем из списка." << std::endl;
                remove_client(client);
            }
        });
    }

    // Отправка сообщения всем подключённым клиентам
    void broadcast_message(const Message& message) {
        for (auto& client : clients_) {
            if (client.is_connected()) {
                client.send_raw_message(message);
            }
        }
    }

    // Функция для удаления неподключенных клиентов
    void remove_disconnected_clients() {
        size_t before_cleanup = clients_.size();
        clients_.erase(std::remove_if(clients_.begin(), clients_.end(),
            [](Out_Client& client) { return !client.is_connected(); }), clients_.end());
        size_t after_cleanup = clients_.size();
        
        std::cout << "[INFO] Очищены отключенные клиенты. Количество удалённых клиентов: " << (before_cleanup - after_cleanup) << std::endl;
        std::cout << "[INFO] Осталось подключённых клиентов: " << after_cleanup << std::endl;
    }

    // Удаление клиента из вектора вручную
    void remove_client(Out_Client& client) {
        if (client.is_connected()) {
            clients_.erase(std::remove(clients_.begin(), clients_.end(), client), clients_.end());
            std::cout << "[INFO] Клиент удалён из списка. Количество подключённых клиентов: " << clients_.size() << std::endl;
        }
    }

    // Таймер для регулярной очистки неподключенных клиентов
    void start_cleanup_timer() {
        cleanup_timer_.expires_from_now(boost::asio::chrono::seconds(40));  // 40 секунд
        cleanup_timer_.async_wait([this](const boost::system::error_code&) {
            remove_disconnected_clients();  // Очищаем неподключенных клиентов
            start_cleanup_timer();  // Перезапускаем таймер
        });
    }

    // Функция для начала прослушивания всех пользователей
    void start_listening() {
        std::cout << "[INFO] Запуск прослушивания всех пользователей..." << std::endl;
        accept_connection();  // Запускаем синхронный прием подключений
        start_cleanup_timer();  // Запускаем таймер для очистки клиентов
    }

    // Метод для запуска сервера
    void run() {
        std::cout << "[INFO] Запуск сервера..." << std::endl;
        start_listening();  // Начинаем прослушивание всех пользователей
        io_context_.run();
    }

    // Метод для отключения всех клиентов
    void stop() {
        for (auto& client : clients_) {
            client.disconnect();
        }
        acceptor_.close();  // Закрываем акцептор
        std::cout << "[INFO] Все клиенты отключены, акцептор закрыт." << std::endl;
    }
};
