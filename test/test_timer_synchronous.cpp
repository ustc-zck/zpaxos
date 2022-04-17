#include <iostream>
#include <boost/asio.hpp>

//g++ -o test_asio test_asio.cpp -ldl -lpthread -lboost_system
int main(){
    boost::asio::io_context io;
    boost::asio::steady_timer t(io, boost::asio::chrono::seconds(5));
    t.wait();
    std::cout << "Hello, world!" << std::endl;
    return 0;
}

