#include "../src/client.cpp"


int main(){
    int count = 0;
    while(1){
        std::cout << "count is " << count << std::endl;
        Client* cli = new Client("127.0.0.1", "9000");
        std::string message("+");
        message += "\t";
        message += "0";
        message += "\t";
        message += "REQUESTVOTE";
        cli->Write(message);
        std::string data = cli->Read();
        std::cout << "read data is " << data << std::endl;
        //cli->~Client();
        delete cli;
        count++;
    }
    return 0;
}