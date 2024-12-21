#include <boost/asio.hpp>
#include <iostream>
#include <string>

#include <fstream>
#include <nlohmann/json.hpp>


#include <server.h>
#include <out_client.h>


using json = nlohmann::json;

using boost::asio::ip::tcp;



int main() {

    boost::asio::io_context io_context;
    Server this_server(io_context, 12345);
    this_server.run();
    

    return 0;
}
