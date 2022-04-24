#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>
#include <boost/function.hpp> 

class Server{
    public:
        Server();
        ~Server();
        void WaitOnce(boost::function<void(const boost::system::error_code&)> f, int64_t milliseconds);
        void WaitRepeated(boost::function<void(const boost::system::error_code&, boost::asio::steady_timer*, int64_t)> f, int64_t milliseconds);
        void Run();       
    private:
               
};