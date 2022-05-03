#include "../src/client.cpp"


int main(){
    Client* cli = new Client("127.0.0.1", "8080");
    std::string message("+");
    message += "\t";
    message += "0";
    message += "\t";
    message += "REQUESTVOTE";
    cli->Write(message);
    std::string data = cli->Read();
    std::cout << "read data is " << data << std::endl;
    return 0;
}