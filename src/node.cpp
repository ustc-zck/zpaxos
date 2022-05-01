#include "node.h"
#include "utils.h"

Node::Node(std::string addr_){
    addr = addr_;
    role = FOLLOWER;
    lastReceivedTime = GetCurrentMillSeconds();
    std::string consistent = ReadFromFile("../conf/consistency.txt");
    if (consistent == ""){
        maxProposalID = -1;
        maxProposalCommand = "";
        minProposalID = -1;
    } else{
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
}

Node::~Node(){
    delete kv;
}

std::string Node::Get(std::string key){
    return kv->Get(key);
}

void Node::Put(std::string key, std::string val){
    return kv->Put(key, val);
}

void Node::Del(std::string key){
    return kv->Del(key);    
}





