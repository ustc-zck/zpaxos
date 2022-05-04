#include "paxos.h"

Paxos::Paxos(){
    std::string conf = ReadFromFile("../conf/config.conf");
    std::vector<std::string> itmes = SplitStr(conf, '\n');
    for(auto item : itmes){
        auto name = item.substr(0, item.find(":"));
        if(trim(name) == "addr"){
            self = trim(item.substr(item.find(":") + 1));
        } else if(trim(name) == "peers"){
            for(auto& peer : SplitStr(item.substr(item.find(":") + 1), ',')){
                peers.push_back(trim(peer));
            }
        } else if(trim(name) == "election_time_out"){
            electionTimeOut = std::stoull(trim(item.substr(item.find(":") + 1)));
        } else if(trim(name) == "ping_time_out"){
            pingTimeOut = std::stoull(trim(item.substr(item.find(":") + 1)));
        }
    }

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
    role = FOLLOWER;
    isVoted = NotVoted;
    leader = NOLEADER;
    lastReceivedTime = GetCurrentMillSeconds();
    port = std::stoul(SplitStr(self, ':')[1]);
    // std::cout << "self addr is " << self << std::endl;
    // std::cout << "ping time out is " << pingTimeOut << std::endl;
    // std::cout << "election time out is " << electionTimeOut << std::endl;
    // std::cout << "port is " << port << std::endl;
    // for(auto& peer : peers){
    //     std::cout << "peer is " << peer << std::endl;
    // }
}

Paxos::~Paxos(){
    delete kv;
}

std::pair<int64_t, std::string> Paxos::Prepare(int64_t n){
    
}

int Paxos::Accept(){
    return 0;
}

//request vote... 
int Paxos::receiveRequestVote(){
    if(isVoted == NotVoted && role == CANDIDATE && leader == NOLEADER){
        isVoted = HasVoted;
        return 1;
    }
    return 0;
}

//timer event...
void Paxos::receivePing(std::string leader_addr){
    std::cout << "receive ping from " << leader_addr << std::endl;
    lastReceivedTime = GetCurrentMillSeconds();
    // if(leader == NOLEADER){
    leader = leader_addr;
    role = FOLLOWER;
    // }
}


//chosen as leader...
void Paxos::chosenAsLeader(){
    role = MASTER;
    leader = self;
    std::cout << self << " is chosed as leader" << std::endl;

}

//happen when time out with ping...
void Paxos::receivePingTimeout(){
    role = CANDIDATE;
    isVoted = NotVoted;
    leader = NOLEADER;
    votes = 0;
    //wait a random time to start to request vote...
    // usleep(GenerateRandomNumber() * pingTimeOut / 20 * 1000);
    // if(isVoted == NotVoted){
    //     votes += 1;
    //     isVoted = HasVoted;
    //     std::cout << "vote for self" << std::endl;
    // }
    auto randomNum = GenerateRandomNumber();
    if(isVoted == NotVoted && randomNum > 0.5){
        votes += 1;
        isVoted = HasVoted;
    }
    for(auto& peer : peers){
        //send request vote...
        try{
            auto addr = SplitStr(peer, ':');
            Client* cli = new Client(addr[0], addr[1]);
            std::string message(REQUESTSIGN);
            message += "\t";
            message += OPERATIONTYPE;
            message += "\t";
            message += "REQUESTVOTE";

            if(cli->Write(message) < 0){
                std::cout << "failed to send data to " << peer << std::endl;
            }
            std::string resp = cli->Read();
            
            delete cli;

            auto items = SplitStr(resp, '\t');
           
            if(items[0] == RESPSIGN && items[1] == RESPOK && items[2] == STRINGTYPE && items[3] == "OK"){
                std::cout << "get vote from " << peer << std::endl;
                votes += 1;
            }
        } catch(const std::exception& e){
            //std::cout << e.what() << std::endl;
        }
    }

    std::cout << "final votes is " << votes << std::endl;
    if (votes > (1 + peers.size()) / 2.0){
        this->chosenAsLeader();
    }
}

