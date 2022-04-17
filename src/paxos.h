#include "node.h"
#include <vector>
#include <string>

using namespace std;
class Paxos{
    public:
        Paxos();
        ~Paxos();
        //id = -1 represents reject, id > 1 represents accpet...
        std::pair<int64_t, std::string> Prepare(int64_t n);
        //int Promise();
        int Accept();
        //timer event...
        int Ping();
        //request vote...   

    private:
        Node* self;
        std::vector<std::string> peers;
        int64_t lastReceivedTime;
        int64_t electionTimeOut;

};