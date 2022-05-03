#include "utils.h"

uint64_t GetCurrentMillSeconds(){
    uint64_t now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    return now;
}
std::string ReadFromFile(std::string filePath){
    std::ifstream inFile;
    inFile.open(filePath);
    std::stringstream strStream;
    strStream << inFile.rdbuf();
    return strStream.str();
}

void WriteIntoFile(std::string filePath, std::string content){
    std::ofstream file(filePath);
    file << content;
}

std::vector<std::string> SplitStr(std::string str, char delimiter){
    std::vector<std::string> result;
    int i, j = 0;
    for(i = 0; i < str.size(); i++){
        if(str[i] == delimiter){
            std::string item = str.substr(j, i - j);
            if(item.length() > 0){
                result.push_back(item);
            }
            j = i + 1;
        }
    }
    if(str[str.length() - 1] != delimiter){
        result.push_back(str.substr(j));
    }
    return result;
}

std::string ltrim(const std::string &s){
    size_t start = s.find_first_not_of(WHITESPACE);
    return (start == std::string::npos) ? "" : s.substr(start);
}

std::string rtrim(const std::string &s){
    size_t end = s.find_last_not_of(WHITESPACE);
    return (end == std::string::npos) ? "" : s.substr(0, end + 1);
}

std::string trim(const std::string &s) {
    return rtrim(ltrim(s));
}

float GenerateRandomNumber(){
    srand((unsigned)time(NULL) ); 
 
    return (float) rand()/RAND_MAX;
}


