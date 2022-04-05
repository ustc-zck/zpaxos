#include <string>
#include <unordered_map>
#include <set>

class Node{
    public:
        Node();
        ~Node();
    private:
        int role; //0 proposer, 1 acceptor
        int64_t acceptedProposalID; //目前接收过的最大的proposal ID
        std::string acceptedCommand; //acceptedProposalID对应的command
        int64_t minProposalID; //目前最小的proposal id
        std::set<int64_t> logIDs;
        std::unordered_map<int64_t, std::string> commands;
};