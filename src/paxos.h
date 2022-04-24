#include <vector>
#include <string>
#include <map>
#include "kv.h"
#include "utils.h"

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
        int receiveRequestVote(std::string peer){
            if(hasVoted == NotVoted && role == CANDIDATE && leader == NOLEADER){
                hasVoted = HasVoted;
                return 1;
            }
            return 0;
        }

        //timer event...
        int receivePing(std::string leader_addr){
            lastReceivedTime = GetCurrentMillSeconds();
            if(leader == NOLEADER){
                leader = leader_addr;
                role = FOLLOWER;
            }
        }
        //happen when time out with ping...
        void receivePingTimeout(){
            role = CANDIDATE;
            hasVoted = NotVoted;
            leader = NOLEADER;
        }

    private:
        const static int MASTER = 0;
        const static int FOLLOWER = 1;
        const static int CANDIDATE = 2;
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

        int hasVoted;
        const static int HasVoted = 0;
        const static int NotVoted = 1;
};