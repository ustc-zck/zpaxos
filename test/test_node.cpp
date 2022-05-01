#include "../src/node.h"
#include <iostream>

//g++ -o test_node test_node.cpp ../src/*
int main(){
    Node* node = new Node();
    std::cout << node->maxProposalID << std::endl;
    std::cout << node->maxProposalCommand << std::endl;
    std::cout << node->minProposalID << std::endl;
    return 0;
}