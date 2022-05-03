#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <string>


// using boost::asio::ip::tcp;

// tcp::socket InitializeClient(std::string ip, std::string port){
//     boost::asio::io_context io_context;

//     tcp::resolver resolver(io_context);
//     tcp::resolver::results_type endpoints =
//       resolver.resolve(ip, port);

//     tcp::socket socket(io_context);
//     boost::asio::connect(socket, endpoints);
//     return socket;
// }

// int ClientWrite(tcp::socket socket, std::string data){
//     boost::system::error_code error;
//     socket.write_some(boost::asio::buffer(data), error);
//     if(error){
//         return -1;
//     }
//     return 0;
// }

// std::string ClientRead(tcp::socket socket){
//     std::string data;
//     for (;;){
//         boost::array<char, 128> buf;
//         boost::system::error_code error;
//         size_t len = socket.read_some(boost::asio::buffer(buf), error);
//         std::cout << "message len is " << len << std::endl;
//         if (error == boost::asio::error::eof)
//             break; // Connection closed cleanly by peer
//         else if (error)
//             throw boost::system::system_error(error); // Some other error.
//         std::string tmp(buf.begin(), buf.begin() + len);
//         data += tmp;
//     }  
//     return data;
// }

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
            delete &socket;
        }

        int Write(std::string data){
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
                // std::cout << "message len is " << len << std::endl;
                if (error == boost::asio::error::eof)
                    break; // Connection closed cleanly by peer
                else if (error)
                    throw boost::system::system_error(error); // Some other error.
                std::string tmp(buf.begin(), buf.begin() + len);
                data += tmp;
            }  
            return data;
        }
    private:
        std::string ip;
        std::string port;
        boost::asio::io_context io_context;
        tcp::socket socket;
};