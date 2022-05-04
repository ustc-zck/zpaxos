#include "../src/client.cpp"


int main(){
    Client* cli = new Client("127.0.0.1", "9000");
    std::string message("PUT\ta\tc");
    cli->Write(message);
    std::string data = cli->Read();
    std::cout << "read data is " << data << std::endl;
    delete cli;
    return 0;
}