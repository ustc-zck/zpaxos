#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>
#include <boost/function.hpp>


void print(const boost::system::error_code& /*e*/)
{
  std::cout << "Hello, world!" << std::endl;
}

void TimerAsynchronous(boost::function<void(const boost::system::error_code&)> func){
    boost::asio::io_context io;
    boost::asio::steady_timer t(io, boost::asio::chrono::seconds(5));
    t.async_wait(boost::bind(print, boost::asio::placeholders::error));
    io.run();
}

int main(){
    TimerAsynchronous(print);
    return 0;
}
