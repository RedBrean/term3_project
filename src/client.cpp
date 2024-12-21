#include <boost/asio.hpp>
#include <iostream>

using boost::asio::ip::tcp;

int main() {
    try {
        boost::asio::io_context io_context;

        // Устанавливаем соединение с сервером
        tcp::resolver resolver(io_context);
        auto endpoints = resolver.resolve("94.131.15.248", "12345"); // Замените "127.0.0.1" на IP сервера
        tcp::socket socket(io_context);
        boost::asio::connect(socket, endpoints);

        // Отправляем сообщение "Привет мир"
        const std::string message = "Привет мир";
        boost::asio::write(socket, boost::asio::buffer(message));

        std::cout << "Сообщение отправлено: " << message << std::endl;

    } catch (std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << std::endl;
    }

    return 0;
}
