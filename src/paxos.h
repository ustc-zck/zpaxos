#include <vector>
#include <string>
#include <map>
#include "kv.h"
#include "utils.h"
#include "server.cpp"
#include "client.cpp"

using namespace std::placeholders;


// void print(const boost::system::error_code& /*e*/, boost::asio::steady_timer* t, int* count){
//     if (*count < 10000) {
//         std::cout << *count << std::endl;
//         ++(*count);
//         t->expires_at(t->expiry() + boost::asio::chrono::seconds(1));
//         t->async_wait(boost::bind(print, boost::asio::placeholders::error, t, count));
//     }
// }


using namespace std;
class Paxos{
    public:
        Paxos();
        ~Paxos();
        //id = -1 represents reject, id > 1 represents accpet...
        std::pair<int64_t, std::string> Prepare(int64_t n);
        //int Promise();
        int Accept();
       
        //request vote... 
        int receiveRequestVote();

        //master ping...
        void ping(const boost::system::error_code& /*e*/, boost::asio::steady_timer* t);
        //timer event...
        void receivePing(std::string leader_addr);

        //happen when time out with ping...
        void receivePingTimeout();
        
        //set as leader
        void chosenAsLeader();
        //refer to resp protocol...
        std::string handler(const std::string& s);

        void isPingTimeOut(const boost::system::error_code& /*e*/, boost::asio::steady_timer* t);
    
        std::string GenerateProposalID(){
            return std::to_string(GetCurrentMicroSeconds()) + "_" + self;
        }

        void run();

    private:
        const static int MASTER = 0;
        const static int FOLLOWER = 1;
        const static int CANDIDATE = 2;

        const std::string REQUESTSIGN = "+";
        const std::string OPERATIONTYPE = "0";
        const std::string STRINGTYPE = "1";
        const std::string LONGTYPE = "2";
        const std::string DOUBLELONGTYPE = "3";
        const std::string INTTYPE = "4";

        const std::string RESPSIGN = "-";
        const std::string RESPERROR = "6";
        const std::string RESPOK = "7";

        int votes = 0;
        int role; 
        std::string self;
        static constexpr const char* NOLEADER = "NONE";
        std::string leader;
        KV* kv;
        int64_t maxProposalID; //max proposal id had accepted
        std::string maxProposalCommand; //command of maxProposalID
        int64_t minProposalID; //min proposal id
        std::map<int64_t, std::string> logCommands; //log commands
        std::vector<std::string> peers;
        int64_t lastReceivedTime; // //last time of receiving ping packet
        int64_t electionTimeOut;
        int64_t pingTimeOut;

        int isVoted;
        const static int HasVoted = 0;
        const static int NotVoted = 1;
        tcp_server* server;
        int port;
        boost::asio::io_context io;

        //commands
        std::map<std::string, std::string> commands;

        //accepted commands
        std::map<std::string, std::string> acceptedCommands;
};
