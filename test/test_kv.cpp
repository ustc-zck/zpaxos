#include "../src/kv.h"
#include <iostream>

//g++ -o test_kv test_kv.cpp ../src/*
int main(){
    KV* kv = new KV();
    kv->Put("a", "1");
    std::cout << kv->Get("a") << std::endl;
    kv->Del("a");
    std::cout << kv->Get("a") << std::endl;
    return 0;
}