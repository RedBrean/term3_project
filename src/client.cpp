#include <boost/asio.hpp>
#include <iostream>
#include <string>

using boost::asio::ip::tcp;

int main() {
    boost::asio::io_context io_context;

    // Создаем сокет для подключения к серверу
    tcp::socket socket(io_context);

    // Подключаемся к серверу по адресу localhost (127.0.0.1) и порту 12345
    tcp::resolver resolver(io_context);
    boost::asio::connect(socket, resolver.resolve("127.0.0.1", "12345"));

    std::string message = "Привет, мир!";

    // Отправляем сообщение серверу
    boost::asio::write(socket, boost::asio::buffer(message));

    std::cout << "Message sent to server: " << message << std::endl;

    return 0;
}
