#include "../src/server.cpp"

std::string echo(const std::string& s){
    return s;
}

void print(){
    std::cout << "Hello, world!" << std::endl;
}

void print1(){
    std::cout << "Hello, world1!" << std::endl;
}

//g++ -o test_server test_server.cpp ../src/server.cpp -lpthread
int main(){
    boost::asio::io_context io_context;

    tcp_server* s = new tcp_server(io_context, echo, 8080);
    //io_context.run();
    s->run();

    delete s;
    return 0;
}