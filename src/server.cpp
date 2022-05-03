#include <ctime>
#include <iostream>
#include <string>
#include <boost/bind/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <functional>
#include <boost/function.hpp>


using boost::asio::ip::tcp;
using namespace std::placeholders; 

class tcp_connection : public boost::enable_shared_from_this<tcp_connection> {
public:
    typedef boost::shared_ptr<tcp_connection> pointer;
    static pointer create(boost::asio::io_context& io_context, std::function<std::string(std::string)> handler_){
        return pointer(new tcp_connection(io_context, handler_));
    }

    tcp::socket& socket(){
        return socket_;
    }

    //handler...
    void start() {
        std::cout << "accept socket" << std::endl;

        //test read data...
        boost::array<char, 128> buf;
        boost::system::error_code error;

        size_t len = socket_.read_some(boost::asio::buffer(buf), error);
        std::string data(buf.begin(), buf.begin() + len);
        std::cout << "server receive message " << data << std::endl;
        message_ = handler(data);
        
        
        boost::asio::async_write(socket_, boost::asio::buffer(message_), 
            boost::bind(&tcp_connection::handle_write, shared_from_this(),
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred));
    }

private:
    tcp_connection(boost::asio::io_context& io_context, std::function<std::string(std::string)> handler_) : socket_(io_context), handler(handler_) {
    }

    void handle_write(const boost::system::error_code& /*error*/, size_t /*bytes_transferred*/){
    }

    tcp::socket socket_;
    std::string message_;
    std::function<std::string(std::string)> handler;
};


class tcp_server {
public:
    tcp_server(boost::asio::io_context& io_context, std::function<std::string(std::string)> handler_, int port_) : io_context_(io_context), 
        handler(handler_),
        acceptor_(io_context, tcp::endpoint(tcp::v4(), port_)){
            std::cout << "server is listening on port: " << port_ << std::endl;
            start_accept();
    }
    
    void run(){
        io_context_.run();
    }

private:
    void start_accept(){
        tcp_connection::pointer new_connection = tcp_connection::create(io_context_, handler);
        acceptor_.async_accept(new_connection->socket(), 
            boost::bind(&tcp_server::handle_accept, this, new_connection, boost::asio::placeholders::error));
    }

    void handle_accept(tcp_connection::pointer new_connection, const boost::system::error_code& error){
        if (!error) {
            new_connection->start();
        }
        start_accept();
    }

    boost::asio::io_context& io_context_;
    tcp::acceptor acceptor_;
    std::function<std::string(std::string)> handler;
};

