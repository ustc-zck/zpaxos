#include "../src/client.cpp"

int main(){
    Client* cli = new Client("127.0.0.1", "8080");
    cli->Write("test client test client test client");
    std::string data = cli->Read();
    std::cout << "read data is " << data << std::endl;
    return 0;
}