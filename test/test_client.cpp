#include "../src/client.cpp"


int main(){
    Client* cli = new Client("127.0.0.1", "9001");
    std::string message("GET\tb");
    cli->Write(message);
    std::string data = cli->Read();
    std::cout << "read data is " << data << std::endl;
    delete cli;
    return 0;
}