void Paxos::ping(const boost::system::error_code& /*e*/, boost::asio::steady_timer* t){
    if(role == MASTER){
        for(auto &peer : peers){
            //use try to fix error happen
            try{
                auto addr = SplitStr(peer, ':');
                Client* cli = new Client(addr[0], addr[1]);
                std::string message(REQUESTSIGN);
                message += "\t";
                message += OPERATIONTYPE;
                message += "\t";
                message += "PING";
                message += "\t";
                message += STRINGTYPE;
                message += "\t";
                message += self;
                cli->Write(message);
                delete cli;
            } catch(std::exception& e){
                std::cout << "failed to ping " << peer << ": " << e.what() << std::endl;
            }
        }
    }
    t->expires_at(t->expiry() + boost::asio::chrono::milliseconds(pingTimeOut / 4));
    t->async_wait(boost::bind(&Paxos::ping, this, boost::asio::placeholders::error, t));
}

//timer event, check if ping time out or not, interval is set as interval / 2
void Paxos::isPingTimeOut(const boost::system::error_code& /*e*/, boost::asio::steady_timer* t){
    //std::cout << "check if ping time out or not" << std::endl;
    if(role != MASTER && GetCurrentMillSeconds() - lastReceivedTime >= pingTimeOut){
        std::cout << "ping time out, start election" << std::endl;
        this->receivePingTimeout();
    }
    t->expires_at(t->expiry() + boost::asio::chrono::milliseconds(pingTimeOut));
    t->async_wait(boost::bind(&Paxos::isPingTimeOut, this, boost::asio::placeholders::error, t));
}


std::string Paxos::handler(const std::string& s){
    auto items = SplitStr(s, '\t');

    if(items[0] == "GET"){
        return kv->Get(items[1]);
    } else if(items[0] == "PUT" || items[0] == "DEL"){
        if(role == MASTER){
            commands[this->GenerateProposalID()] = s;
            for(auto iter = commands.begin(); iter != commands.end(); iter++){
                std::cout << iter->first << std::endl;
                std::cout << iter->second << std::endl;
            }
        } else {
            auto items = SplitStr(leader, ':');
            try{
                Client* cli = new Client(items[0], items[1]);
                cli->Write(s);
                delete cli;
            }catch(std::exception& e){
                std::cout << "failed to transfer data to leader " << e.what() << std::endl;
            }
        }
       
    }
    
    if(items[0] != REQUESTSIGN || items[1] != OPERATIONTYPE){
        std::string resp(RESPSIGN);
        resp += "\t";
        resp += RESPERROR;
        resp += "\t";
        resp += STRINGTYPE;
        resp += "\t";
        resp += "data type is not valid!"; 
        return resp;
    }
    if(items[2] == "PING"){
        if(items[3] == STRINGTYPE){
            this->receivePing(items[4]);
        }
        //fix, no string return
        std::string resp(RESPSIGN);
        resp += "\t";
        resp += RESPOK;
        resp += "\t";
        resp += STRINGTYPE;
        resp += "\t";
        resp += "OK"; 
        return resp;
    } else if (items[2] == "REQUESTVOTE"){
        if(this->receiveRequestVote() == 1){
            std::string resp(RESPSIGN);
            resp += "\t";
            resp += RESPOK;
            resp += "\t";
            resp += STRINGTYPE;
            resp += "\t";
            resp += "OK"; 
            return resp;
        }else{
            std::string resp(RESPSIGN);
            resp += "\t";
            resp += RESPOK;
            resp += "\t";
            resp += STRINGTYPE;
            resp += "\t";
            resp += "REFUSED"; 
            return resp;
        }
    }  else{
        std::string resp(RESPSIGN);
        resp += "\t";
        resp += RESPERROR;
        resp += "\t";
        resp += STRINGTYPE;
        resp += "\t";
        resp += "unrecognized operation found!"; 
        return resp;
    }
    return NULL;
}
void Paxos::run(){
    server = new tcp_server(io, std::bind(&Paxos::handler, this, _1), port);
    
    //check if time out or not, if time out, start to request vote...
    boost::asio::steady_timer t(io, boost::asio::chrono::milliseconds(pingTimeOut));
    t.async_wait(boost::bind(&Paxos::isPingTimeOut, this, boost::asio::placeholders::error, &t));

    // //timer event, ping other paxos instances...
    boost::asio::steady_timer t1(io, boost::asio::chrono::milliseconds(pingTimeOut/4));
    t1.async_wait(boost::bind(&Paxos::ping, this, boost::asio::placeholders::error, &t1));

    io.run();
}