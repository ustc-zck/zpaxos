#include "../../src/server.h"
#include <iostream>

// void print(const boost::system::error_code& /*e*/)
// {
//   std::cout << "Hello, world!" << std::endl;
// }

// // g++ -o test_server test_server.cpp ../src/server.cpp -lpthread
// int main(){
//     Server* s = new Server();
//     s->WaitOnce(boost::bind(print, boost::asio::placeholders::error), 1000);
//     delete s;
//     return 0;
// }

void print(const boost::system::error_code& e, boost::asio::steady_timer*t, int64_t milliseconds)
{
  std::cout << "Hello, world!" << std::endl;
  t->expires_at(t->expiry() + boost::asio::chrono::milliseconds(milliseconds));
  t->async_wait(boost::bind(print, boost::asio::placeholders::error, t, milliseconds));
}

using namespace boost::placeholders;

int main(){
    Server * s = new Server();
    s->WaitRepeated(print, 1000);
    delete s;
    return 0;
}


