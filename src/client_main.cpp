#include <boost/asio.hpp>
#include <iostream>
#include <string>

#include <fstream>
#include <nlohmann/json.hpp>


#include <client.h>
#include <out_server.h>


using json = nlohmann::json;

using boost::asio::ip::tcp;



int main() {

    boost::asio::io_context io_context;
    

    read_settings();
    Out_Server my_server(io_context);
    my_server.connect_to_server(SERVER_ADDRESS, SERVER_PORT);

    auto timer = boost::asio::steady_timer(io_context, boost::asio::chrono::seconds(2));
    timer.wait();
    my_server.close_connection();

    return 0;
}
