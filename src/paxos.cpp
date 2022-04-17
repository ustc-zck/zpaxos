#include "paxos.h"
#include "utils.h"

Paxos::Paxos(){
    std::string conf = ReadFromFile("../conf/config.conf");
    std::vector<std::string> itmes = SplitStr(conf, '\n');
        for(auto item : itmes){
            std::vector<std::string> kv = SplitStr(item, ':');
            if(trim(kv[0]) == "addr"){
                self = new Node(trim(kv[1]));
            } else if(trim(kv[0]) == "peers"){
                peers.push_back(trim(kv[1]));
            } else if(trim(kv[0]) == "election_time_out"){
                electionTimeOut = std::stoull(trim(kv[1]));
            } else{
                //add more conf item...
            }
        }
}

Paxos::~Paxos(){
    delete self;
}

std::pair<int64_t, std::string> Paxos::Prepare(int64_t n){

}

int Paxos::Accept(){

}

int Paxos::Ping(){

}