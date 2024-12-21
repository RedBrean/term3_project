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
    Server this_server(12345);


    return 0;
}
