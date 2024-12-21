#include <boost/asio.hpp>
#include <iostream>

using boost::asio::ip::tcp;

int main() {
    try {
        boost::asio::io_context io_context;

        // Создаём TCP-акцептор для прослушивания входящих соединений
        tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), 12345));
        std::cout << "Сервер запущен и слушает порт 12345..." << std::endl;

        // Принимаем соединение
        tcp::socket socket(io_context);
        acceptor.accept(socket);

        // Читаем сообщение от клиента
        char data[1024];
        size_t length = socket.read_some(boost::asio::buffer(data));
        std::cout << "Получено сообщение: " << std::string(data, length) << std::endl;

    } catch (std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << std::endl;
    }

    return 0;
}
