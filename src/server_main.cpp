#include <boost/asio.hpp>
#include <iostream>
#include <string>

using boost::asio::ip::tcp;

int main() {
    boost::asio::io_context io_context;

    // Создаем сокет для подключения клиента
    tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), 12345));  // Порт 12345

    std::cout << "Server listening on 12345..." << std::endl;

    // Ожидаем подключения клиента
    tcp::socket socket(io_context);
    acceptor.accept(socket);

    std::cout << "Client connected!" << std::endl;

    // Буфер для приема данных
    char data[512];
    boost::system::error_code error;

    // Чтение данных от клиента
    size_t length = socket.read_some(boost::asio::buffer(data), error);

    if (error) {
        std::cout << "Error reading data: " << error.message() << std::endl;
    } else {
        std::cout << "Received message: " << std::string(data, length) << std::endl;
    }

    return 0;
}
