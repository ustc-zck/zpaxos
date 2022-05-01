#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>


void print(const boost::system::error_code& /*e*/)
{
  std::cout << "Hello, world!" << std::endl;
}

//g++ -o test_timer_asynchronous test_timer_asynchronous.cpp -lpthread
int main(){
    boost::asio::io_context io;
    boost::asio::steady_timer t(io, boost::asio::chrono::seconds(5));
    t.async_wait(boost::bind(print, boost::asio::placeholders::error));
    io.run();
    return 0;
}
