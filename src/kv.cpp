#include "kv.h"

KV::KV(){
}

KV::~KV(){ 

}

std::string KV::Get(std::string key){
    return storage[key];
}

void KV::Put(std::string key, std::string val){
    storage[key] = val;
}

void KV::Del(std::string key){
    storage.erase(key);
}
