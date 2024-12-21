#include <boost/asio.hpp>
#include <iostream>
#include <string>

using boost::asio::ip::tcp;

std::string get_local_ip(boost::asio::io_context& io_context) {
    // Создаем resolver для поиска интерфейсов
    boost::asio::ip::udp::resolver resolver(io_context);
    
    // Получаем локальный адрес по умолчанию
    boost::asio::ip::udp::resolver::query query(boost::asio::ip::udp::v4(), "localhost", "");
    auto endpoints = resolver.resolve(query);
    
    // Берем первый доступный адрес (он должен быть локальным IP)
    return endpoints.begin()->endpoint().address().to_string();
}

int main() {
    boost::asio::io_context io_context;

    // Получаем локальный IP-адрес
    std::string local_ip = get_local_ip(io_context);
    std::cout << "Server running on local IP: " << local_ip << std::endl;

    // Создаем серверный сокет
    tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), 12345));
    
    std::cout << "Server listening on port 12345..." << std::endl;

    // Ожидаем подключения
    tcp::socket socket(io_context);
    acceptor.accept(socket);

    std::cout << "Client connected!" << std::endl;

    return 0;
}
