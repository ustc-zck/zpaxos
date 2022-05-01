#include "paxos.h"

Paxos::Paxos(){
    std::string conf = ReadFromFile("../conf/config.conf");
    std::vector<std::string> itmes = SplitStr(conf, '\n');
    for(auto item : itmes){
        std::vector<std::string> kv = SplitStr(item, ':');
        if(trim(kv[0]) == "addr"){
            self = trim(kv[1]);
        } else if(trim(kv[0]) == "peers"){
            peers.push_back(trim(kv[1]));
        } else if(trim(kv[0]) == "election_time_out"){
            electionTimeOut = std::stoull(trim(kv[1]));
        } else{
            //add more conf item...
        }
    }

    role = FOLLOWER;
    std::string consistent = ReadFromFile("../conf/consistency.txt");
    if (consistent == ""){
        maxProposalID = -1;
        maxProposalCommand = "";
        minProposalID = -1;
    } else {
        std::vector<std::string> items = SplitStr(consistent, '\n');
        for(auto item : items){
            std::vector<std::string> kv = SplitStr(item, ':');
            if(trim(kv[0]) == "maxProposalID"){
                maxProposalID = std::stoull(trim(kv[1]));
            } else if(trim(kv[0]) == "maxProposalCommand"){
                maxProposalCommand = trim(kv[1]);
            } else if(trim(kv[0]) == "minProposalID"){
                minProposalID = std::stoull(trim(kv[1]));
            } else {
                //TODO, add more consistent item...
            }
        }
    }
    kv = new KV();
    hasVoted = NotVoted;
    leader = NOLEADER;
    lastReceivedTime = GetCurrentMillSeconds();
}

Paxos::~Paxos(){
    delete kv;
}

std::pair<int64_t, std::string> Paxos::Prepare(int64_t n){

}

int Paxos::Accept(){

}