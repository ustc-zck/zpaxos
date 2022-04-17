#include <string>
#include <set>
#include <map>
#include "kv.h"

class Node{
    public:
        Node(std::string addr_);
        ~Node();
        //db operation, get, put and del
        std::string Get(std::string key);
        void Put(std::string key, std::string val);
        void Del(std::string key);
        const static int MASTER = 0;
        const static int FOLLOWER = 1;
        const static int CANDIDATE = 2;
        int role; 
        int64_t maxProposalID; //max proposal id had accepted
        std::string maxProposalCommand; //command of maxProposalID
        int64_t minProposalID; //min proposal id
        std::map<int64_t, std::string> logCommands; //log commands
        uint64_t lastReceivedTime; //last time of receiving packet
    private:
        KV* kv;
        std::string addr;
};