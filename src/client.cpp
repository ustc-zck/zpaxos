#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <string>

using boost::asio::ip::tcp;

class Client{
    public:   
        Client(std::string ip_, std::string port_):ip(ip_), port(port_), socket(io_context){
            tcp::resolver resolver(io_context);
            tcp::resolver::results_type endpoints = resolver.resolve(ip, port);
            tcp::socket tmp(io_context);
            //std::move...
            socket = std::move(tmp);
            boost::asio::connect(socket, endpoints);
        }
        ~Client(){
            if(socket.is_open()){
                socket.close();
            }
        }

        int Write(std::string data){
            //std::cout << "write data into socket: " << data << std::endl; 
            boost::system::error_code error;
            socket.write_some(boost::asio::buffer(data), error);
            if(error){
                return -1;
            }
            return 0;
        }
        std::string Read(){
            std::string data;
            for (;;){
                boost::array<char, 128> buf;
                boost::system::error_code error;
                size_t len = socket.read_some(boost::asio::buffer(buf), error);
                if (error == boost::asio::error::eof)
                    break; // Connection closed cleanly by peer
                else if (error)
                    throw boost::system::system_error(error); // Some other error.
                std::string tmp(buf.begin(), buf.begin() + len);
                data += tmp;
            }  
            //std::cout << "read data from socket " << data << std::endl;
            return data;
        }
    private:
        std::string ip;
        std::string port;
        boost::asio::io_context io_context;
        tcp::socket socket;
};