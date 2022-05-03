#include "../src/paxos.h"

//g++ -o test_paxos test_paxos.cpp ../src/* -lpthread
int main(){
    Paxos* p = new Paxos();
    p->run();
    return -1;
}