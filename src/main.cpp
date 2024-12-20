#include <iostream>

#include <boost/asio.hpp>
#include <iostream>
#include <string>
using boost::asio::ip::tcp;

int main()
{

try {
        boost::asio::io_context io_context;

        // Создаём endpoint и слушаем соединения на порту 8080
        tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), 8080));

        std::cout << "Сервер запущен, ожидание подключения..." << std::endl;

        for (;;) {
            // Принимаем соединение
            tcp::socket socket(io_context);
            acceptor.accept(socket);

            std::cout << "Клиент подключён!" << std::endl;

            // Отправляем данные клиенту
            std::string message = "Привет, клиент!";
            boost::asio::write(socket, boost::asio::buffer(message));

            std::cout << "Сообщение отправлено." << std::endl;
        }
    } catch (std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << std::endl;
    }

    return 0;
}
// #include <boost/asio.hpp>
// #include <iostream>
// #include <string>

// using boost::asio::ip::tcp;

// int main() {
//     try {
//         boost::asio::io_context io_context;

//         // Создаём endpoint и слушаем соединения на порту 8080
//         tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), 8080));

//         std::cout << "Сервер запущен, ожидание подключения..." << std::endl;

//         for (;;) {
//             // Принимаем соединение
//             tcp::socket socket(io_context);
//             acceptor.accept(socket);

//             std::cout << "Клиент подключён!" << std::endl;

//             // Отправляем данные клиенту
//             std::string message = "Привет, клиент!";
//             boost::asio::write(socket, boost::asio::buffer(message));

//             std::cout << "Сообщение отправлено." << std::endl;
//         }
//     } catch (std::exception& e) {
//         std::cerr << "Ошибка: " << e.what() << std::endl;
//     }

//     return 0;
// }
