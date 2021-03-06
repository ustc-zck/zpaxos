//
// client.cpp
// ~~~~~~~~~~
//
// Copyright (c) 2003-2021 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

//g++ -o client client.cpp -lpthread
int main(int argc, char* argv[])
{
  try
  {
    // if (argc != 2)
    // {
    //   std::cerr << "Usage: client <host>" << std::endl;
    //   return 1;
    // }

    boost::asio::io_context io_context;

    tcp::resolver resolver(io_context);
    tcp::resolver::results_type endpoints =
      resolver.resolve("127.0.0.1", "8080");

    tcp::socket socket(io_context);
    boost::asio::connect(socket, endpoints);

    for (;;)
    {
      boost::array<char, 128> buf;
      boost::system::error_code error;

      socket.write_some(boost::asio::buffer("test echo"), error);
      size_t len = socket.read_some(boost::asio::buffer(buf), error);
      std::cout << "message len is " << len << std::endl;
      if (error == boost::asio::error::eof)
        break; // Connection closed cleanly by peer
      else if (error)
        throw boost::system::system_error(error); // Some other error.

      //std::cout.write(buf.data(), len);
      std::string data(buf.begin(), buf.begin() + len);
      std::cout << "receive message is " << data << std::endl;
    }
  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }
  return 0;
}