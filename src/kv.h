#include <unordered_map>
#include <string> 

class KV{
    public:
        KV();
        ~KV();
        std::string Get(std::string key);
        void Put(std::string key, std::string val);
        void Del(std::string key);
    private:
        std::unordered_map<std::string, std::string> storage;
};
