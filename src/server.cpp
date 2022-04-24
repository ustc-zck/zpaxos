#include "server.h"

Server::Server(){
   
}

Server::~Server(){

}

void Server::WaitOnce(boost::function<void(const boost::system::error_code&)> f, int64_t milliseconds){
    boost::asio::io_context io_context;
    boost::asio::steady_timer t(io_context, boost::asio::chrono::milliseconds(milliseconds));
    t.async_wait(boost::bind(f, boost::asio::placeholders::error));
    io_context.run();
}

void Server::WaitRepeated(boost::function<void(const boost::system::error_code&, boost::asio::steady_timer*, int64_t)> f, int64_t milliseconds){
    boost::asio::io_context io_context;
    boost::asio::steady_timer t(io_context, boost::asio::chrono::milliseconds(milliseconds));
    t.async_wait(boost::bind(f, boost::asio::placeholders::error, &t, milliseconds));
    io_context.run();
}

