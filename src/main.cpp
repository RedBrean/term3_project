#include <boost/asio.hpp>
#include <iostream>
#include <memory>
#include <string>

using boost::asio::ip::tcp;

// Класс для обработки клиента
class Session : public std::enable_shared_from_this<Session> {
public:
    explicit Session(tcp::socket socket) 
        : socket_(std::move(socket)) {}

    void start() {
        // Начинаем обработку клиента
        do_write("Привет, клиент!\n");
    }

private:
    tcp::socket socket_; // Сокет для взаимодействия с клиентом

    void do_write(const std::string& message) {
        auto self = shared_from_this();
        boost::asio::async_write(socket_, boost::asio::buffer(message),
            [this, self](boost::system::error_code ec, std::size_t /*length*/) {
                if (!ec) {
                    std::cout << "Сообщение отправлено клиенту." << std::endl;
                } else {
                    std::cerr << "Ошибка отправки сообщения: " << ec.message() << std::endl;
                }
            });
    }
};

// Класс сервера
class Server {
public:
    Server(boost::asio::io_context& io_context, std::uint16_t port)
        : acceptor_(io_context, tcp::endpoint(tcp::v4(), port)) {
        do_accept();
    }

private:
    tcp::acceptor acceptor_;

    void do_accept() {
        acceptor_.async_accept(
            [this](boost::system::error_code ec, tcp::socket socket) {
                if (!ec) {
                    std::cout << "Клиент подключён!" << std::endl;
                    // Создаём новую сессию для клиента
                    std::make_shared<Session>(std::move(socket))->start();
                } else {
                    std::cerr << "Ошибка при подключении клиента: " << ec.message() << std::endl;
                }

                // Продолжаем принимать новые подключения
                do_accept();
            });
    }
};

int main() {
    try {
        boost::asio::io_context io_context;

        // Запускаем сервер на порту 8080
        Server server(io_context, 8080);

        std::cout << "Сервер запущен на порту 8080, ожидание подключений..." << std::endl;

        // Запускаем обработку событий
        io_context.run();
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
