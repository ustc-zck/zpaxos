#include "../src/client.cpp"


int main(){
    Client* cli1 = new Client("127.0.0.1", "9000");
    Client* cli = new Client("127.0.0.1", "9000");
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